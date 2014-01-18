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

    std::vector<Seg> prev_segs, segs;
    for (size_t i = 0; i < max_iters_; ++i)
    {
        segs.clear();
        size_t length = sequences.size();
        if (!prev_segs.empty())
        {
            for (size_t j = 0; j < length; ++j)
            {
                std::vector<Sequence> words = segment(sequences[j], prev_segs[j]);
                trie_.increase(words, false);
            }
        }

        for (size_t j = 0; j < length; ++j)
        {
            SegResult result = segment(sequences[j].begin(), sequences[j].end());
            const Seg &seg = result.second;

            std::vector<Sequence> words = segment(sequences[j], seg);
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
        SegResult result = segment(it->begin(), it->end());
        results.push_back(segment(*it, result.second));
    }

    return results;
}

std::vector<Segmenter::Sequence> Segmenter::segment(const Sequence &sequence) const
{
    SegResult result = segment(sequence.begin(), sequence.end());
    return segment(sequence, result.second);
}

std::vector<Segmenter::Sequence> Segmenter::segment(const Sequence &sequence,
                                                    const Seg &seg) const
{
    std::vector<Sequence> words;

    Seg::value_type start = 0;
    for (Seg::const_iterator it = seg.begin();
         it != seg.end(); start += *it, ++it)
    {
        Sequence word = sequence.substr(start, *it);
        words.push_back(word);
    }

    Sequence word = sequence.substr(start);
    words.push_back(word);

    return words;
}

Segmenter::SegResult Segmenter::segment(const Sequence::const_iterator &begin,
                                        const Sequence::const_iterator &end) const
{
    // TODO: need to be rewritten
    Seg fs;
    double fv = trie_.get_iv(begin, end);
    Seg::value_type seg = 1;
    for (Sequence::const_iterator it = begin + 1; it != end; ++it, ++seg)
    {
        SegResult left = segment(begin, it);
        SegResult right = segment(it, end);
        double hr = trie_.get_hr(begin, it);
        double hl = trie_.get_hl(it, end);
        double lrv = pow(hr * hl, lrv_exp_);
        double cv = left.first * right.first * lrv;
        if (cv > fv)
        {
            Seg::value_type offset = 0;
            for (Seg::const_iterator it = left.second.begin();
                 it != left.second.end(); ++it)
            {
                offset += *it;
            }

            fv = cv;
            fs.swap(left.second);
            fs.push_back(seg - offset);
            fs.insert(fs.end(), right.second.begin(), right.second.end());
        }
    }

    return make_pair(fv, fs);
}

} // namespace esa
