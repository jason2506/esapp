/************************************************
 *  gen.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_GEN_HPP_
#define ESAPP_GEN_HPP_

#include <iterator>
#include <utility>

namespace esapp
{

/************************************************
 * Declaration: class generator_iterator
 ************************************************/

template <typename I, typename G, typename T>
class generator_iterator : public std::iterator<std::input_iterator_tag, T>
{
private: // Private Type(s)
    typedef std::iterator<std::input_iterator_tag, T> base;

public: // Public Type(s)
    typedef typename base::iterator_category iterator_category;
    typedef typename base::value_type value_type;
    typedef typename base::reference reference;
    typedef typename base::pointer pointer;
    typedef typename base::difference_type difference_type;

    typedef I iterator;
    typedef G generator;

public: // Public Method(s)
    generator_iterator(iterator const &begin, iterator const &end,
                       generator const &g);
    template <typename Initializer>
    generator_iterator(iterator const &begin, iterator const &end,
                       generator const &g, Initializer const &init);
    generator_iterator(generator_iterator const &g);

    generator_iterator begin(void) const;
    generator_iterator end(void) const;

    generator_iterator &operator++(void);
    generator_iterator operator++(int);
    value_type &operator*(void);
    value_type *operator->(void) const;
    bool operator==(generator_iterator const &it) const;
    bool operator!=(generator_iterator const &it) const;

private: // Private Property(ies)
    iterator begin_, end_;
    generator g_;
    value_type val_;
    bool has_val_;
}; // class generator_iterator

/************************************************
 * Implementation: class generator_iterator
 ************************************************/

template <typename I, typename G, typename T>
inline generator_iterator<I, G, T>::generator_iterator(iterator const &begin,
                                                       iterator const &end,
                                                       generator const &g)
    : begin_(begin), end_(end), g_(g), has_val_(false)
{
    // do nothing
}

template <typename I, typename G, typename T>
template <typename Initializer>
inline generator_iterator<I, G, T>::generator_iterator(iterator const &begin,
                                                       iterator const &end,
                                                       generator const &g,
                                                       Initializer const &init)
    : begin_(begin), end_(end), g_(g), has_val_(false)
{
    init(begin_, end_);
}

template <typename I, typename G, typename T>
inline generator_iterator<I, G, T>::generator_iterator(generator_iterator const &g)
    : generator_iterator(g.begin_, g.end_, g.g_)
{
    // do nothing
}

template <typename I, typename G, typename T>
inline typename generator_iterator<I, G, T>::generator_iterator
generator_iterator<I, G, T>::begin(void) const
{
    return *this;
}

template <typename I, typename G, typename T>
inline typename generator_iterator<I, G, T>::generator_iterator
generator_iterator<I, G, T>::end(void) const
{
    return generator_iterator(end_, end_, g_);
}

template <typename I, typename G, typename T>
inline typename generator_iterator<I, G, T>::generator_iterator &
generator_iterator<I, G, T>::operator++(void)
{
    has_val_ = false;
    return *this;
}

template <typename I, typename G, typename T>
inline typename generator_iterator<I, G, T>::generator_iterator
generator_iterator<I, G, T>::operator++(int)
{
    generator_iterator it(*this);
    operator++();
    return it;
}

template <typename I, typename G, typename T>
inline typename generator_iterator<I, G, T>::value_type &
generator_iterator<I, G, T>::operator*(void)
{
    if (!has_val_) { val_ = g_(begin_, end_); }

    has_val_ = true;
    return val_;
}

template <typename I, typename G, typename T>
inline typename generator_iterator<I, G, T>::value_type *
generator_iterator<I, G, T>::operator->(void) const
{
    return &val_;
}

template <typename I, typename G, typename T>
inline bool generator_iterator<I, G, T>::operator==(generator_iterator const &it) const
{
    return begin_ == it.begin_ && end_ == it.end_;
}

template <typename I, typename G, typename T>
inline bool generator_iterator<I, G, T>::operator!=(generator_iterator const &it) const
{
    return !(*this == it);
}

} // namespace esapp

#endif // ESAPP_GEN_HPP_
