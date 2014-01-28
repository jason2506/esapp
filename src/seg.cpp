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

Segmenter::Segmenter(double lrv_exp, size_t max_iters, size_t max_length, double smooth)
    : trie_(max_length, smooth), lrv_exp_(lrv_exp), max_iters_(max_iters)
{
    // do nothing
}

std::vector<std::vector<Segmenter::Sequence> >
Segmenter::fit_and_segment(const std::vector<Sequence> &sequences)
{
    fit(sequences);
    return segment(sequences);
}

void Segmenter::fit(const std::vector<Sequence> &sequences)
{
    trie_.clear();
    trie_.increase(sequences);
    trie_.update_hsp1();
    trie_.update_fm();
    trie_.update_iv();

    size_t length = sequences.size();
    std::vector<Seg> prev_segs, segs;
    for (size_t i = 0; i < max_iters_; ++i)
    {
        segs.clear();
        if (!prev_segs.empty())
        {
            for (size_t j = 0; j < length; ++j)
            {
                std::vector<Sequence> words = segment_sequence(sequences[j], prev_segs[j]);
                trie_.increase(words, false);
            }
        }

        for (size_t j = 0; j < length; ++j)
        {
            Seg seg = optimize_segment(sequences[j]);
            std::vector<Sequence> words = segment_sequence(sequences[j], seg);
            trie_.decrease(words, false);
            segs.push_back(seg);
        }

        if (prev_segs == segs) { break; }

        prev_segs.swap(segs);
        trie_.update_iv();
    }
}

std::vector<std::vector<Segmenter::Sequence> >
Segmenter::segment(const std::vector<Sequence> &sequences) const
{
    std::vector<std::vector<Segmenter::Sequence> > results;
    for (std::vector<Sequence>::const_iterator it = sequences.begin();
         it != sequences.end(); ++it)
    {
        results.push_back(segment(*it));
    }

    return results;
}

std::vector<Segmenter::Sequence> Segmenter::segment(const Sequence &sequence) const
{
    return segment_sequence(sequence, optimize_segment(sequence));
}

Segmenter::Seg Segmenter::optimize_segment(const Sequence &sequence) const
{
    size_t n = sequence.size();
    if (n == 0) { return Seg(); }

    size_t m = n * (1 + n) / 2;
    size_t **fs = new size_t*[n];
    double **fv = new double*[n];
    fs[0] = new size_t[m];
    fv[0] = new double[m];
    for (size_t i = 1, offset = n; i < n; ++i, --offset)
    {
        fs[i] = fs[i - 1] + offset;
        fv[i] = fv[i - 1] + offset;
    }

    Sequence::const_iterator it = sequence.begin();
    for (size_t j = 0; j < n; ++j)
    {
        for (size_t i = 0; i + j < n; ++i)
        {
            Sequence::const_iterator begin = it + i;
            Sequence::const_iterator end = it + i + j + 1;
            fv[i][j] = trie_.get_iv(begin, end);
            fs[i][j] = 0;
            for (size_t k = 1; k <= j; ++k)
            {
                double hr = trie_.get_hr(begin, begin + k);
                double hl = trie_.get_hl(begin + k, end);
                double lrv = pow(hr * hl, lrv_exp_);
                double cv = fv[i][k - 1] * fv[i + k][j - k] * lrv;
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

void Segmenter::generate_segment(Seg &seg, size_t **fs, size_t i, size_t j) const
{
    if (fs[i][j] == 0) { return; }

    size_t k = fs[i][j];
    generate_segment(seg, fs, i, k - 1);
    seg.push_back(i + k);
    generate_segment(seg, fs, i + k, j - k);
}

std::vector<Segmenter::Sequence> Segmenter::segment_sequence(
    const Sequence &sequence, const Seg &seg) const
{
    std::vector<Sequence> words;
    if (sequence.empty()) { return words; }

    Seg::value_type start = 0;
    for (Seg::const_iterator it = seg.begin();
         it != seg.end(); start = *it, ++it)
    {
        Sequence word = sequence.substr(start, *it - start);
        words.push_back(word);
    }

    Sequence word = sequence.substr(start);
    words.push_back(word);

    return words;
}

} // namespace esa
