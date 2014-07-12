/************************************************
 *  count.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_COUNT_HPP_
#define ESAPP_COUNT_HPP_

#include <cmath>
#include <algorithm>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>

#include "sufarr.hpp"
#include "trie.hpp"

namespace esapp
{

/************************************************
 * Declaration: class string_counter
 ************************************************/

class string_counter
{
public: // Public Type(s)
    typedef suffix_array::value_type index_type;
    typedef wchar_t term_type;
    typedef std::basic_string<term_type> sequence;

public: // Public Method(s)
    string_counter(double lrv_exp, size_t max_len = 30, double smooth = 0.0);

    void fit(encoded_multistring const &s);

    void set_pres(std::vector<index_type> pres, size_t p, size_t n);
    void unset_pres(std::vector<index_type> pres, size_t p, size_t n);

    double score(size_t i, size_t n) const;

    size_t raw_string_count(void) const;
    size_t raw_string_length(size_t i) const;

    void clear(void);

private: // Private Type(s)
    typedef suffix_array::term_type term_id;
    typedef std::vector<term_id> id_sequence;
    typedef std::unordered_map<term_id, index_type> term_counts;

private: // Private Method(s)
    void calc_avg(void);
    double entropy(term_counts const &counts) const;

private: // Private Property(ies)
    double lrv_exp_;
    index_type max_len_;
    double smooth_;
    double h1_;

    std::vector<double> f_avgs_;
    std::vector<double> hl_avgs_;
    std::vector<double> hr_avgs_;
    std::vector<index_type> str_nums_;

    std::vector<index_type> count_min_lens_;

    freq_trie trie_;
    suffix_array sa_;

    static const term_id BOUNDARY_;
}; // class string_counter

/************************************************
 * Implementation: class string_counter
 ************************************************/

inline size_t string_counter::raw_string_count(void) const
{
    return sa_.data().string_count();
}

inline size_t string_counter::raw_string_length(size_t i) const
{
    return sa_.data().size(i);
}

inline void string_counter::clear(void)
{
    sa_.clear();
}

} // namespace esapp

#endif // ESAPP_COUNT_HPP_
