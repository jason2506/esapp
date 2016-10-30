/************************************************
 *  utf8_decode_iterator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_UTF8_DECODE_ITERATOR_HPP_
#define ESAPP_UTF8_DECODE_ITERATOR_HPP_

#include <exception>
#include <stdexcept>
#include <string>

#include "nested_generator.hpp"
#include "generator_adaptor.hpp"

namespace esapp
{

namespace impl
{

/************************************************
 * Declaration: class utf8_decode_error
 ************************************************/

class invalid_byte_sequence : public ::std::exception { /* empty */ };

/************************************************
 * Declaration: class utf8_decode_iterator
 ************************************************/

class utf8_decode_iterator
    : public nested_generator
    <
        utf8_decode_iterator,
        generator_adaptor<::std::string::const_iterator>,
        uint32_t
    >
{
private: // Private Type(s)
    typedef nested_generator
        <
            utf8_decode_iterator,
            generator_adaptor<::std::string::const_iterator>,
            uint32_t
        > base_t;

public: // Public Type(s)
    typedef typename base_t::iterator_category iterator_category;
    typedef typename base_t::value_type value_type;
    typedef typename base_t::reference reference;
    typedef typename base_t::pointer pointer;
    typedef typename base_t::difference_type difference_type;

    typedef typename base_t::inner_generator inner_generator;
    typedef typename inner_generator::input_iterator input_iterator;

public: // Public Method(s)
    utf8_decode_iterator(void) = default;
    explicit utf8_decode_iterator(::std::string const &s);

    void next(void);
    reference dereference(void) const;
    bool equal(utf8_decode_iterator const &it) const;
    bool valid(void) const;

private: // Private Method(s)
    char next_byte(void);

private: // Private Property(ies)
    value_type char_;
}; // class utf8_decode_iterator

/************************************************
 * Implementation: class utf8_decode_iterator
 ************************************************/

inline utf8_decode_iterator::utf8_decode_iterator(::std::string const &s)
    : base_t(inner_generator(s.begin(), s.end()))
{
    next();
}

inline void utf8_decode_iterator::next(void)
{
    if (!base_t::valid())
    {
        char_ = 0;
        return;
    }

    int byte_needed = 0;
    char_ = *base_t::base();
    base_t::next();
    if ((char_ & 0x80) == 0)
    {
        // do nothing
    }
    else if ((char_ & 0xE0) == 0xC0)
    {
        byte_needed = 1;
        char_ &= 0x1F;
    }
    else if ((char_ & 0xF0) == 0xE0)
    {
        byte_needed = 2;
        char_ &= 0x0F;
    }
    else if ((char_ & 0xF8) == 0xF0)
    {
        byte_needed = 3;
        char_ &= 0x07;
    }
    else
    {
        throw invalid_byte_sequence();
    }

    for ( ; byte_needed > 0; --byte_needed)
    {
        char_ <<= 6;
        char_ |= next_byte() & 0x3f;
    }
}

inline typename utf8_decode_iterator::reference
utf8_decode_iterator::dereference(void) const
{
    return char_;
}

inline bool utf8_decode_iterator::equal(utf8_decode_iterator const &it) const
{
    return base_t::equal(it) && char_ == it.char_;
}

inline bool utf8_decode_iterator::valid(void) const
{
    return base_t::valid() || char_ != 0;
}

inline char utf8_decode_iterator::next_byte(void)
{
    if (!base_t::valid())
    {
        throw ::std::out_of_range("Input iterator out of range");
    }

    decltype(next_byte()) byte = *base_t::base();
    if ((byte & 0xC0) != 0x80)
    {
        throw invalid_byte_sequence();
    }

    base_t::next();
    return byte;
}

} // namespace impl

} // namespace esapp

#endif // ESAPP_UTF8_DECODE_ITERATOR_HPP_
