/************************************************
 *  generator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_GENERATOR_HPP_
#define ESAPP_GENERATOR_HPP_

#include <iterator>

namespace esapp
{

/************************************************
 * Declaration: class generator
 ************************************************/

template <typename Derived, typename Iterator,
          typename Value = typename std::iterator_traits<Iterator>::value_type>
class generator : public std::iterator<std::input_iterator_tag, Value,
                                       ptrdiff_t, Value const *, Value const &>
{
private: // Private Type(s)
    typedef Derived derived_t;
    typedef std::iterator<std::input_iterator_tag, Value,
                          ptrdiff_t, Value const *, Value const &> base_t;

public: // Public Type(s)
    typedef typename base_t::iterator_category iterator_category;
    typedef typename base_t::value_type value_type;
    typedef typename base_t::reference reference;
    typedef typename base_t::pointer pointer;
    typedef typename base_t::difference_type difference_type;

    typedef Iterator input_iterator;

public: // Public Method(s)
    generator(void) = default;
    generator(input_iterator const &begin,
              input_iterator const &end = input_iterator());
    generator(generator const &g) = default;

    derived_t begin(void) const;
    derived_t end(void) const;

    void next(void);
    reference get(void) const;
    bool equal(derived_t const &it) const;
    bool ended(void) const;

    derived_t &operator++(void);
    derived_t operator++(int);
    reference operator*(void) const;
    pointer operator->(void) const;
    bool operator==(derived_t const &it) const;
    bool operator!=(derived_t const &it) const;
    operator bool(void) const;

private: // Private Method(s)
    derived_t &subcls(void);
    derived_t const &subcls(void) const;

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
inline typename generator<D, I, T>::derived_t
generator<D, I, T>::begin(void) const
{
    return subcls();
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::derived_t
generator<D, I, T>::end(void) const
{
    return derived_t(end_, end_);
}

template <typename D, typename I, typename T>
inline void generator<D, I, T>::next(void)
{
    ++it_;
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::reference
generator<D, I, T>::get(void) const
{
    return *it_;
}

template <typename D, typename I, typename T>
inline bool generator<D, I, T>::equal(derived_t const &it) const
{
    return it_ == it.it_;
}

template <typename D, typename I, typename T>
inline bool generator<D, I, T>::ended(void) const
{
    return it_ == end_;
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::derived_t &
generator<D, I, T>::operator++(void)
{
    auto &that = subcls();
    that.next();
    return that;
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::derived_t
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
    return subcls().get();
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::pointer
generator<D, I, T>::operator->(void) const
{
    return &(subcls().get());
}

template <typename D, typename I, typename T>
inline bool generator<D, I, T>::operator==(derived_t const &it) const
{
    return subcls().equal(it);
}

template <typename D, typename I, typename T>
inline bool generator<D, I, T>::operator!=(derived_t const &it) const
{
    return !subcls().equal(it);
}

template <typename D, typename I, typename T>
inline generator<D, I, T>::operator bool(void) const
{
    return !subcls().ended();
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::derived_t &generator<D, I, T>::subcls(void)
{
    return *static_cast<derived_t *>(this);
}

template <typename D, typename I, typename T>
inline typename generator<D, I, T>::derived_t const &generator<D, I, T>::subcls(void) const
{
    return *static_cast<derived_t const *>(this);
}

} // namespace esapp

#endif // ESAPP_GENERATOR_HPP_
