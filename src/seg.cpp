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
 * Implementation: class Segmenter
 ************************************************/

Segmenter::Segmenter(double lrv_exp, size_t max_iters,
                     size_t max_length, double smooth)
    : trie_(max_length, smooth), lrv_exp_(lrv_exp), max_iters_(max_iters)
{
    // do nothing
}

std::vector<std::vector<std::string>>
Segmenter::fit_and_segment(std::vector<std::string> const &sequences)
{
    auto ws_sequences = vec_s2ws(sequences);
    auto ws_words_list = fit_and_segment(ws_sequences);

    decltype(fit_and_segment(sequences)) words_list(ws_words_list.size());
    std::transform(ws_words_list.begin(), ws_words_list.end(),
                   words_list.begin(), &vec_ws2s);

    return words_list;
}

std::vector<std::vector<std::wstring>>
Segmenter::fit_and_segment(std::vector<std::wstring> const &sequences)
{
    fit(sequences);
    return segment(sequences);
}

void Segmenter::fit(std::vector<std::string> const &sequences)
{
    auto ws_sequences = vec_s2ws(sequences);
    fit(ws_sequences);
}

void Segmenter::fit(std::vector<std::wstring> const &sequences)
{
    trie_.clear();
    trie_.increase(sequences);
    trie_.update_hsp1();
    trie_.update_fm();
    trie_.update_iv();

    auto length = sequences.size();
    std::vector<Seg> prev_segs, segs;
    for (decltype(max_iters_) i = 0; i < max_iters_; ++i)
    {
        segs.clear();
        if (!prev_segs.empty())
        {
            for (decltype(length) j = 0; j < length; ++j)
            {
                auto words = segment_sequence(sequences[j], prev_segs[j]);
                trie_.increase(words, false);
            }
        }

        for (decltype(length) j = 0; j < length; ++j)
        {
            auto seg = optimize_segment(sequences[j]);
            auto words = segment_sequence(sequences[j], seg);
            trie_.decrease(words, false);
            segs.push_back(seg);
        }

        if (prev_segs == segs) { break; }

        prev_segs.swap(segs);
        trie_.update_iv();
    }
}

std::vector<std::vector<std::string>>
Segmenter::segment(std::vector<std::string> const &sequences) const
{
    decltype(segment(sequences)) results;
    for (auto const &sequence : sequences)
    {
        results.push_back(segment(sequence));
    }

    return results;
}

std::vector<std::vector<std::wstring>>
Segmenter::segment(std::vector<std::wstring> const &sequences) const
{
    decltype(segment(sequences)) results;
    for (auto const &sequence : sequences)
    {
        results.push_back(segment(sequence));
    }

    return results;
}

std::vector<std::string> Segmenter::segment(std::string const &sequence) const
{
    auto ws_sequences = s2ws(sequence);
    auto ws_words = segment(ws_sequences);

    decltype(segment(sequence)) words(ws_words.size());
    std::transform(ws_words.begin(), ws_words.end(), words.begin(), &ws2s);

    return words;
}

std::vector<std::wstring> Segmenter::segment(std::wstring const &sequence) const
{
    return segment_sequence(sequence, optimize_segment(sequence));
}

Segmenter::Seg Segmenter::optimize_segment(std::wstring const &sequence) const
{
    auto n = sequence.size();
    if (n == 0) { return Seg(); }

    auto m = n * (1 + n) / 2;
    auto **fs = new size_t*[n];
    auto **fv = new double*[n];
    fs[0] = new size_t[m];
    fv[0] = new double[m];
    for (decltype(n) i = 1, offset = n; i < n; ++i, --offset)
    {
        fs[i] = fs[i - 1] + offset;
        fv[i] = fv[i - 1] + offset;
    }

    auto it = sequence.begin();
    for (decltype(n) j = 0; j < n; ++j)
    {
        for (decltype(n) i = 0; i + j < n; ++i)
        {
            auto begin = it + i;
            auto end = it + i + j + 1;
            fv[i][j] = trie_.get_iv(begin, end);
            fs[i][j] = 0;
            for (decltype(j) k = 1; k <= j; ++k)
            {
                auto hr = trie_.get_hr(begin, begin + k);
                auto hl = trie_.get_hl(begin + k, end);
                auto lrv = pow(hr * hl, lrv_exp_);
                auto cv = fv[i][k - 1] * fv[i + k][j - k] * lrv;
                if (cv > fv[i][j])
                {
                    fv[i][j] = cv;
                    fs[i][j] = k;
                }
            }
        }
    }

    Seg seg;
    generate_segment(seg, fs, 0, n - 1);

    delete [] fs[0];
    delete [] fv[0];
    delete [] fs;
    delete [] fv;

    return seg;
}

void Segmenter::generate_segment(Seg &seg, size_t **fs,
                                 size_t i, size_t j) const
{
    if (fs[i][j] == 0) { return; }

    auto k = fs[i][j];
    generate_segment(seg, fs, i, k - 1);
    seg.push_back(i + k);
    generate_segment(seg, fs, i + k, j - k);
}

std::vector<std::wstring> Segmenter::segment_sequence(
    std::wstring const &sequence, Seg const &seg) const
{
    typedef decltype(segment_sequence(sequence, seg)) Words;
    Words words;
    if (sequence.empty()) { return words; }

    Seg::value_type start = 0;
    for (auto const &pos : seg)
    {
        auto word = sequence.substr(start, pos - start);
        words.push_back(word);
        start = pos;
    }

    auto word = sequence.substr(start);
    words.push_back(word);

    return words;
}

} // namespace esa
