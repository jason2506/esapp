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
    : counter_(max_length, smooth), lrv_exp_(lrv_exp), max_iters_(max_iters)
{
    if (lrv_exp_ < 0)
    {
        throw std::invalid_argument("The exponent parameter of LRV must be " \
                                    "greater than or equal to 0.");
    }
}

std::vector<std::vector<std::wstring>> Segmenter::fit_and_segment(
    std::vector<std::wstring> const &sequences)
{
    // pre-segment sequences by alphabets, numbers, and symbols
    std::vector<std::wstring> tokens;
    for (auto const &sequence : sequences)
    {
        Tokenizer tokenizer(sequence);
        while (tokenizer.has_next())
        {
            auto token = tokenizer.next();
            if (!ischs(token[0])) { continue; }

            tokens.push_back(token);
        }
    }

    // construct substring counter
    counter_.fit(tokens);

    auto m = tokens.size();
    std::vector<Seg> prev_segs(m), segs(m);
    for (decltype(max_iters_) i = 0; i < max_iters_; ++i)
    {
        // segment sequences
        for (decltype(m) j = 0, p = 0; j < m; ++j)
        {
            auto n = tokens[j].size();
            segs[j] = optimize_segment(p, tokens[j].size());
            p += n + 1;
        }

        if (prev_segs == segs) { break; }

        // update substring counts
        for (decltype(m) j = 0, p = 0; j < m; ++j)
        {
            auto n = tokens[j].size();
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
        decltype(words_list)::value_type words;
        Tokenizer tokenizer(sequence);
        while (tokenizer.has_next())
        {
            auto token = tokenizer.next();
            if (ischs(token[0]))    { segment_sequence(words, token, *it++); }
            else                    { words.push_back(token); }
        }

        words_list.push_back(words);
    }

    return words_list;
}

std::vector<std::vector<std::string>> Segmenter::fit_and_segment(
    std::vector<std::string> const &sequences)
{
    auto ws_sequences = s2ws(sequences);
    auto ws_words_list = fit_and_segment(ws_sequences);

    decltype(fit_and_segment(sequences)) words_list;
    words_list.reserve(ws_words_list.size());
    for (auto const &ws_words : ws_words_list)
    {
        words_list.push_back(ws2s(ws_words));
    }

    return words_list;
}

Segmenter::Seg Segmenter::optimize_segment(size_t p, size_t n) const
{
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

    for (decltype(n) j = 0; j < n; ++j)
    {
        for (decltype(n) i = 0; i + j < n; ++i)
        {
            fv[i][j] = counter_.get_iv(p + i, j + 1);
            fs[i][j] = 0;
            for (decltype(j) k = 1; k <= j; ++k)
            {
                // find right most boundary before position k
                auto x = 0;
                while (fs[i + x][k - x - 1] != 0)
                {
                    x += fs[i + x][k - x - 1];
                }

                // find left most boundary after position k
                auto y = j - k + 1;
                while (fs[i + k][y - 1] != 0)
                {
                    y = fs[i + k][y - 1];
                }

                // calculate combined goodness value for position k
                auto hr = counter_.get_hr(p + i + x, k - x);
                auto hl = counter_.get_hl(p + i + k, y);
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

void Segmenter::segment_sequence(std::vector<std::wstring> &words,
                                 std::wstring const &sequence,
                                 Seg const &seg) const
{
    if (sequence.empty()) { return; }

    Seg::value_type start = 0;
    for (auto const &pos : seg)
    {
        auto word = sequence.substr(start, pos - start);
        words.push_back(word);
        start = pos;
    }

    auto word = sequence.substr(start);
    words.push_back(word);
}

std::vector<std::wstring> Segmenter::segment_sequence(
    std::wstring const &sequence, Seg const &seg) const
{
    decltype(segment_sequence(sequence, seg)) words;
    words.reserve(seg.size() + 1);
    segment_sequence(words, sequence, seg);
    return words;
}

} // namespace esapp
