/************************************************
 *  seg.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_SEG_HPP_
#define ESAPP_SEG_HPP_

#include <algorithm>
#include <string>
#include <vector>
#include <cmath>

#include "tok.hpp"
#include "trie.hpp"
#include "conv.hpp"

namespace esapp
{

/************************************************
 * Declaration: class Segmenter
 ************************************************/

class Segmenter
{
public: // Public Method(s)
    Segmenter(double lrv_exp, size_t max_iters = 10,
              size_t max_length = 30, double smooth = 0.0);

    void fit(std::vector<std::wstring> const &sequences);
    void fit(std::vector<std::string> const &sequences);

    std::vector<std::wstring> segment(std::wstring const &sequence) const;
    std::vector<std::string> segment(std::string const &sequence) const;

    std::vector<std::vector<std::wstring>> segment(
        std::vector<std::wstring> const &sequences) const;
    std::vector<std::vector<std::string>> segment(
        std::vector<std::string> const &sequences) const;

    std::vector<std::vector<std::wstring>> fit_and_segment(
        std::vector<std::wstring> const &sequences);
    std::vector<std::vector<std::string>> fit_and_segment(
        std::vector<std::string> const &sequences);

private: // Private Type(s)
    typedef std::vector<size_t> Seg;

private: // Private Method(s)
    Seg optimize_segment(std::wstring const &sequence) const;
    void generate_segment(Seg &seg, size_t **fs, size_t i, size_t j) const;
    void segment_sequence(std::vector<std::wstring> &words,
                          std::wstring const &sequence, Seg const &seg) const;
    std::vector<std::wstring> segment_sequence(std::wstring const &sequence,
                                               Seg const &seg) const;

private: // Private Property(ies)
    FreqTrie trie_;
    double lrv_exp_;
    size_t max_iters_;
}; // class Segmenter

} // namespace esa

#endif // ESAPP_SEG_HPP_
