/************************************************
 *  filter_iterator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_FILTER_ITERATOR_HPP_
#define ESAPP_FILTER_ITERATOR_HPP_

#include <iterator>
#include <type_traits>

#include "generator.hpp"

namespace esapp
{

template <typename P, typename I> class filter_iterator;

/************************************************
 * Inline Helper Function(s)
 ************************************************/

template <typename Predicate, typename Iterator>
inline auto make_filter_iterator(Predicate const &predicate,
                                 Iterator const &begin,
                                 Iterator const &end)
    -> filter_iterator<Predicate, Iterator>
{
    typedef decltype(make_filter_iterator(predicate, begin, end)) it_t;
    return it_t(predicate, begin, end);
}

/************************************************
 * Declaration: class filter_iterator
 ************************************************/

template <typename Predicate, typename Iterator>
class filter_iterator
    : public generator<filter_iterator<Predicate, Iterator>, Iterator>
{
private: // Private Type(s)
    typedef generator<filter_iterator<Predicate, Iterator>, Iterator> base_t;

public: // Public Type(s)
    typedef typename base_t::iterator_category iterator_category;
    typedef typename base_t::value_type value_type;
    typedef typename base_t::reference reference;
    typedef typename base_t::pointer pointer;
    typedef typename base_t::difference_type difference_type;
    typedef typename base_t::input_iterator input_iterator;

    typedef Predicate predicate;

public: // Public Method(s)
    filter_iterator(void) = default;
    filter_iterator(predicate const &pred,
                    input_iterator const &begin,
                    input_iterator const &end = input_iterator());
    filter_iterator(filter_iterator const &it) = default;

    filter_iterator end(void) const;
    void find_next(void);
    void next(void);

private: // Private Property(ies)
    predicate pred_;
}; // class filter_iterator

/************************************************
 * Implementation: class filter_iterator
 ************************************************/

template <typename P, typename I>
inline filter_iterator<P, I>::filter_iterator(predicate const &pred,
                                              input_iterator const &begin,
                                              input_iterator const &end)
    : base_t(begin, end), pred_(pred)
{
    find_next();
}

template <typename P, typename I>
inline filter_iterator<P, I> filter_iterator<P, I>::end(void) const
{
    return filter_iterator(pred_, this->end_, this->end_);
}

template <typename P, typename I>
inline void filter_iterator<P, I>::find_next(void)
{
    while (this->it_ != this->end_ && !pred_(*(this->it_))) { ++(this->it_); }
}

template <typename P, typename I>
inline void filter_iterator<P, I>::next(void)
{
    ++(this->it_);
    find_next();
}

} // namespace esapp

#endif // ESAPP_FILTER_ITERATOR_HPP_
