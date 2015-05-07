/************************************************
 *  token_iterator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2015, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_TOKEN_ITERATOR_HPP_
#define ESAPP_TOKEN_ITERATOR_HPP_

#include <cwctype>
#include <string>
#include <vector>

#include "nested_generator.hpp"
#include "utf8_decode_iterator.hpp"

namespace esapp
{

namespace impl
{

/************************************************
 * Inline Helper Function(s)
 ************************************************/

inline int ischs(::std::wint_t c)
{
    return c >= u'\u4E00' && c <= u'\u9FFF';
}

inline int isfwalnum(::std::wint_t c)
{
    return (c >= u'Ａ' && c <= u'Ｚ') ||
           (c >= u'ａ' && c <= u'ｚ') ||
           (c >= u'０' && c <= u'９');
}

/************************************************
 * Declaration: class token_iterator
 ************************************************/

class token_iterator : public nested_generator
    <
        token_iterator,
        utf8_decode_iterator,
        ::std::vector<utf8_decode_iterator::value_type>
    >
{
private: // Private Type(s)
    typedef nested_generator
        <
            token_iterator,
            utf8_decode_iterator,
            ::std::vector<utf8_decode_iterator::value_type>
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
    token_iterator(void) = default;
    explicit token_iterator(::std::string const &s);

    void next(void);
    reference dereference(void) const;
    bool equal(token_iterator const &it) const;
    bool valid(void) const;

    input_iterator position(void) const;

private: // Private Method(s)
    template <typename Predicate>
    void skip(Predicate pred);
    template <typename Predicate>
    bool scan(Predicate pred);

private: // Private Property(ies)
    value_type token_;
    input_iterator it_;
}; // class token_iterator

/************************************************
 * Implementation: class token_iterator
 ************************************************/

inline token_iterator::token_iterator(::std::string const &s)
    : base_t(inner_generator(s))
{
    next();
}

template <typename Predicate>
inline void token_iterator::skip(Predicate pred)
{
    for ( ; base_t::valid() && pred(*base_t::base()); base_t::next())
    {
        it_ = base().base().base();
    }
}

template <typename Predicate>
inline bool token_iterator::scan(Predicate pred)
{
    auto const &current = base();
    auto const begin(current);
    skip(pred);

    bool scanned = begin != current;
    if (scanned) { token_.assign(begin, current); }

    return scanned;
}

inline void token_iterator::next(void)
{
    if (!base_t::valid())
    {
        token_.clear();
        return;
    }

    if (!scan(&ischs) && !scan(&isfwalnum) &&
        !scan(&::std::iswalnum) && !scan(&::std::iswspace))
    {
        token_.assign(1, *base_t::base());
        it_ = base().base().base();
        base_t::next();
    }
}

inline typename token_iterator::reference
token_iterator::dereference(void) const
{
    return token_;
}

inline bool token_iterator::equal(token_iterator const &it) const
{
    return base_t::equal(it) && token_ == it.token_;
}

inline bool token_iterator::valid(void) const
{
    return base_t::valid() || !token_.empty();
}

inline token_iterator::input_iterator token_iterator::position(void) const
{
    return it_;
}

} // namespace impl

} // namespace esapp

#endif // ESAPP_TOKENIZE_ITERATOR_HPP_
