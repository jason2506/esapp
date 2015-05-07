/************************************************
 *  suffix_array.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2015, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_SUFFIX_ARRAY_HPP_
#define ESAPP_SUFFIX_ARRAY_HPP_

#include <cstdlib>
#include <limits>
#include <algorithm>
#include <type_traits>
#include <vector>

#include "encoded_multistring.hpp"

namespace esapp
{

namespace impl
{

/************************************************
 * Declaration: class suffix_array
 ************************************************/

class suffix_array
{
private: // Private Type(s)
    typedef encoded_multistring sequence;

public: // Public Type(s)
    typedef size_t value_type;
    typedef sequence::value_type term_type;

    typedef typename ::std::vector<term_type>::const_iterator data_iterator;
    typedef typename ::std::vector<value_type>::const_iterator iterator;
    typedef typename ::std::vector<value_type>::const_iterator const_iterator;

public: // Public Method(s)
    suffix_array(void) = default;
    template <typename Generator>
    explicit suffix_array(Generator const &g);

    template <typename Generator>
    void construct(Generator const &g);
    void clear(void);

    size_t size(void) const;
    size_t size(size_t i) const;
    bool empty(void) const;

    term_type term(size_t i) const;
    value_type rank(size_t i) const;
    value_type lcp(size_t i) const;
    value_type at(size_t i) const;
    value_type operator[](size_t i) const;

    size_t alphabet_count(void) const;
    size_t string_count(void) const;

    data_iterator data_begin(void) const;
    data_iterator data_end(void) const;

    const_iterator begin(void) const;
    const_iterator end(void) const;

private: // Private Method(s)
    void construct(void);
    void construct(size_t num_alphas);

    bool is_lms(::std::vector<bool> const &suf_types, size_t i, size_t len_s);

    template <typename T>
    void init_bkt(T const &s, ::std::vector<value_type> &bkt,
                  size_t num_chars, size_t num_alphas, bool end);

    template <typename T>
    void induce(T const &s, ::std::vector<bool> const &suf_types,
                ::std::vector<value_type> &sa, ::std::vector<value_type> &bkt,
                size_t num_chars, size_t num_alphas);

    template <typename T>
    void gen_sa(T const &s, ::std::vector<value_type> &sa,
                ::std::vector<value_type> &bkt,
                size_t num_chars, size_t num_alphas);

    void gen_isa(void);
    void gen_lcpa(void);

private: // Private Property(ies)
    sequence s_;
    ::std::vector<value_type> sa_;
    ::std::vector<value_type> isa_;
    ::std::vector<value_type> lcpa_;
}; // class suffix_array

/************************************************
 * Implementation: class suffix_array
 ************************************************/

template <typename Generator>
inline suffix_array::suffix_array(Generator const &g)
    : s_(g)
{
    construct();
}

template <typename Generator>
inline void suffix_array::construct(Generator const &g)
{
    for (auto it = g; it; ++it)
    {
        s_.push_back(*it);
    }

    construct();
}

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

inline size_t suffix_array::size(size_t i) const
{
    return s_.size(i);
}

inline bool suffix_array::empty(void) const
{
    return sa_.empty();
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

inline size_t suffix_array::alphabet_count(void) const
{
    return s_.alphabet_count();
}

inline size_t suffix_array::string_count(void) const
{
    return s_.string_count();
}

inline suffix_array::data_iterator suffix_array::data_begin(void) const
{
    return s_.begin();
}

inline suffix_array::data_iterator suffix_array::data_end(void) const
{
    return s_.end();
}

inline suffix_array::const_iterator suffix_array::begin(void) const
{
    return sa_.begin();
}

inline suffix_array::const_iterator suffix_array::end(void) const
{
    return sa_.end();
}

} // namespace impl

} // namespace esapp

#endif // ESAPP_SUFFIX_ARRAY_HPP_
