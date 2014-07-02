/************************************************
 *  count.cpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include "count.hpp"

namespace esapp
{

const StringCounter::TermId StringCounter::BOUNDARY_ = 0;

/************************************************
 * Implementation: class StringCounter
 ************************************************/

StringCounter::StringCounter(size_t max_len, double smooth, Term boundary)
    : max_len_(max_len), smooth_(smooth), boundary_(boundary),
      f_avgs_(max_len), hl_avgs_(max_len), hr_avgs_(max_len), str_nums_(max_len)
{
    if (smooth_ < 0)
    {
        throw std::invalid_argument("The smoothing parameter must be " \
                                    "greater than or equal to 0.");
    }
}

void StringCounter::fit(std::vector<Sequence> const &sequences)
{
    // concatenate sequences
    auto s = sequences[0];
    for (decltype(sequences.size()) i = 1; i < sequences.size(); i++)
    {
        s.push_back(boundary_);
        s.append(sequences[i]);
    }

    // convert term sequence into id sequence
    auto idx_seq = init_char_id_map(s);

    // calculate average statistics of substrings
    sa_.construct(idx_seq, char_id_map_.size());
    calc_avg();

    // initialize vector of preserve lengths
    count_min_lens_.resize(sa_.size());
    std::fill(count_min_lens_.begin(), count_min_lens_.end(), 0);
}

void StringCounter::set_pres(std::vector<size_t> pres, size_t p, size_t n)
{
    // update preserve lengths (for suffix array)
    size_t len = 1;
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
    auto &s = sa_.data();
    auto i = p;
    for (auto pos : pres)
    {
        trie_.decrease(s.begin() + i, s.begin() + p + pos);
        i = p + pos;
    }

    trie_.decrease(s.begin() + i, s.begin() + p + n);
}

void StringCounter::unset_pres(std::vector<size_t> pres, size_t p, size_t n)
{
    // update preserve lengths (for suffix array)
    std::fill(count_min_lens_.begin() + p, count_min_lens_.begin() + p + n, 0);

    // update trie
    auto &s = sa_.data();
    auto i = p;
    for (auto pos : pres)
    {
        trie_.increase(s.begin() + i, s.begin() + p + pos);
        i = p + pos;
    }

    trie_.increase(s.begin() + i, s.begin() + p + n);
}

double StringCounter::get_iv(size_t i, size_t n) const
{
    if (n > max_len_ || f_avgs_[n - 1] == 0.0) { return 0.0; }

    double f;
    auto j = sa_.rank(i);
    if (sa_.lcp(j) >= n || (j + 1 < sa_.size() && sa_.lcp(j + 1) >= n))
    {
        auto &s = sa_.data();
        auto node = trie_.find(s.begin() + i, s.begin() + i + n);
        f = node->f;
    }
    else
    {
        f = n > count_min_lens_[i] ? 1 : 0;
    }

    return pow(f / f_avgs_[n - 1], n);
}

double StringCounter::get_hl(size_t i, size_t n) const
{
    if (n > max_len_ || hl_avgs_[n - 1] == 0.0) { return 0.0; }

    double hl;
    auto j = sa_.rank(i);
    if (sa_.lcp(j) >= n || (j + 1 < sa_.size() && sa_.lcp(j + 1) >= n))
    {
        auto &s = sa_.data();
        auto node = trie_.find(s.begin() + i, s.begin() + i + n);
        hl = node->hl;
    }
    else
    {
        hl = h1_;
    }

    return hl / hl_avgs_[n - 1];
}

double StringCounter::get_hr(size_t i, size_t n) const
{
    if (n > max_len_ || hr_avgs_[n - 1] == 0.0) { return 0.0; }

    double hr;
    auto j = sa_.rank(i);
    if (sa_.lcp(j) >= n || (j + 1 < sa_.size() && sa_.lcp(j + 1) >= n))
    {
        auto &s = sa_.data();
        auto node = trie_.find(s.begin() + i, s.begin() + i + n);
        hr = node->hr;
    }
    else
    {
        hr = h1_;
    }

    return hr / hr_avgs_[n - 1];
}

StringCounter::IdSequence StringCounter::init_char_id_map(Sequence const &s)
{
    char_id_map_.clear();
    char_id_map_[boundary_] = BOUNDARY_;

    decltype(init_char_id_map(s)) id_seq;
    id_seq.reserve(s.size());

    typename decltype(id_seq)::value_type idx = 1;
    for (auto const &c : s)
    {
        if (char_id_map_.find(c) == char_id_map_.end())
        {
            char_id_map_[c] = idx++;
        }

        id_seq.push_back(char_id_map_[c]);
    }

    return id_seq;
}

StringCounter::IdSequence StringCounter::to_char_ids(Sequence const &s) const
{
    decltype(to_char_ids(s)) idx_seq;
    idx_seq.reserve(s.size());
    for (auto const &c : s)
    {
        idx_seq.push_back(char_id_map_.at(c));
    }

    return idx_seq;
}

void StringCounter::calc_avg(void)
{
    typedef std::pair<size_t, size_t> StackItem;
    std::stack<StackItem> lcp_stack;
    lcp_stack.emplace(0, 0);
    h1_ = entropy(TermCounts({{BOUNDARY_, 1}}));

    std::fill(f_avgs_.begin(), f_avgs_.end(), 0);
    std::fill(hl_avgs_.begin(), hl_avgs_.end(), 0);
    std::fill(hr_avgs_.begin(), hr_avgs_.end(), 0);
    std::fill(str_nums_.begin(), str_nums_.end(), 0);

    auto n = sa_.size();
    auto &s = sa_.data();
    decltype(sa_.lcp(0)) prev_lcp = 0;
    for (decltype(n) i = 0; i < n; i++)
    {
        decltype(prev_lcp) lcp = 0;
        if (i + 1 < n)
        {
            if (s[sa_[i + 1]] == BOUNDARY_) { continue; }

            lcp = std::min(sa_.lcp(i + 1), max_len_);
        }

        // count substrings occurring only once
        for (auto j = std::max(lcp, prev_lcp), idx = sa_[i] + j;
             j < max_len_ && idx < s.size() && s[idx] != BOUNDARY_; j++, idx++)
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
                lcp_stack.emplace(i, lcp);
            }

            // calculate sp1l
            TermCounts sp1l;
            for (auto k = top.first; k <= i; k++)
            {
                auto c = sa_[k] > 0 ? s[sa_[k] - 1] : BOUNDARY_;
                sp1l[c]++;
            }

            auto hl = entropy(sp1l);
            auto f = i - top.first + 1;
            for (auto j = lcp_stack.top().second; j < top.second; j++)
            {
                // calculate sp1r
                TermCounts sp1r;
                for (auto k = top.first; k <= i; k++)
                {
                    auto idx = sa_[k] + j + 1;
                    auto c = idx < n ? s[idx] : BOUNDARY_;
                    sp1r[c]++;
                }

                auto hr = entropy(sp1r);
                f_avgs_[j] += f;
                hl_avgs_[j] += hl;
                hr_avgs_[j] += hr;
                str_nums_[j]++;

                auto node = trie_.insert(s.begin() + sa_[i],
                                         s.begin() + sa_[i] + j + 1);
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

double StringCounter::entropy(TermCounts const &counts) const
{
    auto num_events = char_id_map_.size();
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

} // namespace esapp
