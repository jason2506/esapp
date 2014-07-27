/************************************************
 *  filter.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_FILTER_HPP_
#define ESAPP_FILTER_HPP_

#include <type_traits>

#include "gen.hpp"

namespace esapp
{

template <typename P, typename I> class filter_iterator;

/************************************************
 * Inline Helper Function(s)
 ************************************************/

template <typename Predicate, typename Iterable>
inline auto make_filter_iterator(Predicate const &predicate,
                                 Iterable const &iterable)
    -> filter_iterator<Predicate, decltype(iterable.begin())>
{
    typedef decltype(make_filter_iterator(predicate, iterable)) it_t;
    return it_t(predicate, iterable.begin(), iterable.end());
}

/************************************************
 * Declaration: class filter_iterator
 ************************************************/

template <typename Predicate, typename Iterator>
class filter_iterator
    : public generator<
        filter_iterator<Predicate, Iterator>,
        Iterator,
        typename std::remove_const<
            typename std::remove_reference<
                decltype(*std::declval<Iterator>())
            >::type
        >::type
    >
{
private: // Private Type(s)
    typedef generator<
        filter_iterator,
        Iterator,
        typename std::remove_const<
            typename std::remove_reference<
                decltype(*std::declval<Iterator>())
            >::type
        >::type
    > supercls_t;

public: // Public Type(s)
    typedef typename supercls_t::iterator_category iterator_category;
    typedef typename supercls_t::value_type value_type;
    typedef typename supercls_t::reference reference;
    typedef typename supercls_t::pointer pointer;
    typedef typename supercls_t::difference_type difference_type;
    typedef typename supercls_t::input_iterator input_iterator;

    typedef Predicate predicate;

public: // Public Method(s)
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
    : supercls_t(begin, end), pred_(pred)
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

#endif // ESAPP_FILTER_HPP_
