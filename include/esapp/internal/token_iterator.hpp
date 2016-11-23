/************************************************
 *  token_iterator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_INTERNAL_TOKEN_ITERATOR_HPP_
#define ESAPP_INTERNAL_TOKEN_ITERATOR_HPP_

#include <cwctype>
#include <vector>

#include <literator/iterator_adaptor.hpp>

#include "utf8_decode_iterator.hpp"

namespace esapp {

namespace internal {

// forward declaration
class token_iterator;

/************************************************
 * Inline Helper Function(s)
 ************************************************/

inline int ischs(std::wint_t c) {
    return c >= u'\u4E00' && c <= u'\u9FFF';
}

inline int isfwalnum(std::wint_t c) {
    return (c >= u'Ａ' && c <= u'Ｚ') ||
           (c >= u'ａ' && c <= u'ｚ') ||
           (c >= u'０' && c <= u'９');
}

/************************************************
 * Declaration: type token_iterator_base
 ************************************************/

using token_iterator_base = literator::iterator_adaptor<
    token_iterator,
    utf8_decode_iterator::base_type,
    std::vector<utf8_decode_iterator::value_type>,
    std::forward_iterator_tag,
    std::vector<utf8_decode_iterator::value_type> const &
>;

/************************************************
 * Declaration: class token_iterator
 ************************************************/

class token_iterator : public token_iterator_base {
 private:  // Private Type(s)
    friend literator::iterator_core_access;
    using super_t = token_iterator_base;

 public:  // Public Type(s)
    using base_type = super_t::base_type;
    using value_type = super_t::value_type;

 public:  // Public Method(s)
    token_iterator() = default;
    token_iterator(base_type it, base_type end);

 private:  // Private Method(s)
    super_t::reference dereference() const;
    void increment();

    void next_token();

    template <typename Predicate>
    bool scan_while(Predicate pred);

 private:  // Private Property(ies)
    utf8_decode_iterator u8_it_;
    utf8_decode_iterator u8_end_;
    value_type token_;
};  // class token_iterator

/************************************************
 * Implementation: class token_iterator
 ************************************************/

inline token_iterator::token_iterator(base_type it, base_type end)
    : super_t(it), u8_it_(it, end), u8_end_(end, end), token_() {
    next_token();
}

inline typename token_iterator::super_t::reference
token_iterator::dereference() const {
    return token_;
}

inline void token_iterator::increment() {
    next_token();
}

inline void token_iterator::next_token() {
    token_.clear();
    if (u8_it_ == u8_end_) {
        this->base_reference() = u8_it_.base();
        return;
    }

    if (!scan_while(&ischs) && !scan_while(&isfwalnum) &&
        !scan_while(&std::iswalnum) && !scan_while(&std::iswspace)) {
        token_.push_back(*u8_it_);
        this->base_reference() = u8_it_.base();
        ++u8_it_;
    }
}

template <typename Predicate>
inline bool token_iterator::scan_while(Predicate pred) {
    while (u8_it_ != u8_end_ && pred(*u8_it_)) {
        token_.push_back(*u8_it_);
        this->base_reference() = u8_it_.base();
        ++u8_it_;
    }

    return token_.size() > 0;
}

}  // namespace internal

}  // namespace esapp

#endif  // ESAPP_INTERNAL_TOKEN_ITERATOR_HPP_
