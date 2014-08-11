/************************************************
 *  flatten_iterator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_FLATTEN_ITERATOR_HPP_
#define ESAPP_FLATTEN_ITERATOR_HPP_

#include <memory>
#include <type_traits>
#include <utility>

#include "generator.hpp"

namespace esapp
{

template <typename I> class flatten_iterator;

/************************************************
 * Inline Helper Function(s)
 ************************************************/

template <typename Iterable>
inline auto make_flatten_iterator(Iterable const &iterable)
    -> flatten_iterator<decltype(iterable.begin())>
{
    typedef decltype(make_flatten_iterator(iterable)) it_t;
    return it_t(iterable.begin(), iterable.end());
}

/************************************************
 * Declaration: class flatten_iterator
 ************************************************/

template <typename Iterator>
class flatten_iterator
    : public generator<
        flatten_iterator<Iterator>,
        Iterator,
        typename std::remove_const<
            typename std::remove_reference<
                decltype(*(std::declval<Iterator>()->begin()))
            >::type
        >::type
    >
{
private: // Private Type(s)
    typedef generator<
        flatten_iterator,
        Iterator,
        typename std::remove_const<
            typename std::remove_reference<
                decltype(*(std::declval<Iterator>()->begin()))
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
    typedef decltype(std::declval<Iterator>()->begin()) value_iterator;

public: // Public Method(s)
    flatten_iterator(void) = default;
    flatten_iterator(input_iterator const &begin, input_iterator const &end);
    flatten_iterator(flatten_iterator const &it);

    flatten_iterator end(void) const;
    void next(void);
    reference dereference(void) const;
    bool equal(flatten_iterator const &it) const;

private: // Private Method(s)
    flatten_iterator(input_iterator const &begin,
                   input_iterator const &end,
                   std::unique_ptr<value_iterator> &&val_it_ptr);

private: // Private Property(ies)
    std::unique_ptr<value_iterator> val_it_ptr_;
}; // class flatten_iterator

/************************************************
 * Implementation: class flatten_iterator
 ************************************************/

template <typename I>
inline flatten_iterator<I>::flatten_iterator(input_iterator const &begin,
                                         input_iterator const &end)
    : supercls_t(begin, end), val_it_ptr_(nullptr)
{
    if (this->it_ != this->end_)
    {
        val_it_ptr_.reset(new value_iterator(this->it_->begin()));
    }
}

template <typename I>
inline flatten_iterator<I>::flatten_iterator(flatten_iterator const &it)
    : supercls_t(it.it_, it.end_)
{
    if (it.val_it_ptr_.get() != nullptr)
    {
        val_it_ptr_.reset(new value_iterator(*(it.val_it_ptr_)));
    }
    else
    {
        val_it_ptr_.reset();
    }
}

template <typename I>
inline flatten_iterator<I>::flatten_iterator(
    input_iterator const &begin, input_iterator const &end,
    std::unique_ptr<value_iterator> &&val_it_ptr)
    : supercls_t(begin, end), val_it_ptr_(std::move(val_it_ptr))
{
    // do nothing
}

template <typename I>
inline flatten_iterator<I> flatten_iterator<I>::end(void) const
{
    return flatten_iterator(this->end_, this->end_,
                            decltype(val_it_ptr_)(nullptr));
}

template <typename I>
inline void flatten_iterator<I>::next(void)
{
    if (++(*val_it_ptr_) == this->it_->end())
    {
        if (++this->it_ == this->end_)  { val_it_ptr_.reset(); }
        else                            { *val_it_ptr_ = this->it_->begin(); }
    }
}

template <typename I>
inline typename flatten_iterator<I>::reference
flatten_iterator<I>::dereference(void) const
{
    return **val_it_ptr_;
}

template <typename I>
inline bool flatten_iterator<I>::equal(flatten_iterator const &it) const
{
    if (val_it_ptr_)            { return (bool) it.val_it_ptr_; }
    else if (it.val_it_ptr_)    { return false; }

    return *val_it_ptr_ == *(it.val_it_ptr_);
}

} // namespace esapp

#endif // ESAPP_FLATTEN_ITERATOR_HPP_
