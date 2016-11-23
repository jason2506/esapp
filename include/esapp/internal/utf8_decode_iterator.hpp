/************************************************
 *  utf8_decode_iterator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_INTERNAL_UTF8_DECODE_ITERATOR_HPP_
#define ESAPP_INTERNAL_UTF8_DECODE_ITERATOR_HPP_

#include <iterator>
#include <exception>
#include <stdexcept>
#include <string>

#include <literator/iterator_adaptor.hpp>

namespace esapp {

namespace internal {

// forward declaration
class utf8_decode_iterator;

/************************************************
 * Declaration: class utf8_decode_error
 ************************************************/

class invalid_byte_sequence : public std::exception { /* empty */ };

/************************************************
 * Declaration: type utf8_decode_iterator_base
 ************************************************/

using utf8_decode_iterator_base = literator::iterator_adaptor<
    utf8_decode_iterator,
    std::string::const_iterator,
    std::uint32_t,
    std::forward_iterator_tag,
    std::uint32_t const &
>;

/************************************************
 * Declaration: class utf8_decode_iterator
 ************************************************/

class utf8_decode_iterator : public utf8_decode_iterator_base {
 private:  // Private Type(s)
    friend literator::iterator_core_access;
    using super_t = utf8_decode_iterator_base;

 public:  // Public Type(s)
    using base_type = super_t::base_type;
    using value_type = super_t::value_type;

 public:  // Public Method(s)
    utf8_decode_iterator() = default;
    utf8_decode_iterator(base_type it, base_type end);

 private:  // Private Method(s)
    super_t::reference dereference() const;
    void increment();

    void next_codepoint();
    typename base_type::value_type next_byte();

 private:  // Private Property(ies)
    base_type end_;
    value_type codepoint_;
};  // class utf8_decode_iterator

/************************************************
 * Implementation: class utf8_decode_iterator
 ************************************************/

inline utf8_decode_iterator::utf8_decode_iterator(base_type it, base_type end)
    : super_t(it), end_(end), codepoint_(0) {
    next_codepoint();
}

inline typename utf8_decode_iterator::super_t::reference
utf8_decode_iterator::dereference() const {
    return codepoint_;
}

inline void utf8_decode_iterator::increment() {
    ++(this->base_reference());
    next_codepoint();
}

inline void utf8_decode_iterator::next_codepoint() {
    if (this->base_reference() == end_) {
        codepoint_ = 0;
        return;
    }

    codepoint_ = *(this->base_reference());

    std::size_t byte_needed = 0;
    if ((codepoint_ & 0x80) == 0) {
        // do nothing
    } else if ((codepoint_ & 0xE0) == 0xC0) {
        byte_needed = 1;
        codepoint_ &= 0x1F;
    } else if ((codepoint_ & 0xF0) == 0xE0) {
        byte_needed = 2;
        codepoint_ &= 0x0F;
    } else if ((codepoint_ & 0xF8) == 0xF0) {
        byte_needed = 3;
        codepoint_ &= 0x07;
    } else {
        throw invalid_byte_sequence();
    }

    for ( ; byte_needed > 0; --byte_needed) {
        codepoint_ <<= 6;
        codepoint_ |= next_byte() & 0x3f;
    }
}

inline typename utf8_decode_iterator::base_type::value_type
utf8_decode_iterator::next_byte() {
    ++(this->base_reference());
    if (this->base_reference() == end_) {
        throw std::out_of_range("Input iterator out of range");
    }

    auto byte = *(this->base_reference());
    if ((byte & 0xC0) != 0x80) {
        throw invalid_byte_sequence();
    }

    return byte;
}

}  // namespace internal

}  // namespace esapp

#endif  // ESAPP_INTERNAL_UTF8_DECODE_ITERATOR_HPP_
