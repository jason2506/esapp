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
#include <type_traits>

namespace esapp
{

/************************************************
 * Declaration: class generator_iterator
 ************************************************/

template <typename I, typename G>
class generator_iterator
    : public std::iterator<std::input_iterator_tag,
                           typename std::result_of<G(I &, I const &)>::type>
{
private: // Private Type(s)
    typedef std::iterator<
        std::input_iterator_tag,
        typename std::result_of<G(I &, I const &)>::type
    > base;

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
    generator_iterator(generator_iterator const &it);

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

template <typename I, typename G>
inline generator_iterator<I, G>::generator_iterator(iterator const &begin,
                                                    iterator const &end,
                                                    generator const &g)
    : begin_(begin), end_(end), g_(g)
{
    operator++();
}

template <typename I, typename G>
template <typename Initializer>
inline generator_iterator<I, G>::generator_iterator(iterator const &begin,
                                                    iterator const &end,
                                                    generator const &g,
                                                    Initializer const &init)
    : begin_(begin), end_(end), g_(g)
{
    init(begin_, end_);
    operator++();
}

template <typename I, typename G>
inline generator_iterator<I, G>::generator_iterator(generator_iterator const &it)
    : begin_(it.begin_), end_(it.end_), g_(it.g_),
      val_(it.val_), has_val_(it.has_val_)
{
    // do nothing
}

template <typename I, typename G>
inline typename generator_iterator<I, G>::generator_iterator
generator_iterator<I, G>::begin(void) const
{
    return *this;
}

template <typename I, typename G>
inline typename generator_iterator<I, G>::generator_iterator
generator_iterator<I, G>::end(void) const
{
    return generator_iterator(end_, end_, g_);
}

template <typename I, typename G>
inline typename generator_iterator<I, G>::generator_iterator &
generator_iterator<I, G>::operator++(void)
{
    has_val_ = begin_ != end_;
    val_ = has_val_ ? g_(begin_, end_) : decltype(val_)();

    return *this;
}

template <typename I, typename G>
inline typename generator_iterator<I, G>::generator_iterator
generator_iterator<I, G>::operator++(int)
{
    generator_iterator it(*this);
    operator++();
    return it;
}

template <typename I, typename G>
inline typename generator_iterator<I, G>::value_type &
generator_iterator<I, G>::operator*(void)
{
    return val_;
}

template <typename I, typename G>
inline typename generator_iterator<I, G>::value_type *
generator_iterator<I, G>::operator->(void) const
{
    return &val_;
}

template <typename I, typename G>
inline bool generator_iterator<I, G>::operator==(generator_iterator const &it) const
{
    return begin_ == it.begin_ && end_ == it.end_ && g_ == it.g_ &&
           val_ == it.val_ && has_val_ == it.has_val_;
}

template <typename I, typename G>
inline bool generator_iterator<I, G>::operator!=(generator_iterator const &it) const
{
    return !(*this == it);
}

} // namespace esapp

#endif // ESAPP_GEN_HPP_
