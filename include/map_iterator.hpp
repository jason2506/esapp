/************************************************
 *  map_iterator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_MAP_ITERATOR_HPP_
#define ESAPP_MAP_ITERATOR_HPP_

#include <iterator>
#include <type_traits>

#include "generator.hpp"

namespace esapp
{

template <typename T, typename I> class map_iterator;

/************************************************
 * Inline Helper Function(s)
 ************************************************/

template <typename Transform, typename Iterator>
inline auto make_map_iterator(Transform const &transform,
                              Iterator const &begin,
                              Iterator const &end)
    -> map_iterator<Transform, Iterator>
{
    typedef decltype(make_map_iterator(transform, begin, end)) it_t;
    return it_t(transform, begin, end);
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
            Transform(typename std::iterator_traits<Iterator>::value_type)
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
    > base_t;

public: // Public Type(s)
    typedef typename base_t::iterator_category iterator_category;
    typedef typename base_t::value_type value_type;
    typedef typename base_t::reference reference;
    typedef typename base_t::pointer pointer;
    typedef typename base_t::difference_type difference_type;
    typedef typename base_t::input_iterator input_iterator;

    typedef Transform transform;

public: // Public Method(s)
    map_iterator(void) = default;
    map_iterator(transform const &trans,
                 input_iterator const &begin,
                 input_iterator const &end = input_iterator());
    map_iterator(map_iterator const &it) = default;

    map_iterator end(void) const;
    void next(void);
    reference get(void) const;
    bool equal(map_iterator const &it) const;
    bool ended(void) const;

private: // Private Property(ies)
    transform trans_;
    value_type val_;
    bool has_next_;
}; // class map_iterator

/************************************************
 * Implementation: class map_iterator
 ************************************************/

template <typename T, typename I>
inline map_iterator<T, I>::map_iterator(transform const &trans,
                                        input_iterator const &begin,
                                        input_iterator const &end)
    : base_t(begin, end), trans_(trans)
{
    next();
}

template <typename T, typename I>
inline map_iterator<T, I> map_iterator<T, I>::end(void) const
{
    return map_iterator(trans_, this->end_, this->end_);
}

template <typename T, typename I>
inline void map_iterator<T, I>::next(void)
{
    has_next_ = this->it_ != this->end_;
    if (!has_next_) { return; }

    val_ = trans_(*(this->it_));
    this->it_++;
}

template <typename T, typename I>
inline typename map_iterator<T, I>::reference
map_iterator<T, I>::get(void) const
{
    return val_;
}

template <typename T, typename I>
inline bool map_iterator<T, I>::equal(map_iterator const &it) const
{
    return this->it_ == it.it_ && has_next_ == it.has_next_;
}

template <typename T, typename I>
inline bool map_iterator<T, I>::ended(void) const
{
    return !has_next_;
}

} // namespace esapp

#endif // ESAPP_MAP_ITERATOR_HPP_
