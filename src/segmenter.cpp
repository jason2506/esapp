/************************************************
 *  segmenter.cpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include "segmenter.hpp"

namespace esapp
{

namespace impl
{

segmenter::term_id const segmenter::BOUNDARY_ = 0;

/************************************************
 * Implementation: class segmenter
 ************************************************/

segmenter::segmenter(double lrv_exp, size_type max_iters,
                     size_type max_len, double smooth)
    : lrv_exp_(lrv_exp), max_iters_(max_iters), max_len_(max_len), smooth_(smooth),
      f_avgs_(max_len), hl_avgs_(max_len), hr_avgs_(max_len), str_nums_(max_len)
{
    if (lrv_exp_ < 0)
    {
        throw ::std::invalid_argument("The exponent parameter of LRV must " \
                                      "be greater than or equal to 0.");
    }

    if (smooth_ < 0)
    {
        throw ::std::invalid_argument("The smoothing parameter must be " \
                                      "greater than or equal to 0.");
    }
}

::std::vector<::std::vector<::std::string>> segmenter::fit_and_segment(
    ::std::vector<::std::string> const &sequences)
{
    // pre-segment sequences by alphabets, numbers, and symbols
    auto tokens = make_filter_iterator(
        [] (::std::vector<term_type> const &token) { return ischs(token[0]); },
        make_flatten_iterator(
            make_map_iterator(
                [](::std::string const &s) { return token_iterator(s); },
                make_generator_adaptor(sequences.begin(), sequences.end())
            )
        )
    );

    // calculate average statistics of substrings
    sa_.construct(tokens);
    calc_avgs();

    // initialize vector of preserve lengths
    count_min_lens_.resize(sa_.size());
    ::std::fill(count_min_lens_.begin(), count_min_lens_.end(), 0);

    auto m = sa_.string_count();
    ::std::vector<seg_pos_list> prev_segs(m), segs(m);
    for (decltype(max_iters_) i = 0; i < max_iters_; ++i)
    {
        // segment sequences
        for (decltype(m) j = 0, p = 0; j < m; ++j)
        {
            auto n = sa_.size(j);
            optimize_segment(segs[j], p, n);
            p += n + 1;
        }

        if (prev_segs == segs) { break; }

        // update substring counts
        for (decltype(m) j = 0, p = 0; j < m; ++j)
        {
            auto n = sa_.size(j);
            if (i > 0)
            {
                unset_pres(prev_segs[j], p, n);
            }

            set_pres(segs[j], p, n);
            p += n + 1;
        }

        prev_segs.swap(segs);
    }

    // generate segmented word lists
    auto it = prev_segs.begin();
    decltype(fit_and_segment(sequences)) words_list;
    words_list.reserve(sequences.size());
    for (auto const &sequence : sequences)
    {
        typename decltype(words_list)::value_type words;
        auto begin = sequence.begin();
        for (auto tokens_it = token_iterator(sequence); tokens_it; ++tokens_it)
        {
            auto end = tokens_it.position();
            auto ch = (*tokens_it)[0];
            if (ischs(ch))
            {
                segment_sequence(words, begin, end, *it++);
            }
            else if (!::std::iswspace(ch))
            {
                words.emplace_back(begin, end);
            }

            begin = end;
        }

        words_list.push_back(words);
    }

    return words_list;
}

void segmenter::calc_avgs(void)
{
    h1_ = entropy(term_counts({{BOUNDARY_, 1}}), 1);
    ::std::fill(f_avgs_.begin(), f_avgs_.end(), 0);
    ::std::fill(hl_avgs_.begin(), hl_avgs_.end(), 0);
    ::std::fill(hr_avgs_.begin(), hr_avgs_.end(), 0);
    ::std::fill(str_nums_.begin(), str_nums_.end(), 0);

    constexpr size_type START = 0, LCP = 1, SP1L = 2, SP1R = 3;
    typedef ::std::tuple<size_type, size_type, term_counts, term_counts> stack_item;
    ::std::stack<stack_item> lcp_stack;

    auto node = trie_.get_root();
    ::std::stack<::std::pair<freq_trie::term_type, freq_trie::raw_node_ptr>> trie_path;

    auto const n = sa_.size();
    decltype(sa_.lcp(0)) prev_lcp = 0;
    for (auto i = sa_.string_count(); i < n; i++)
    {
        // count substrings occurring only once
        decltype(prev_lcp) lcp = i + 1 < n ? ::std::min(sa_.lcp(i + 1), max_len_) : 0;
        for (auto j = ::std::max(lcp, prev_lcp), idx = sa_[i] + j;
             j < max_len_ && sa_.term(idx) != BOUNDARY_; j++, idx++)
        {
            f_avgs_[j]++;
            hl_avgs_[j] += h1_;
            hr_avgs_[j] += h1_;
            str_nums_[j]++;
        }

        if (lcp < prev_lcp && !lcp_stack.empty())
        {
            ::std::get<SP1L>(lcp_stack.top())[sa_[i] > 0 ? sa_.term(sa_[i] - 1) : BOUNDARY_]++;
            ::std::get<SP1R>(lcp_stack.top())[sa_.term(sa_[i] + prev_lcp)]++;
        }

        auto top_lcp = lcp_stack.empty() ? 0 : ::std::get<LCP>(lcp_stack.top());
        while (lcp < top_lcp)
        {
            auto top = lcp_stack.top();
            lcp_stack.pop();

            top_lcp = lcp_stack.empty() ? 0 : ::std::get<LCP>(lcp_stack.top());
            if (lcp > top_lcp)
            {
                lcp_stack.emplace(::std::get<START>(top), lcp, term_counts(), term_counts());
                top_lcp = lcp;
            }

            auto f = i - ::std::get<START>(top) + 1;
            auto hl = entropy(::std::get<SP1L>(top), f);
            auto hr = entropy(::std::get<SP1R>(top), f);
            auto j = ::std::get<LCP>(top) - 1;
            f_avgs_[j] += f;
            hl_avgs_[j] += hl;
            hr_avgs_[j] += hr;
            str_nums_[j]++;

            auto c = trie_path.top().first;
            node->f = f;
            node->hl = hl;
            node->hr = hr;
            node = trie_path.top().second;
            trie_path.pop();

            if (j > top_lcp)
            {
                hr = entropy(term_counts({{BOUNDARY_, f}}), f);
                do
                {
                    f_avgs_[j - 1] += f;
                    hl_avgs_[j - 1] += hl;
                    hr_avgs_[j - 1] += hr;
                    str_nums_[j - 1]++;

                    node->f = f;
                    node->hl = hl;
                    node->hr = hr;
                    node = trie_path.top().second;
                    c = trie_path.top().first;
                    trie_path.pop();
                } while (--j > top_lcp);
            }

            if (!lcp_stack.empty())
            {
                ::std::get<SP1R>(lcp_stack.top())[c] = f;
                for (auto &count : ::std::get<SP1L>(top))
                {
                    ::std::get<SP1L>(lcp_stack.top())[count.first] += count.second;
                }
            }
        }

        if (lcp > top_lcp)
        {
            for (auto j = top_lcp; j < lcp; j++)
            {
                auto c = sa_.term(sa_[i] + j);
                trie_path.emplace(c, node);
                node = node->get(c, true);
            }

            lcp_stack.emplace(i, lcp, term_counts(), term_counts());
        }

        if (lcp >= prev_lcp && !lcp_stack.empty())
        {
            ::std::get<SP1L>(lcp_stack.top())[sa_[i] > 0 ? sa_.term(sa_[i] - 1) : BOUNDARY_]++;
            ::std::get<SP1R>(lcp_stack.top())[sa_.term(sa_[i] + lcp)]++;
        }

        prev_lcp = lcp;
    }

    // calculate average
    for (decltype(max_len_) j = 0; j < max_len_ && str_nums_[j] > 0; j++)
    {
        f_avgs_[j] /= str_nums_[j];
        hl_avgs_[j] /= str_nums_[j];
        hr_avgs_[j] /= str_nums_[j];
    }
}

double segmenter::entropy(term_counts const &counts, size_type n) const
{
    auto h = 0.0;
    auto num_events = sa_.alphabet_count();
    double m = n + num_events * smooth_;
    for (auto const &count : counts)
    {
        auto p = (count.second + smooth_) / m;
        h -= p * log(p);
    }

    if (smooth_ > 0)
    {
        auto num_zeros = num_events - counts.size();
        auto p = smooth_ / m;
        h -= num_zeros * (p * log(p));
    }

    return h;
}

void segmenter::set_pres(::std::vector<size_type> const &pres, size_type p, size_type n)
{
    // update preserve lengths (for suffix array)
    size_type len = 1;
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

void segmenter::unset_pres(::std::vector<size_type> const &pres, size_type p, size_type n)
{
    // update preserve lengths (for suffix array)
    ::std::fill(count_min_lens_.begin() + p, count_min_lens_.begin() + p + n, 0);

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

void segmenter::optimize_segment(seg_pos_list &seg, size_type p, size_type n) const
{
    if (n == 0) { return; }

    ::std::vector<size_type> fs(n);
    ::std::vector<double> fv(n);
    ::std::fill(fs.begin(), fs.end(), 0);
    ::std::fill(fv.begin(), fv.end(), -::std::numeric_limits<double>::infinity());
    for (decltype(n) i = 0; i < n; ++i)
    {
        auto node = trie_.get_root();
        auto max_m = ::std::min(n - i, max_len_);
        for (decltype(i) m = 0; m < max_m; ++m)
        {
            if (node)
            {
                auto c = sa_.term(p + i + m);
                node = node->get(c);
            }

            double f, hl, hr;
            if (node)
            {
                f = node->f;
                hl = node->hl;
                hr = node->hr;
            }
            else if (m >= count_min_lens_[p + i])
            {
                hl = hr = h1_;
                f = 1;
            }
            else { continue; }

            f /= f_avgs_[m];
            hl /= hl_avgs_[m];
            hr /= hr_avgs_[m];

            auto score = (m + 1) * log(f) + lrv_exp_ * log(hl * hr);
            if (i == 0)
            {
                fv[m] = score;
            }
            else if (fv[i - 1] + score > fv[i + m])
            {
                fv[i + m] = fv[i - 1] + score;
                fs[i + m] = i;
            }
        }
    }

    seg.clear();
    for (auto i = fs[n - 1]; i > 0; i = fs[i - 1])
    {
        seg.push_back(i);
    }

    ::std::reverse(seg.begin(), seg.end());
}

void segmenter::segment_sequence(::std::vector<::std::string> &words,
                                 ::std::string::const_iterator begin,
                                 ::std::string::const_iterator end,
                                 seg_pos_list const &seg) const
{
    seg_pos_list::value_type prev_pos = 0;
    for (auto const &pos : seg)
    {
        words.emplace_back(begin + prev_pos * 3, begin + pos * 3);
        prev_pos = pos;
    }

    words.emplace_back(begin + prev_pos * 3, end);
}

} // namespace impl

} // namespace esapp
