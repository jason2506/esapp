/************************************************
 *  flatten_iterator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_FLATTEN_ITERATOR_HPP_
#define ESAPP_FLATTEN_ITERATOR_HPP_

#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

#include "nested_generator.hpp"

namespace esapp
{

template <typename G> class flatten_iterator;

/************************************************
 * Inline Helper Function(s)
 ************************************************/

template <typename Generator>
inline flatten_iterator<Generator>
make_flatten_iterator(Generator const &generator)
{
    typedef decltype(make_flatten_iterator(generator)) it_t;
    return it_t(generator);
}

/************************************************
 * Declaration: class flatten_iterator
 ************************************************/

template <typename Generator>
class flatten_iterator : public nested_generator
    <
        flatten_iterator<Generator>,
        Generator,
        typename std::iterator_traits<
            typename std::iterator_traits<Generator>::value_type
        >::value_type
    >
{
private: // Private Type(s)
    typedef nested_generator
        <
            flatten_iterator<Generator>,
            Generator,
            typename std::iterator_traits<
                typename std::iterator_traits<Generator>::value_type
            >::value_type
        > base_t;

public: // Public Type(s)
    typedef typename base_t::iterator_category iterator_category;
    typedef typename base_t::value_type value_type;
    typedef typename base_t::reference reference;
    typedef typename base_t::pointer pointer;
    typedef typename base_t::difference_type difference_type;

    typedef typename base_t::inner_generator inner_generator;
    typedef typename std::iterator_traits<Generator>::value_type value_generator;

public: // Public Method(s)
    flatten_iterator(void) = default;
    explicit flatten_iterator(inner_generator const &g);

    void next(void);
    reference dereference(void) const;
    bool equal(flatten_iterator const &it) const;

private: // Private Property(ies)
    value_generator val_g_;
}; // class flatten_iterator

/************************************************
 * Implementation: class flatten_iterator
 ************************************************/

template <typename G>
inline flatten_iterator<G>::flatten_iterator(inner_generator const &g)
    : base_t(g)
{
    if (base_t::valid()) { val_g_ = *base_t::base(); }
}

template <typename G>
inline void flatten_iterator<G>::next(void)
{
    if (!(++val_g_))
    {
        base_t::next();
        if (base_t::valid()) { val_g_ = *base_t::base(); }
    }
}

template <typename G>
inline typename flatten_iterator<G>::reference
flatten_iterator<G>::dereference(void) const
{
    return *val_g_;
}

template <typename G>
inline bool flatten_iterator<G>::equal(flatten_iterator const &it) const
{
    return base_t::equal(it) &&
        base_t::valid() == it.valid() &&
        (!base_t::valid() || val_g_ == it.val_g_);
}

} // namespace esapp

#endif // ESAPP_FLATTEN_ITERATOR_HPP_
