/************************************************
 *  decode_utf8.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2017, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_INTERNAL_DECODE_UTF8_HPP_
#define ESAPP_INTERNAL_DECODE_UTF8_HPP_

#include <exception>
#include <stdexcept>

namespace esapp {

namespace internal {

/************************************************
 * Declaration: class utf8_decode_error
 ************************************************/

class invalid_byte_sequence : public std::exception { /* empty */ };

/************************************************
 * Declaration: function decode_utf8<I>
 ************************************************/

template <typename Result, typename Iterator>
Result decode_utf8(Iterator &it, Iterator const &end) {  // NOLINT(runtime/references)
    Result codepoint = *it;

    std::size_t byte_needed = 0;
    if ((codepoint & 0x80) == 0) {
        // do nothing
    } else if ((codepoint & 0xE0) == 0xC0) {
        byte_needed = 1;
        codepoint &= 0x1F;
    } else if ((codepoint & 0xF0) == 0xE0) {
        byte_needed = 2;
        codepoint &= 0x0F;
    } else if ((codepoint & 0xF8) == 0xF0) {
        byte_needed = 3;
        codepoint &= 0x07;
    } else {
        throw invalid_byte_sequence();
    }

    ++it;
    for ( ; byte_needed > 0; --byte_needed) {
        if (it == end) {
            throw std::out_of_range("Input iterator out of range");
        }

        auto byte = *it;
        if ((byte & 0xC0) != 0x80) {
            throw invalid_byte_sequence();
        }

        codepoint <<= 6;
        codepoint |= byte & 0x3f;
        ++it;
    }

    return codepoint;
}

}  // namespace internal

}  // namespace esapp

#endif  // ESAPP_INTERNAL_DECODE_UTF8_HPP_
