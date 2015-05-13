/************************************************
 *  filter_iterator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2015, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_FILTER_ITERATOR_HPP_
#define ESAPP_FILTER_ITERATOR_HPP_

#include <iterator>
#include <type_traits>

#include "nested_generator.hpp"

namespace esapp
{

template <typename P, typename G> class filter_iterator;

/************************************************
 * Inline Helper Function(s)
 ************************************************/

template <typename Predicate, typename Generator>
inline filter_iterator<Predicate, Generator>
make_filter_iterator(Predicate const &predicate, Generator const &generator)
{
    typedef decltype(make_filter_iterator(predicate, generator)) it_t;
    return it_t(predicate, generator);
}

/************************************************
 * Declaration: class filter_iterator
 ************************************************/

template <typename Predicate, typename Generator>
class filter_iterator : public nested_generator
    <
        filter_iterator<Predicate, Generator>,
        Generator
    >
{
private: // Private Type(s)
    typedef nested_generator
        <
            filter_iterator<Predicate, Generator>,
            Generator
        > base_t;

public: // Public Type(s)
    typedef typename base_t::iterator_category iterator_category;
    typedef typename base_t::value_type value_type;
    typedef typename base_t::reference reference;
    typedef typename base_t::pointer pointer;
    typedef typename base_t::difference_type difference_type;

    typedef typename base_t::inner_generator inner_generator;

    typedef Predicate predicate;

public: // Public Method(s)
    filter_iterator(void) = default;
    filter_iterator(predicate const &pred, inner_generator const &g);

    void find_next(void);
    void next(void);

private: // Private Property(ies)
    predicate pred_;
}; // class filter_iterator

/************************************************
 * Implementation: class filter_iterator
 ************************************************/

template <typename P, typename G>
inline filter_iterator<P, G>::filter_iterator(predicate const &pred,
                                              inner_generator const &g)
    : base_t(g), pred_(pred)
{
    find_next();
}

template <typename P, typename G>
inline void filter_iterator<P, G>::find_next(void)
{
    while (base_t::valid() && !pred_(*base_t::base())) { base_t::next(); }
}

template <typename P, typename G>
inline void filter_iterator<P, G>::next(void)
{
    base_t::next();
    find_next();
}

} // namespace esapp

#endif // ESAPP_FILTER_ITERATOR_HPP_
