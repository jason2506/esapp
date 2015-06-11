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
#include <limits>
#include <stack>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include <iostream>

#include "token_iterator.hpp"
#include "map_iterator.hpp"
#include "filter_iterator.hpp"
#include "flatten_iterator.hpp"
#include "generator_adaptor.hpp"
#include "suffix_array.hpp"
#include "freq_trie.hpp"

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
    explicit segmenter(double lrv_exp, size_type max_iters = 10,
                       size_type max_len = 30, double smooth = 0.0);
    ::std::vector<::std::vector<::std::string>> fit_and_segment(
        ::std::vector<::std::string> const &sequences);

private: // Private Type(s)
    static_assert(::std::is_same<suffix_array::term_type,
                                 freq_trie::term_type>::value,
                  "suffix_array and freq_trie should have same term_type.");

    typedef suffix_array::term_type term_id;
    typedef token_iterator::value_type::value_type term_type;
    typedef ::std::unordered_map<term_id, size_type> term_counts;
    typedef ::std::vector<size_type> seg_pos_list;

private: // Private Method(s)
    void calc_avgs(void);
    double entropy(term_counts const &counts, size_type n) const;

    void set_pres(::std::vector<size_type> const &pres, size_type p, size_type n);
    void unset_pres(::std::vector<size_type> const &pres, size_type p, size_type n);

    void optimize_segment(seg_pos_list &seg, size_type p, size_type n) const;
    void segment_sequence(::std::vector<::std::string> &words,
                          ::std::string::const_iterator begin,
                          ::std::string::const_iterator end,
                          seg_pos_list const &seg) const;

private: // Private Property(ies)
    double lrv_exp_;
    size_type max_iters_;
    size_type max_len_;
    double smooth_;
    double h1_;

    ::std::vector<double> f_avgs_;
    ::std::vector<double> hl_avgs_;
    ::std::vector<double> hr_avgs_;
    ::std::vector<size_type> str_nums_;

    freq_trie trie_;
    suffix_array sa_;

    ::std::vector<size_type> count_min_lens_;

    static term_id const BOUNDARY_;
}; // class segmenter

} // namespace impl

} // namespace esapp

#endif // ESAPP_SEGMENTER_HPP_
