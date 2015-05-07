/************************************************
 *  segmenter.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2015, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_SEGMENTER_HPP_
#define ESAPP_SEGMENTER_HPP_

#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include "token_iterator.hpp"
#include "map_iterator.hpp"
#include "filter_iterator.hpp"
#include "flatten_iterator.hpp"
#include "generator_adaptor.hpp"
#include "string_counter.hpp"

namespace esapp
{

namespace impl
{

/************************************************
 * Declaration: class segmenter
 ************************************************/

class segmenter
{
public: // Public Method(s)
    explicit segmenter(double lrv_exp, size_t max_iters = 10,
                       size_t max_len = 30, double smooth = 0.0);
    std::vector<std::vector<std::string>> fit_and_segment(
        std::vector<std::string> const &sequences);

private: // Private Type(s)
    typedef string_counter::index_type index_type;
    typedef std::vector<index_type> seg_pos_list;

private: // Private Method(s)
    void optimize_segment(seg_pos_list &seg, size_t p, size_t n) const;
    void segment_sequence(std::vector<std::string> &words,
                          std::string::const_iterator begin,
                          std::string::const_iterator end,
                          seg_pos_list const &seg) const;

private: // Private Property(ies)
    string_counter counter_;
    size_t max_iters_;
}; // class segmenter

} // namespace impl

} // namespace esapp

#endif // ESAPP_SEGMENTER_HPP_
