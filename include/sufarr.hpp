/************************************************
 *  sufarr.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_SUFARR_HPP_
#define ESAPP_SUFARR_HPP_

#include <cstdlib>
#include <limits>
#include <algorithm>
#include <type_traits>
#include <vector>

#include "encstr.hpp"

namespace esapp
{

/************************************************
 * Declaration: class suffix_array
 ************************************************/

class suffix_array
{
public: // Public Type(s) - Part 1
    typedef size_t value_type;
    typedef encoded_multistring sequence;
    typedef encoded_multistring::value_type term_type;

    typedef typename std::vector<value_type>::const_iterator iterator;
    typedef typename std::vector<value_type>::const_iterator const_iterator;

public: // Public Method(s)
    suffix_array(void);
    suffix_array(sequence const &s);

    void construct(sequence const &s);
    void clear(void);

    size_t size(void) const;
    bool empty(void) const;

    sequence const &data(void) const;
    term_type term(size_t i) const;
    value_type rank(size_t i) const;
    value_type lcp(size_t i) const;
    value_type at(size_t i) const;
    value_type operator[](size_t i) const;

    const_iterator begin(void) const;
    const_iterator end(void) const;

private: // Private Method(s)
    void construct(void);
    void construct(size_t num_alphas);

    bool is_lms(std::vector<bool> const &suf_types, size_t i, size_t len_s);

    template <typename T>
    void init_bkt(T const &s, std::vector<value_type> &bkt,
                  size_t num_chars, size_t num_alphas, bool end);

    template <typename T>
    void induce(T const &s, std::vector<bool> const &suf_types,
                std::vector<value_type> &sa, std::vector<value_type> &bkt,
                size_t num_chars, size_t num_alphas);

    template <typename T>
    void gen_sa(T const &s, std::vector<value_type> &sa,
                std::vector<value_type> &bkt,
                size_t num_chars, size_t num_alphas);

    void gen_isa(void);
    void gen_lcpa(void);

private: // Private Property(ies)
    sequence s_;
    std::vector<value_type> sa_;
    std::vector<value_type> isa_;
    std::vector<value_type> lcpa_;
}; // class suffix_array

/************************************************
 * Implementation: class suffix_array
 ************************************************/

inline void suffix_array::clear(void)
{
    s_.clear();
    sa_.clear();
    isa_.clear();
    lcpa_.clear();
}

inline size_t suffix_array::size(void) const
{
    return sa_.size();
}

inline bool suffix_array::empty(void) const
{
    return sa_.empty();
}

inline suffix_array::sequence const &suffix_array::data(void) const
{
    return s_;
}

inline suffix_array::term_type suffix_array::term(size_t i) const
{
    return s_[i];
}

inline suffix_array::value_type suffix_array::rank(size_t i) const
{
    return isa_[i];
}

inline suffix_array::value_type suffix_array::lcp(size_t i) const
{
    return lcpa_[i];
}

inline suffix_array::value_type suffix_array::at(size_t i) const
{
    return sa_[i];
}

inline suffix_array::value_type suffix_array::operator[](size_t i) const
{
    return sa_[i];
}

inline suffix_array::const_iterator suffix_array::begin(void) const
{
    return sa_.begin();
}

inline suffix_array::const_iterator suffix_array::end(void) const
{
    return sa_.end();
}

} // namespace esapp

#endif // ESAPP_SUFARR_HPP_
