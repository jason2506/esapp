/************************************************
 *  segmenter.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_SEGMENTER_HPP_
#define ESAPP_SEGMENTER_HPP_

#include <cmath>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <desa/text_index.hpp>
#include <desa/with_lcp.hpp>

#include "token_iterator.hpp"
#include "map_iterator.hpp"
#include "filter_iterator.hpp"
#include "flatten_iterator.hpp"
#include "generator_adaptor.hpp"
#include "with_segments.hpp"

namespace esapp
{

namespace impl
{

/************************************************
 * Declaration: class segmenter
 ************************************************/

class segmenter
{
public: // Public Type(s)
    typedef ::std::size_t size_type;

public: // Public Method(s)
    explicit segmenter(double lrv_exp, size_type max_iters = 10);
    ::std::vector<::std::vector<::std::string>> fit_and_segment(
        ::std::vector<::std::string> const &sequences);

private: // Private Type(s)
    typedef ::desa::text_index
    <
        ::desa::with_lcp
        <
            with_segments<30>::policy
        >::policy
    > text_index;
    typedef text_index::term_type term_id;
    typedef token_iterator::value_type::value_type term_type;
    typedef ::std::vector<size_type> seg_pos_list;

private: // Private Property(ies)
    double lrv_exp_;
    size_type max_iters_;

    ::std::unordered_map<term_type, term_id> term_id_map_;
    text_index index_;

    static term_id const BOUNDARY_;
}; // class segmenter

} // namespace impl

} // namespace esapp

#endif // ESAPP_SEGMENTER_HPP_
