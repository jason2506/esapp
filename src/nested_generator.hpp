/************************************************
 *  nested_generator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_NESTED_GENERATOR_HPP_
#define ESAPP_NESTED_GENERATOR_HPP_

#include <iterator>

#include "generator.hpp"

namespace esapp
{

/************************************************
 * Declaration: class nested_generator
 ************************************************/

template <typename Derived, typename Generator,
          typename Value = typename ::std::iterator_traits<Generator>::value_type>
class nested_generator : public generator
    <
        Derived,
        Value,
        typename ::std::iterator_traits<Generator>::difference_type
    >
{
private: // Private Type(s)
    typedef generator
        <
            Derived,
            Value,
            typename ::std::iterator_traits<Generator>::difference_type
        > base_t;
    typedef Derived derived_t;

public: // Public Type(s)
    typedef typename base_t::iterator_category iterator_category;
    typedef typename base_t::value_type value_type;
    typedef typename base_t::reference reference;
    typedef typename base_t::pointer pointer;
    typedef typename base_t::difference_type difference_type;

    typedef Generator inner_generator;

public: // Public Method(s)
    void next(void);
    reference dereference(void) const;
    bool equal(derived_t const &g) const;
    bool valid(void) const;

    inner_generator const &base(void) const;

protected: // Protected Method(s)
    nested_generator(void) = default;
    explicit nested_generator(inner_generator const &g);

private: // Private Property(ies)
    inner_generator g_;
}; // class nested_generator

/************************************************
 * Implementation: class nested_generator
 ************************************************/

template <typename D, typename G, typename T>
inline nested_generator<D, G, T>::nested_generator(inner_generator const &g)
    : g_(g)
{
    // do nothing
}

template <typename D, typename G, typename T>
inline void nested_generator<D, G, T>::next(void)
{
    ++g_;
}

template <typename D, typename G, typename T>
inline typename nested_generator<D, G, T>::reference
nested_generator<D, G, T>::dereference(void) const
{
    return *g_;
}

template <typename D, typename G, typename T>
inline bool nested_generator<D, G, T>::equal(derived_t const &g) const
{
    return g_ == g.g_;
}

template <typename D, typename G, typename T>
inline bool nested_generator<D, G, T>::valid(void) const
{
    return static_cast<bool>(g_);
}

template <typename D, typename G, typename T>
inline typename nested_generator<D, G, T>::inner_generator const &
nested_generator<D, G, T>::base(void) const
{
    return g_;
}

} // namespace esapp

#endif // ESAPP_NESTED_GENERATOR_HPP_
