/************************************************
 *  segmenter.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_SEGMENTER_HPP_
#define ESAPP_SEGMENTER_HPP_

#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include "tokenize_iterator.hpp"
#include "map_iterator.hpp"
#include "filter_iterator.hpp"
#include "flatten_iterator.hpp"
#include "string_counter.hpp"
#include "conv.hpp"

namespace esapp
{

/************************************************
 * Declaration: class segmenter
 ************************************************/

class segmenter
{
public: // Public Method(s)
    segmenter(double lrv_exp, size_t max_iters = 10,
              size_t max_len = 30, double smooth = 0.0);

    std::vector<std::vector<std::wstring>> fit_and_segment(
        std::vector<std::wstring> const &sequences);
    std::vector<std::vector<std::string>> fit_and_segment(
        std::vector<std::string> const &sequences);

private: // Private Type(s)
    typedef string_counter::index_type index_type;
    typedef std::vector<index_type> segment;

private: // Private Method(s)
    template <typename T, typename Tokenize, typename Append>
    std::vector<std::vector<T>> fit_and_segment(
        std::vector<T> const &sequences,
        Tokenize const &tokenize, Append const &append);
    void optimize_segment(segment &seg, size_t p, size_t n) const;
    void segment_sequence(std::vector<std::wstring> &words,
                          std::wstring const &sequence,
                          segment const &seg) const;
    std::vector<std::wstring> segment_sequence(std::wstring const &sequence,
                                               segment const &seg) const;

private: // Private Property(ies)
    string_counter counter_;
    size_t max_iters_;
}; // class segmenter

} // namespace esapp

#endif // ESAPP_SEGMENTER_HPP_
