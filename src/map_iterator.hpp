/************************************************
 *  map_iterator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2015, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_MAP_ITERATOR_HPP_
#define ESAPP_MAP_ITERATOR_HPP_

#include <iterator>
#include <type_traits>

#include "nested_generator.hpp"

namespace esapp
{

template <typename T, typename G> class map_iterator;

/************************************************
 * Inline Helper Function(s)
 ************************************************/

template <typename Transform, typename Generator>
inline map_iterator<Transform, Generator>
make_map_iterator(Transform const &transform, Generator const &generator)
{
    typedef decltype(make_map_iterator(transform, generator)) it_t;
    return it_t(transform, generator);
}

/************************************************
 * Declaration: class map_iterator
 ************************************************/

template <typename Transform, typename Generator>
class map_iterator : public nested_generator
    <
        map_iterator<Transform, Generator>,
        Generator,
        typename std::result_of<
            const Transform(typename std::iterator_traits<Generator>::value_type)
        >::type
    >
{
private: // Private Type(s)
    typedef nested_generator
        <
            map_iterator,
            Generator,
            typename std::result_of<
                const Transform(typename std::iterator_traits<Generator>::value_type)
            >::type
        > base_t;

public: // Public Type(s)
    typedef typename base_t::iterator_category iterator_category;
    typedef typename base_t::value_type value_type;
    typedef typename base_t::reference reference;
    typedef typename base_t::pointer pointer;
    typedef typename base_t::difference_type difference_type;

    typedef typename base_t::inner_generator inner_generator;
    typedef Transform transform;

public: // Public Method(s)
    map_iterator(void) = default;
    map_iterator(transform const &trans, inner_generator const &g);

    void next(void);
    reference dereference(void) const;
    bool equal(map_iterator const &it) const;

private: // Private Property(ies)
    transform trans_;
    value_type val_;
}; // class map_iterator

/************************************************
 * Implementation: class map_iterator
 ************************************************/

template <typename T, typename G>
inline map_iterator<T, G>::map_iterator(transform const &trans,
                                        inner_generator const &g)
    : base_t(g), trans_(trans)
{
    if (base_t::valid()) { val_ = trans_(*base_t::base()); }
}

template <typename T, typename G>
inline void map_iterator<T, G>::next(void)
{
    base_t::next();
    if (base_t::valid()) { val_ = trans_(*base_t::base()); }
}

template <typename T, typename G>
inline typename map_iterator<T, G>::reference
map_iterator<T, G>::dereference(void) const
{
    return val_;
}

} // namespace esapp

#endif // ESAPP_MAP_ITERATOR_HPP_
