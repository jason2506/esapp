/************************************************
 *  generator_adaptor.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_GENERATOR_ADAPTOR_HPP_
#define ESAPP_GENERATOR_ADAPTOR_HPP_

#include <iterator>

#include "generator.hpp"

namespace esapp
{

template <typename I> class generator_adaptor;

/************************************************
 * Inline Helper Function(s)
 ************************************************/

template <typename Iterator>
inline generator_adaptor<Iterator>
make_generator_adaptor(Iterator const &begin,
                       Iterator const &end = Iterator())
{
    typedef decltype(make_generator_adaptor(begin, end)) it_t;
    return it_t(begin, end);
}

/************************************************
 * Declaration: class generator_adaptor
 ************************************************/

template <typename Iterator>
class generator_adaptor : public generator
    <
        generator_adaptor<Iterator>,
        typename ::std::iterator_traits<Iterator>::value_type,
        typename ::std::iterator_traits<Iterator>::difference_type
    >
{
private: // Private Type(s)
    typedef generator
        <
            generator_adaptor<Iterator>,
            typename ::std::iterator_traits<Iterator>::value_type,
            typename ::std::iterator_traits<Iterator>::difference_type
        > base_t;

public: // Public Type(s)
    typedef typename base_t::iterator_category iterator_category;
    typedef typename base_t::value_type value_type;
    typedef typename base_t::reference reference;
    typedef typename base_t::pointer pointer;
    typedef typename base_t::difference_type difference_type;

    typedef Iterator input_iterator;

public: // Public Method(s)
    generator_adaptor(void) = default;
    explicit generator_adaptor(input_iterator const &begin,
                               input_iterator const &end = input_iterator());

    void next(void);
    reference dereference(void) const;
    bool equal(generator_adaptor const &g) const;
    bool valid(void) const;

    input_iterator const &base(void) const;
    input_iterator end(void) const;

private: // Private Property(ies)
    input_iterator it_, end_;
}; // class generator_adaptor

/************************************************
 * Implementation: class generator_adaptor
 ************************************************/

template <typename I>
inline generator_adaptor<I>::generator_adaptor(input_iterator const &begin,
                                               input_iterator const &end)
    : it_(begin), end_(end)
{
    // do nothing
}

template <typename I>
inline void generator_adaptor<I>::next(void)
{
    ++it_;
}

template <typename I>
inline typename generator_adaptor<I>::reference
generator_adaptor<I>::dereference(void) const
{
    return *it_;
}

template <typename I>
inline bool generator_adaptor<I>::equal(generator_adaptor const &g) const
{
    return it_ == g.it_;
}

template <typename I>
inline bool generator_adaptor<I>::valid(void) const
{
    return it_ != end_;
}

template <typename I>
inline typename generator_adaptor<I>::input_iterator const &
generator_adaptor<I>::base(void) const
{
    return it_;
}

template <typename I>
inline typename generator_adaptor<I>::input_iterator
generator_adaptor<I>::end(void) const
{
    return end_;
}

} // namespace esapp

#endif // ESAPP_GENERATOR_ADAPTOR_HPP_
