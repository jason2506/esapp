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
    typedef decltype(fit_and_segment(sequences)) vec_type;
    typedef std::vector<std::wstring> words_type;
    return fit_and_segment(
        sequences,
        [this](std::wstring const &s) { return tokenize_iterator(s); },
        [](vec_type &v, words_type const &e) { v.push_back(e); });
}

std::vector<std::vector<std::string>> segmenter::fit_and_segment(
    std::vector<std::string> const &sequences)
{
    typedef decltype(fit_and_segment(sequences)) vec_type;
    typedef std::vector<std::wstring> words_type;
    std::wstring ws;
    return fit_and_segment(
        sequences,
        [this, &ws](std::string const &s) { ws = s2ws(s);
                                            return tokenize_iterator(ws); },
        [](vec_type &v, words_type const &e) { v.push_back(ws2s(e)); });
}

template <typename T, typename F, typename G>
std::vector<std::vector<T>> segmenter::fit_and_segment(
    std::vector<T> const &sequences, F const &f, G const &g)
{
    // pre-segment sequences by alphabets, numbers, and symbols
    encoded_multistring s;
    for (auto const &sequence : sequences)
    {
        auto git = f(sequence);
        for (auto const &token : git)
        {
            if (!ischs(token[0])) { continue; }

            s.append(token);
        }
    }

    // construct substring counter
    counter_.fit(std::move(s));

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

    // generate segmented word lists
    auto it = prev_segs.begin();
    decltype(fit_and_segment(sequences, f, g)) words_list;
    words_list.reserve(sequences.size());
    for (auto const &sequence : sequences)
    {
        auto git = f(sequence);
        std::vector<typename decltype(git)::value_type> words;
        for (auto const &token : git)
        {
            if (ischs(token[0]))    { segment_sequence(words, token, *it++); }
            else                    { words.push_back(token); }
        }

        g(words_list, words);
    }

    return words_list;
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
