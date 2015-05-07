/************************************************
 *  segmenter.cpp
 *  ESA++
 *
 *  Copyright (c) 2014-2015, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include "segmenter.hpp"

namespace esapp
{

namespace impl
{

/************************************************
 * Implementation: class segmenter
 ************************************************/

segmenter::segmenter(double lrv_exp, size_t max_iters,
                     size_t max_len, double smooth)
    : counter_(lrv_exp, max_len, smooth), max_iters_(max_iters)
{
    // do nothing
}

std::vector<std::vector<std::string>> segmenter::fit_and_segment(
    std::vector<std::string> const &sequences)
{
    // pre-segment sequences by alphabets, numbers, and symbols
    auto tokens = make_filter_iterator(
        [] (std::vector<uint32_t> const &token) { return ischs(token[0]); },
        make_flatten_iterator(
            make_map_iterator(
                [](std::string const &s) { return token_iterator(s); },
                make_generator_adaptor(sequences.begin(), sequences.end())
            )
        )
    );

    // construct substring counter
    counter_.fit(tokens);

    auto m = counter_.raw_string_count();
    std::vector<seg_pos_list> prev_segs(m), segs(m);
    for (decltype(max_iters_) i = 0; i < max_iters_; ++i)
    {
        // segment sequences
        for (decltype(m) j = 0, p = 0; j < m; ++j)
        {
            auto n = counter_.raw_string_length(j);
            optimize_segment(segs[j], p, n);
            p += n + 1;
        }

        if (prev_segs == segs) { break; }

        // update substring counts
        for (decltype(m) j = 0, p = 0; j < m; ++j)
        {
            auto n = counter_.raw_string_length(j);
            if (i > 0)
            {
                counter_.unset_pres(prev_segs[j], p, n);
            }

            counter_.set_pres(segs[j], p, n);
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
            else if (!std::iswspace(ch))
            {
                words.emplace_back(begin, end);
            }

            begin = end;
        }

        words_list.push_back(words);
    }

    return words_list;
}

void segmenter::optimize_segment(seg_pos_list &seg, size_t p, size_t n) const
{
    if (n == 0) { return; }

    std::vector<size_t> fs(n);
    std::vector<double> fv(n);
    for (decltype(n) i = 0; i < n; ++i)
    {
        fs[i] = 0;
        fv[i] = counter_.score(p, i + 1);
        for (decltype(i) j = 0; j < i; ++j)
        {
            if (fv[j] == -std::numeric_limits<double>::infinity()) { continue; }

            auto cv = fv[j] + counter_.score(p + j + 1, i - j);
            if (cv > fv[i])
            {
                fv[i] = cv;
                fs[i] = j + 1;
            }
        }
    }

    seg.clear();
    for (auto i = fs[n - 1]; i > 0; i = fs[i - 1])
    {
        seg.push_back(i);
    }

    std::reverse(seg.begin(), seg.end());
}

void segmenter::segment_sequence(std::vector<std::string> &words,
                                 std::string::const_iterator begin,
                                 std::string::const_iterator end,
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
