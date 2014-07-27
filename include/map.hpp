/************************************************
 *  map.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_MAP_HPP_
#define ESAPP_MAP_HPP_

#include <type_traits>

#include "gen.hpp"

namespace esapp
{

template <typename T, typename I> class map_iterator;

/************************************************
 * Inline Helper Function(s)
 ************************************************/

template <typename Transform, typename Iterable>
inline auto make_map_iterator(Transform const &transform,
                              Iterable const &iterable)
    -> map_iterator<Transform, decltype(iterable.begin())>
{
    typedef decltype(make_map_iterator(transform, iterable)) it_t;
    return it_t(transform, iterable.begin(), iterable.end());
}

/************************************************
 * Declaration: class map_iterator
 ************************************************/

template <typename Transform, typename Iterator>
class map_iterator
    : public generator<
        map_iterator<Transform, Iterator>,
        Iterator,
        typename std::result_of<
            Transform(decltype(*std::declval<Iterator>()))
        >::type
    >
{
private: // Private Type(s)
    typedef generator<
        map_iterator,
        Iterator,
        typename std::result_of<
            Transform(decltype(*std::declval<Iterator>()))
        >::type
    > supercls_t;

public: // Public Type(s)
    typedef typename supercls_t::iterator_category iterator_category;
    typedef typename supercls_t::value_type value_type;
    typedef typename supercls_t::reference reference;
    typedef typename supercls_t::pointer pointer;
    typedef typename supercls_t::difference_type difference_type;
    typedef typename supercls_t::input_iterator input_iterator;

    typedef Transform transform;

public: // Public Method(s)
    map_iterator(transform const &trans,
                 input_iterator const &begin,
                 input_iterator const &end = input_iterator());
    map_iterator(map_iterator const &it) = default;

    map_iterator end(void) const;
    void next(void);
    reference dereference(void) const;

private: // Private Property(ies)
    transform trans_;
    value_type val_;
}; // class map_iterator

/************************************************
 * Implementation: class map_iterator
 ************************************************/

template <typename T, typename I>
inline map_iterator<T, I>::map_iterator(transform const &trans,
                                        input_iterator const &begin,
                                        input_iterator const &end)
    : supercls_t(begin, end), trans_(trans), val_(trans_(*(this->it_)))
{
    // do nothing
}

template <typename T, typename I>
inline map_iterator<T, I> map_iterator<T, I>::end(void) const
{
    return map_iterator(trans_, this->end_, this->end_);
}

template <typename T, typename I>
inline void map_iterator<T, I>::next(void)
{
    val_ = trans_(*(++(this->it_)));
}

template <typename T, typename I>
inline typename map_iterator<T, I>::reference
map_iterator<T, I>::dereference(void) const
{
    return val_;
}

} // namespace esapp

#endif // ESAPP_MAP_HPP_
