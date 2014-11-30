/************************************************
 *  string_counter.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_STRING_COUNTER_HPP_
#define ESAPP_STRING_COUNTER_HPP_

#include <cmath>
#include <algorithm>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>

#include "suffix_array.hpp"
#include "freq_trie.hpp"

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

public: // Public Method(s)
    string_counter(double lrv_exp, size_t max_len = 30, double smooth = 0.0);

    template <typename Generator>
    void fit(Generator const &g);

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

template <typename Generator>
inline void string_counter::fit(Generator const &g)
{
    // calculate average statistics of substrings
    sa_.construct(g);
    calc_avg();

    // initialize vector of preserve lengths
    count_min_lens_.resize(sa_.size());
    std::fill(count_min_lens_.begin(), count_min_lens_.end(), 0);
}

inline size_t string_counter::raw_string_count(void) const
{
    return sa_.string_count();
}

inline size_t string_counter::raw_string_length(size_t i) const
{
    return sa_.size(i);
}

inline void string_counter::clear(void)
{
    sa_.clear();
}

} // namespace esapp

#endif // ESAPP_STRING_COUNTER_HPP_
