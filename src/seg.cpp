#include "seg.hpp"

namespace esa
{

/************************************************
 * Implementation: class Segmenter
 ************************************************/

Segmenter::Segmenter(double lrv_exp, size_t max_iters, size_t max_length, double smooth)
    : trie_(max_length, smooth), lrv_exp_(lrv_exp), max_iters_(max_iters)
{
    // do nothing
}

void Segmenter::fit(const std::vector<std::wstring> &sequences)
{
    trie_.clear();
    trie_.increase(sequences);
    trie_.update_hsp1();
    trie_.update_fm();
    trie_.update_iv();

    std::vector<std::vector<size_t> > prev_segs, segs;
    for (size_t i = 0; i < max_iters_; ++i)
    {
        segs.clear();
        size_t length = sequences.size();
        if (!prev_segs.empty())
        {
            for (size_t j = 0; j < length; ++j)
            {
                std::vector<std::wstring> words = segment(sequences[j], prev_segs[j]);
                trie_.increase(words, false);
            }
        }

        for (size_t j = 0; j < length; ++j)
        {
            SegResult result = segment(sequences[j].begin(), sequences[j].end());
            const std::vector<size_t> &seg = result.second;

            std::vector<std::wstring> words = segment(sequences[j], seg);
            trie_.decrease(words, false);

            segs.push_back(seg);
        }

        if (prev_segs == segs) { break; }

        prev_segs.swap(segs);
        trie_.update_iv();
    }
}

std::vector<std::wstring> Segmenter::segment(const std::wstring &sequence) const
{
    SegResult result = segment(sequence.begin(), sequence.end());
    return segment(sequence, result.second);
}

std::vector<std::wstring> Segmenter::segment(const std::wstring &sequence,
                                             const std::vector<size_t> &seg) const
{
    std::vector<std::wstring> words;

    size_t start = 0;
    for (std::vector<size_t>::const_iterator it = seg.begin();
         it != seg.end(); start += *it, ++it)
    {
        std::wstring word = sequence.substr(start, *it);
        words.push_back(word);
    }

    std::wstring word = sequence.substr(start);
    words.push_back(word);

    return words;
}

Segmenter::SegResult Segmenter::segment(const std::wstring::const_iterator &begin,
                                        const std::wstring::const_iterator &end) const
{
    // TODO: need to be rewritten
    std::vector<size_t> fs;
    double fv = trie_.get_iv(begin, end);
    size_t seg = 1;
    for (std::wstring::const_iterator it = begin + 1; it != end; ++it, ++seg)
    {
        SegResult left = segment(begin, it);
        SegResult right = segment(it, end);
        double hr = trie_.get_hr(begin, it);
        double hl = trie_.get_hl(it, end);
        double lrv = pow(hr * hl, lrv_exp_);
        double cv = left.first * right.first * lrv;
        if (cv > fv)
        {
            size_t offset = 0;
            for (std::vector<size_t>::const_iterator it = left.second.begin();
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
