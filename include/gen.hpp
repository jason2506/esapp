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

namespace esapp
{

/************************************************
 * Declaration: class generator
 ************************************************/

template <typename D, typename I, typename T = typename I::value_type>
class generator : public std::iterator<std::input_iterator_tag, T,
                                       ptrdiff_t, T const *, T const &>
{
private: // Private Type(s)
    typedef D subcls_t;
    typedef std::iterator<std::input_iterator_tag, T,
                          ptrdiff_t, T const *, T const &> supercls_t;

public: // Public Type(s)
    typedef typename supercls_t::iterator_category iterator_category;
    typedef typename supercls_t::value_type value_type;
    typedef typename supercls_t::reference reference;
    typedef typename supercls_t::pointer pointer;
    typedef typename supercls_t::difference_type difference_type;

    typedef I input_iterator;

public: // Public Method(s)
    generator(input_iterator const &begin, input_iterator const &end);
    generator(generator const &g);

    subcls_t begin(void) const;
    subcls_t end(void) const;

    subcls_t &operator++(void);
    subcls_t operator++(int);
    reference operator*(void) const;
    pointer operator->(void) const;
    bool operator==(subcls_t const &it) const;
    bool operator!=(subcls_t const &it) const;

private: // Private Method(s)
    subcls_t &subcls(void);
    subcls_t const &subcls(void) const;

protected: // Protected Property(ies)
    input_iterator it_, end_;
}; // class generator

/************************************************
 * Implementation: class generator
 ************************************************/

template <typename D, typename I, typename T>
inline generator<D, I, T>::generator(input_iterator const &begin,
                                     input_iterator const &end)
    : it_(begin), end_(end)
{
    // do nothing
}

template <typename D, typename I, typename T>
inline generator<D, I, T>::generator(generator const &g)
    : it_(g.it_), end_(g.end_)
{
    // do nothing
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::subcls_t
generator<D, I, T>::begin(void) const
{
    return subcls();
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::subcls_t
generator<D, I, T>::end(void) const
{
    return subcls_t(end_, end_);
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::subcls_t &
generator<D, I, T>::operator++(void)
{
    auto &that = subcls();
    that.next();
    return that;
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::subcls_t
generator<D, I, T>::operator++(int)
{
    generator it(subcls());
    operator++();
    return it;
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::reference
generator<D, I, T>::operator*(void) const
{
    return subcls().dereference();
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::pointer
generator<D, I, T>::operator->(void) const
{
    return &(subcls().dereference());
}

template <typename D, typename I, typename T>
inline bool generator<D, I, T>::operator==(subcls_t const &it) const
{
    return subcls().equal(it);
}

template <typename D, typename I, typename T>
inline bool generator<D, I, T>::operator!=(subcls_t const &it) const
{
    return !subcls().equal(it);
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::subcls_t &generator<D, I, T>::subcls(void)
{
    return *static_cast<subcls_t *>(this);
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::subcls_t const &generator<D, I, T>::subcls(void) const
{
    return *static_cast<subcls_t const *>(this);
}

} // namespace esapp

#endif // ESAPP_GEN_HPP_
