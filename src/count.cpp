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
        s.push_back(L'\0');
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
    // update preserve lengths
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
}

void StringCounter::unset_pres(std::vector<size_t> pres, size_t p, size_t n)
{
    // update preserve lengths
    std::fill(count_min_lens_.begin() + p, count_min_lens_.begin() + p + n, 0);
}

double StringCounter::get_iv(size_t i, size_t n) const
{
    if (n > max_len_ || f_avgs_[n - 1] == 0.0) { return 0.0; }

    auto &s = sa_.data();
    IdSequence t(s.begin() + i, s.begin() + i + n);

    auto lb = sa_.lower_bound(t), ub = sa_.upper_bound(t);
    double f = 0;
    for (auto it = lb; it != ub; ++it)
    {
        auto j = *it;
        if (n > count_min_lens_[j]) { f += 1; }
    }

    return pow(f / f_avgs_[n - 1], n);
}

double StringCounter::get_hl(size_t i, size_t n) const
{
    if (n > max_len_ || hl_avgs_[n - 1] == 0.0) { return 0.0; }

    auto &s = sa_.data();
    IdSequence t(s.begin() + i, s.begin() + i + n);

    auto lb = sa_.lower_bound(t), ub = sa_.upper_bound(t);
    TermCounts sp1l(char_id_map_.size());
    for (auto it = lb; it != ub; ++it)
    {
        auto j = *it;
        if (j > 0)              { sp1l[s[j - 1]]++; }
        else                    { sp1l[BOUNDARY_]++; }
    }

    return entropy(sp1l) / hl_avgs_[n - 1];
}

double StringCounter::get_hr(size_t i, size_t n) const
{
    if (n > max_len_ || hr_avgs_[n - 1] == 0.0) { return 0.0; }

    auto &s = sa_.data();
    IdSequence t(s.begin() + i, s.begin() + i + n);

    auto lb = sa_.lower_bound(t), ub = sa_.upper_bound(t);
    TermCounts sp1r(char_id_map_.size());
    for (auto it = lb; it != ub; ++it)
    {
        auto j = *it;
        if (j + n < sa_.size()) { sp1r[s[j + n]]++; }
        else                    { sp1r[BOUNDARY_]++; }
    }

    return entropy(sp1r) / hr_avgs_[n - 1];
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
    typedef std::tuple<size_t, size_t, size_t, TermCounts> StackItem;
    std::stack<StackItem> lcp_stack;
    lcp_stack.emplace(0, 0, 0, TermCounts({{BOUNDARY_, 1}}));
    auto h1 = entropy(std::get<3>(lcp_stack.top()));

    std::fill(f_avgs_.begin(), f_avgs_.end(), 0);
    std::fill(hl_avgs_.begin(), hl_avgs_.end(), 0);
    std::fill(hr_avgs_.begin(), hr_avgs_.end(), 0);
    std::fill(str_nums_.begin(), str_nums_.end(), 0);
    std::vector<TermCounts> sp1r_vec(max_len_);

    auto n = sa_.size();
    auto &s = sa_.data();
    for (decltype(n) i = 1; i < n; i++)
    {
        if (s[sa_[i]] == BOUNDARY_) { continue; }

        auto lcp = std::min(sa_.lcp(i), max_len_);

        // count substrings occurring only once
        for (auto j = std::max(lcp, std::min(sa_.lcp(i - 1), max_len_)),
                  idx = sa_[i - 1] + j;
             j < max_len_ && idx < s.size() && s[idx] != BOUNDARY_; j++, idx++)
        {
            f_avgs_[j]++;
            hl_avgs_[j] += h1;
            hr_avgs_[j] += h1;
            str_nums_[j]++;

            sp1r_vec[j].clear();
        }

        while (lcp < std::get<1>(lcp_stack.top()))
        {
            auto top = lcp_stack.top();
            lcp_stack.pop();
            if (lcp > std::get<1>(lcp_stack.top()))
            {
                auto sp1l = (sa_[i - 1] > 0) ? s[sa_[i - 1] - 1] : BOUNDARY_;
                lcp_stack.emplace(i - 1, lcp, 1, TermCounts({{sp1l, 1}}));
            }

            auto f = std::get<2>(top);
            auto hl = entropy(std::get<3>(top));
            for (auto j = std::get<1>(lcp_stack.top());
                 j < std::get<1>(top); j++)
            {
                auto hr = entropy(sp1r_vec[j]);
                f_avgs_[j] += f;
                hl_avgs_[j] += hl;
                hr_avgs_[j] += hr;
                str_nums_[j]++;
                sp1r_vec[j].clear();
            }

            if (lcp_stack.size() > 1)
            {
                // update f and sp1l
                std::get<2>(lcp_stack.top()) += std::get<2>(top) - 1;
                for (auto const &kv : std::get<3>(top))
                {
                    std::get<3>(lcp_stack.top())[kv.first] += kv.second;
                }

                auto j = std::get<0>(top);
                auto sp1l = (sa_[j] > 0) ? s[sa_[j] - 1] : BOUNDARY_;
                std::get<3>(lcp_stack.top())[sp1l]--;
            }
        }

        if (lcp > std::get<1>(lcp_stack.top()))
        {
            auto sp1l = (sa_[i - 1] > 0) ? s[sa_[i - 1] - 1] : BOUNDARY_;
            lcp_stack.emplace(i - 1, lcp, 1, TermCounts({{sp1l, 1}}));
        }

        // update f and sp1l
        auto sp1l = (sa_[i] > 0) ? s[sa_[i] - 1] : BOUNDARY_;
        std::get<2>(lcp_stack.top())++;
        std::get<3>(lcp_stack.top())[sp1l]++;

        // update sp1r
        for (decltype(max_len_) j = 0, idx = sa_[i] + j + 1;
             j < max_len_; j++, idx++)
        {
            if (idx == s.size() || s[idx] == BOUNDARY_)
            {
                sp1r_vec[j][BOUNDARY_]++;
                break;
            }

            sp1r_vec[j][s[idx]]++;
        }
    }

    // count substrings occurring only once
    auto i = sa_.size() - 1;
    for (auto j = std::min(sa_.lcp(i), max_len_), idx = sa_[i] + j;
         j < max_len_ && idx < s.size() && s[idx] != BOUNDARY_; j++, idx++)
    {
        f_avgs_[j]++;
        hl_avgs_[j] += h1;
        hr_avgs_[j] += h1;
        str_nums_[j]++;

        sp1r_vec[j].clear();
    }

    while (lcp_stack.size() > 1)
    {
        auto top = lcp_stack.top();
        lcp_stack.pop();

        auto f = std::get<2>(top);
        auto hl = entropy(std::get<3>(top));
        for (auto j = std::get<1>(lcp_stack.top());
             j < std::get<1>(top); j++)
        {
            auto hr = entropy(sp1r_vec[j]);
            f_avgs_[j] += f;
            hl_avgs_[j] += hl;
            hr_avgs_[j] += hr;
            str_nums_[j]++;
            sp1r_vec[j].clear();
        }

        // update f and sp1l
        std::get<2>(lcp_stack.top()) += std::get<2>(top) - 1;
        for (auto const &kv : std::get<3>(top))
        {
            std::get<3>(lcp_stack.top())[kv.first] += kv.second;
        }

        auto j = std::get<0>(top);
        auto sp1l = (sa_[j] > 0) ? s[sa_[j] - 1] : BOUNDARY_;
        std::get<3>(lcp_stack.top())[sp1l]--;
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
