/************************************************
 *  sufarr.cpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include "sufarr.hpp"

namespace esapp
{

/************************************************
 * Implementation: class suffix_array
 ************************************************/

suffix_array::suffix_array(void)
{
    // do nothing
}

suffix_array::suffix_array(sequence const &s)
    : s_(s)
{
    construct();
}

suffix_array::suffix_array(sequence const &s, size_t num_alphas)
{
    construct(num_alphas);
}

void suffix_array::construct(sequence const &s)
{
    s_ = s;
    construct();
}

void suffix_array::construct(sequence const &s, size_t num_alphas)
{
    s_ = s;
    construct(num_alphas);
}

void suffix_array::construct(void)
{
    construct(*(std::max_element(s_.begin(), s_.end())));
}

void suffix_array::construct(size_t num_alphas)
{
    auto len_s = s_.size() + 1;
    sa_.resize(len_s);

    std::vector<value_type> bkt(num_alphas + 1);
    gen_sa(s_.begin(), sa_, bkt, len_s, num_alphas + 1);
    gen_isa();
    gen_lcpa();
}

suffix_array::const_iterator suffix_array::find(sequence const &t) const
{
    auto len_s = s_.size(), len_t = t.size();
    value_type l = 1, r = len_s;
    while (l + 1 < r)
    {
        auto m = (l + r) / 2, mlcp = calc_lcp(t, m);
        if (mlcp == len_t)                      { return sa_.begin() + m; }
        else if (t[mlcp] < s_[sa_[m] + mlcp])   { r = m; }
        else                                    { l = m; }
    }

    return sa_.end();
}

suffix_array::const_iterator suffix_array::lower_bound(sequence const &t) const
{
    auto len_s = s_.size(), len_t = t.size();
    auto llcp = calc_lcp(t, 1), rlcp = calc_lcp(t, len_s);

    value_type sa_idx;
    if (llcp == len_t || t[llcp] < s_[sa_[1] + llcp])
    {
        sa_idx = 1;
    }
    else if (rlcp < len_t && t[rlcp] > s_[sa_[len_s] + rlcp])
    {
        sa_idx = len_s + 1;
    }
    else
    {
        value_type l = 1, r = len_s;
        while (l + 1 < r)
        {
            auto m = (l + r) / 2, mlcp = calc_lcp(t, m);
            if (mlcp == len_t || t[mlcp] < s_[sa_[m] + mlcp])   { r = m; }
            else                                                { l = m; }
        }

        sa_idx = r;
    }

    return sa_.begin() + sa_idx;
}

suffix_array::const_iterator suffix_array::upper_bound(sequence const &t) const
{
    auto len_s = s_.size(), len_t = t.size();
    auto llcp = calc_lcp(t, 1), rlcp = calc_lcp(t, len_s);

    value_type sa_idx;
    if (llcp < len_t && t[llcp] < s_[sa_[1] + llcp])
    {
        sa_idx = 1;
    }
    else if (rlcp == len_t || t[rlcp] > s_[sa_[len_s] + rlcp])
    {
        sa_idx = len_s + 1;
    }
    else
    {
        value_type l = 1, r = len_s;
        while (l + 1 < r)
        {
            auto m = (l + r) / 2, mlcp = calc_lcp(t, m);
            if (mlcp < len_t && t[mlcp] < s_[sa_[m] + mlcp])    { r = m; }
            else                                                { l = m; }
        }

        sa_idx = r;
    }

    return sa_.begin() + sa_idx;
}

suffix_array::value_type suffix_array::calc_lcp(sequence const &t, size_t i) const
{
    decltype(calc_lcp(t, i)) lcp = 0;
    auto len_s = s_.size(), len_t = t.size();
    while (lcp < len_t && sa_[i] + lcp < len_s &&
           t[lcp] == s_[sa_[i] + lcp]) { lcp++; }

    return lcp;
}

inline bool suffix_array::is_lms(std::vector<bool> const &suf_types,
                                 size_t i, size_t len_s)
{
    return i > 0 && i < len_s && suf_types[i] && !suf_types[i - 1];
}

template <typename T>
void suffix_array::init_bkt(T const &s, std::vector<value_type> &bkt,
                            size_t len_s, size_t num_alphas, bool end)
{
    fill_n(bkt.begin(), num_alphas, 0);
    bkt[0] = 1;
    for (decltype(len_s) i = 0; i < len_s - 1; i++)
    {
        bkt[s[i]]++;
    }

    typedef std::remove_reference<decltype(bkt)>::type Bkt;
    Bkt::value_type sum = 0;
    for (decltype(num_alphas) i = 0; i < num_alphas; i++)
    {
        sum += bkt[i];
        bkt[i] = end ? sum : sum - bkt[i];
    }
}

template <typename T>
void suffix_array::induce(T const &s, std::vector<bool> const &suf_types,
                          std::vector<value_type> &sa,
                          std::vector<value_type> &bkt,
                          size_t len_s, size_t num_alphas)
{
    init_bkt(s, bkt, len_s, num_alphas, false);
    for (decltype(len_s) i = 0; i < len_s; i++)
    {
        auto j = sa[i];
        if (j > 0 && j < len_s && !suf_types[j - 1])
        {
            j--;
            sa[bkt[s[j]]++] = j;
        }
    }

    init_bkt(s, bkt, len_s, num_alphas, true);
    for (decltype(len_s) i = len_s; i > 0; i--)
    {
        auto j = sa[i - 1];
        if (j > 0 && j < len_s && suf_types[j - 1])
        {
            j--;
            sa[--bkt[s[j]]] = j;
        }
    }
}

template <typename T>
void suffix_array::gen_sa(T const &s, std::vector<value_type> &sa,
                          std::vector<value_type> &bkt,
                          size_t len_s, size_t num_alphas)
{
    // LS-type array in bits
    std::vector<bool> suf_types(len_s);

    // classify the type of each character
    suf_types[len_s - 1] = true;
    for (decltype(len_s) i = len_s - 1; i > 0; i--)
    {
        auto j = i - 1;
        suf_types[j] = (s[j] < s[i]) || (s[j] == s[i] && suf_types[i]);
    }

    // stage 1: reduce the problem by at least 1/2 sort all the S-substrings
    init_bkt(s, bkt, len_s, num_alphas, true);
    fill_n(sa.begin(), len_s, std::numeric_limits<value_type>::max());
    for (decltype(len_s) i = 1; i < len_s; i++)
    {
        if (is_lms(suf_types, i, len_s))
        {
            sa[--bkt[s[i]]] = i;
        }
    }

    induce(s, suf_types, sa, bkt, len_s, num_alphas);

    // compact all the sorted substrings into the first n1 items of SA
    decltype(len_s) num_lms = 0;
    for (decltype(len_s) i = 0; i < len_s; i++)
    {
        if (is_lms(suf_types, sa[i], len_s))
        {
            sa[num_lms++] = sa[i];
        }
    }

    // find the lexicographic names of substrings
    auto prev = sa[0];
    decltype(len_s) num_names = 1;
    fill_n(sa.begin() + num_lms, len_s - num_lms,
           std::numeric_limits<value_type>::max());
    sa[num_lms + (prev >> 1)] = 0;
    for (decltype(num_lms) i = 1; i < num_lms; i++)
    {
        auto pos = sa[i];
        for (decltype(len_s) d = 0; d < len_s; d++)
        {
            if (s[pos + d] != s[prev + d] ||
                suf_types[pos + d] != suf_types[prev + d])
            {
                num_names++;
                prev = pos;
                break;
            }
            else if (d > 0 && (is_lms(suf_types, pos + d, len_s) ||
                               is_lms(suf_types, prev + d, len_s)))
            {
                break;
            }
        }

        sa[num_lms + (pos >> 1)] = num_names - 1;
    }

    for (decltype(len_s) i = len_s - 1, j = len_s - 1; i >= num_lms; i--)
    {
        if (sa[i] < num_names)
        {
            sa[j--] = sa[i];
        }
    }

    // stage 2: solve the reduced problem
    auto s1 = sa.begin() + len_s - num_lms;
    if (num_names < num_lms)
    {
        // recurse if names are not yet unique
        bkt.resize(num_names);
        gen_sa(s1, sa, bkt, num_lms, num_names);
        bkt.resize(num_alphas);
    }
    else
    {
        // generate the suffix array of s1 directly
        for (decltype(num_lms) i = 0; i < num_lms; i++)
        {
            sa[s1[i]] = i;
        }
    }

    // stage 3: induce the result for the original problem
    for (decltype(len_s) i = 1, j = 0; i < len_s; i++)
    {
        if (is_lms(suf_types, i, len_s))
        {
            // get p1
            s1[j++] = i;
        }
    }

    // get index in s
    for (decltype(num_lms) i = 0; i < num_lms; i++)
    {
        sa[i] = s1[sa[i]];
    }

    init_bkt(s, bkt, len_s, num_alphas, true);
    fill_n(sa.begin() + num_lms, len_s - num_lms,
           std::numeric_limits<value_type>::max());
    for (decltype(num_lms) i = num_lms; i-- > 0; )
    {
        auto j = sa[i];
        sa[i] = std::numeric_limits<decltype(j)>::max();
        sa[--bkt[s[j]]] = j;
    }

    induce(s, suf_types, sa, bkt, len_s, num_alphas);
}

void suffix_array::gen_isa(void)
{
    isa_.resize(sa_.size());
    for (decltype(sa_.size()) i = 0; i < sa_.size(); i++)
    {
        isa_[sa_[i]] = i;
    }
}

void suffix_array::gen_lcpa(void)
{
    lcpa_.resize(sa_.size());
    typename decltype(lcpa_)::value_type lcp = 0;
    for (decltype(isa_.size()) i = 0; i < isa_.size(); i++)
    {
        if (isa_[i] > 0)
        {
            auto j = sa_[isa_[i] - 1];
            if (lcp > 0) { lcp--; }
            while (s_[i + lcp] == s_[j + lcp] && s_[i + lcp] != 0) { lcp++; }

            lcpa_[isa_[i]] = lcp;
        }
    }
}

} // namespace esapp
