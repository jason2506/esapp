/************************************************
 *  generator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
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

template <typename Derived, typename Value, typename Difference>
class generator
{
private: // Private Type(s)
    typedef Derived derived_t;

public: // Public Type(s)
    typedef ::std::input_iterator_tag iterator_category;
    typedef Value value_type;
    typedef value_type const *pointer;
    typedef value_type const &reference;
    typedef Difference difference_type;

public: // Public Method(s)
    // [NOTE] Derived classes must implement those methods
    // void next(void);
    // reference dereference(void) const;
    // bool equal(derived_t const &g) const;
    // bool valid(void) const;

    derived_t &operator++(void);
    derived_t operator++(int);
    reference operator*(void) const;
    pointer operator->(void) const;
    bool operator==(derived_t const &g) const;
    bool operator!=(derived_t const &g) const;
    explicit operator bool(void) const;

protected: // Protected Method(s)
    generator(void) = default;

private: // Private Method(s)
    derived_t &derived(void);
    derived_t const &derived(void) const;
}; // class generator

/************************************************
 * Implementation: class generator
 ************************************************/

template <typename D, typename T, typename X>
inline typename generator<D, T, X>::derived_t &
generator<D, T, X>::operator++(void)
{
    auto &that = derived();
    that.next();
    return that;
}

template <typename D, typename T, typename X>
inline typename generator<D, T, X>::derived_t
generator<D, T, X>::operator++(int)
{
    generator it(derived());
    operator++();
    return it;
}

template <typename D, typename T, typename X>
inline typename generator<D, T, X>::reference
generator<D, T, X>::operator*(void) const
{
    return derived().dereference();
}

template <typename D, typename T, typename X>
inline typename generator<D, T, X>::pointer
generator<D, T, X>::operator->(void) const
{
    return &(derived().dereference());
}

template <typename D, typename T, typename X>
inline bool generator<D, T, X>::operator==(derived_t const &g) const
{
    return derived().equal(g);
}

template <typename D, typename T, typename X>
inline bool generator<D, T, X>::operator!=(derived_t const &g) const
{
    return !derived().equal(g);
}

template <typename D, typename T, typename X>
inline generator<D, T, X>::operator bool(void) const
{
    return derived().valid();
}

template <typename D, typename T, typename X>
inline typename generator<D, T, X>::derived_t &
generator<D, T, X>::derived(void)
{
    return *static_cast<derived_t *>(this);
}

template <typename D, typename T, typename X>
inline typename generator<D, T, X>::derived_t const &
generator<D, T, X>::derived(void) const
{
    return *static_cast<derived_t const *>(this);
}

} // namespace esapp

#endif // ESAPP_GENERATOR_HPP_
