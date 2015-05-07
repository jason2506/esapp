/************************************************
 *  string_counter.cpp
 *  ESA++
 *
 *  Copyright (c) 2014-2015, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include "string_counter.hpp"

namespace esapp
{

namespace impl
{

const string_counter::term_id string_counter::BOUNDARY_ = 0;

/************************************************
 * Implementation: class string_counter
 ************************************************/

string_counter::string_counter(double lrv_exp, size_t max_len, double smooth)
    : lrv_exp_(lrv_exp), max_len_(max_len), smooth_(smooth),
      f_avgs_(max_len), hl_avgs_(max_len), hr_avgs_(max_len),
      str_nums_(max_len)
{
    if (lrv_exp_ < 0)
    {
        throw std::invalid_argument("The exponent parameter of LRV must be " \
                                    "greater than or equal to 0.");
    }

    if (smooth_ < 0)
    {
        throw std::invalid_argument("The smoothing parameter must be " \
                                    "greater than or equal to 0.");
    }
}

void string_counter::set_pres(std::vector<index_type> const &pres, size_t p, size_t n)
{
    // update preserve lengths (for suffix array)
    index_type len = 1;
    for (auto i = n - 1, j = pres.size(); i > 0; i--)
    {
        count_min_lens_[p + i] = len++;
        if (j > 0 && i == pres[j - 1])
        {
            count_min_lens_[p + i]--;
            len = 1;
            j--;
        }
    }

    count_min_lens_[p] = len - 1;

    // update trie
    auto it = sa_.data_begin();
    auto i = p;
    for (auto const &pos : pres)
    {
        trie_.decrease(it + i, it + p + pos);
        i = p + pos;
    }

    trie_.decrease(it + i, it + p + n);
}

void string_counter::unset_pres(std::vector<index_type> const &pres, size_t p, size_t n)
{
    // update preserve lengths (for suffix array)
    std::fill(count_min_lens_.begin() + p, count_min_lens_.begin() + p + n, 0);

    // update trie
    auto it = sa_.data_begin();
    auto i = p;
    for (auto const &pos : pres)
    {
        trie_.increase(it + i, it + p + pos);
        i = p + pos;
    }

    trie_.increase(it + i, it + p + n);
}

double string_counter::score(size_t i, size_t n) const
{
    if (n > max_len_ || f_avgs_[n - 1] == 0.0)
    {
        return -std::numeric_limits<double>::infinity();
    }

    double f, hl, hr;
    auto j = sa_.rank(i);
    if (sa_.lcp(j) >= n || (j + 1 < sa_.size() && sa_.lcp(j + 1) >= n))
    {
        auto it = sa_.data_begin();
        auto node = trie_.find(it + i, it + i + n);
        f = node->f;
        hl = node->hl;
        hr = node->hr;
    }
    else if (n > count_min_lens_[i])
    {
        f = 1;
        hl = hr = h1_;
    }
    else
    {
        return -std::numeric_limits<double>::infinity();
    }

    f /= f_avgs_[n - 1];
    hl /= hl_avgs_[n - 1];
    hr /= hr_avgs_[n - 1];
    return n * log(f) + lrv_exp_ * log(hl * hr);
}

void string_counter::calc_avg(void)
{
    typedef std::pair<index_type, index_type> stack_item;
    std::stack<stack_item> lcp_stack;
    lcp_stack.emplace(0, 0);
    h1_ = entropy(term_counts({{BOUNDARY_, 1}}));

    std::fill(f_avgs_.begin(), f_avgs_.end(), 0);
    std::fill(hl_avgs_.begin(), hl_avgs_.end(), 0);
    std::fill(hr_avgs_.begin(), hr_avgs_.end(), 0);
    std::fill(str_nums_.begin(), str_nums_.end(), 0);

    auto n = sa_.size();
    auto it = sa_.data_begin();
    decltype(sa_.lcp(0)) prev_lcp = 0;
    for (decltype(n) i = 0; i < n; i++)
    {
        decltype(prev_lcp) lcp = 0;
        if (i + 1 < n)
        {
            if (it[sa_[i + 1]] == BOUNDARY_) { continue; }

            lcp = std::min(sa_.lcp(i + 1), max_len_);
        }

        // count substrings occurring only once
        for (auto j = std::max(lcp, prev_lcp), idx = sa_[i] + j;
             j < max_len_ && it[idx] != BOUNDARY_; j++, idx++)
        {
            f_avgs_[j]++;
            hl_avgs_[j] += h1_;
            hr_avgs_[j] += h1_;
            str_nums_[j]++;
        }

        while (lcp < lcp_stack.top().second)
        {
            auto top = lcp_stack.top();
            lcp_stack.pop();
            if (lcp > lcp_stack.top().second)
            {
                lcp_stack.emplace(top.first, lcp);
            }

            // calculate sp1l
            term_counts sp1l;
            for (auto k = top.first; k <= i; k++)
            {
                auto c = sa_[k] > 0 ? it[sa_[k] - 1] : BOUNDARY_;
                sp1l[c]++;
            }

            auto hl = entropy(sp1l);
            auto f = i - top.first + 1;
            for (auto j = lcp_stack.top().second; j < top.second; j++)
            {
                // calculate sp1r
                term_counts sp1r;
                for (auto k = top.first; k <= i; k++)
                {
                    auto idx = sa_[k] + j + 1;
                    auto c = idx < n ? it[idx] : BOUNDARY_;
                    sp1r[c]++;
                }

                auto hr = entropy(sp1r);
                f_avgs_[j] += f;
                hl_avgs_[j] += hl;
                hr_avgs_[j] += hr;
                str_nums_[j]++;

                auto node = trie_.insert(it + sa_[i],
                                         it + sa_[i] + j + 1);
                node->f = f;
                node->hl = hl;
                node->hr = hr;
            }
        }

        if (lcp > lcp_stack.top().second)
        {
            lcp_stack.emplace(i, lcp);
        }

        prev_lcp = lcp;
    }

    // calculate average
    for (decltype(max_len_) j = 0; j < max_len_; j++)
    {
        if (str_nums_[j] == 0) { break; }

        f_avgs_[j] /= str_nums_[j];
        hl_avgs_[j] /= str_nums_[j];
        hr_avgs_[j] /= str_nums_[j];
    }
}

double string_counter::entropy(term_counts const &counts) const
{
    auto num_events = sa_.alphabet_count();
    auto n = num_events * smooth_;
    for (auto const &count : counts)
    {
        n += count.second;
    }

    auto h = 0.0;
    for (auto const &count : counts)
    {
        auto p = (count.second + smooth_) / n;
        h -= p * log(p);
    }

    if (smooth_ > 0)
    {
        auto num_zeros = num_events - counts.size();
        auto p = smooth_ / n;
        h -= num_zeros * (p * log(p));
    }

    return h;
}

} // namespace impl

} // namespace esapp
