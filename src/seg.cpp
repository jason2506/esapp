/************************************************
 *  seg.cpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include "seg.hpp"

namespace esapp
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

std::vector<std::vector<std::wstring>> segmenter::fit_and_segment(
    std::vector<std::wstring> const &sequences)
{
    // pre-segment sequences by alphabets, numbers, and symbols
    encoded_multistring s;
    for (auto const &sequence : sequences)
    {
        tokenizer tok(sequence);
        while (tok.has_next())
        {
            auto token = tok.next();
            if (!ischs(token[0])) { continue; }

            s.append(token);
        }
    }

    // construct substring counter
    counter_.fit(std::move(s));
    auto segs = fit_and_segment();

    // generate segmented word lists
    auto it = segs.begin();
    decltype(fit_and_segment(sequences)) words_list;
    words_list.reserve(sequences.size());
    for (auto const &sequence : sequences)
    {
        decltype(words_list)::value_type words;
        tokenizer tok(sequence);
        while (tok.has_next())
        {
            auto token = tok.next();
            if (ischs(token[0]))    { segment_sequence(words, token, *it++); }
            else                    { words.push_back(token); }
        }

        words_list.push_back(words);
    }

    return words_list;
}

std::vector<std::vector<std::string>> segmenter::fit_and_segment(
    std::vector<std::string> const &sequences)
{
    // pre-segment sequences by alphabets, numbers, and symbols
    encoded_multistring s;
    for (auto const &sequence : sequences)
    {
        auto ws_sequence = s2ws(sequence);
        tokenizer tok(ws_sequence);
        while (tok.has_next())
        {
            auto token = tok.next();
            if (!ischs(token[0])) { continue; }

            s.append(token);
        }
    }

    // construct substring counter
    counter_.fit(std::move(s));
    auto segs = fit_and_segment();

    // generate segmented word lists
    auto it = segs.begin();
    decltype(fit_and_segment(sequences)) words_list;
    words_list.reserve(sequences.size());
    for (auto const &sequence : sequences)
    {
        std::vector<std::wstring> words;

        auto ws_sequence = s2ws(sequence);
        tokenizer tok(ws_sequence);
        while (tok.has_next())
        {
            auto token = tok.next();
            if (ischs(token[0]))    { segment_sequence(words, token, *it++); }
            else                    { words.push_back(token); }
        }

        words_list.push_back(ws2s(words));
    }

    return words_list;
}

std::vector<segmenter::segment> segmenter::fit_and_segment(void)
{
    auto m = counter_.raw_string_count();
    std::vector<segment> prev_segs(m), segs(m);
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

    return prev_segs;
}

void segmenter::optimize_segment(segment &seg, size_t p, size_t n) const
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
            auto cv = fv[j] * counter_.score(p + j + 1, i - j);
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

void segmenter::segment_sequence(std::vector<std::wstring> &words,
                                 std::wstring const &sequence,
                                 segment const &seg) const
{
    segment::value_type start = 0;
    for (auto const &pos : seg)
    {
        auto word = sequence.substr(start, pos - start);
        words.push_back(word);
        start = pos;
    }

    auto word = sequence.substr(start);
    words.push_back(word);
}

std::vector<std::wstring> segmenter::segment_sequence(
    std::wstring const &sequence, segment const &seg) const
{
    decltype(segment_sequence(sequence, seg)) words;
    words.reserve(seg.size() + 1);
    segment_sequence(words, sequence, seg);
    return words;
}

} // namespace esapp
