/************************************************
 *  token_iterator.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_TOKEN_ITERATOR_HPP_
#define ESAPP_TOKEN_ITERATOR_HPP_

#include <cwctype>
#include <string>

#include "generator.hpp"

namespace esapp
{

/************************************************
 * Inline Helper Function(s)
 ************************************************/

inline int ischs(std::wint_t c)
{
    return c >= u'\u4E00' && c <= u'\u9FFF';
}

inline int isfwalnum(std::wint_t c)
{
    return (c >= u'Ａ' && c <= u'Ｚ') ||
           (c >= u'ａ' && c <= u'ｚ') ||
           (c >= u'０' && c <= u'９');
}

/************************************************
 * Declaration: class token_iterator
 ************************************************/

class token_iterator : public generator<token_iterator,
                                        std::wstring::const_iterator,
                                        std::wstring>
{
private: // Private Type(s)
    typedef generator<token_iterator,
                      std::wstring::const_iterator,
                      std::wstring> base_t;

public: // Public Type(s)
    typedef typename base_t::iterator_category iterator_category;
    typedef typename base_t::value_type value_type;
    typedef typename base_t::reference reference;
    typedef typename base_t::pointer pointer;
    typedef typename base_t::difference_type difference_type;
    typedef typename base_t::input_iterator input_iterator;

public: // Public Method(s)
    token_iterator(void) = default;
    token_iterator(input_iterator const &begin,
                   input_iterator const &end = input_iterator());
    token_iterator(std::wstring const &s);
    token_iterator(token_iterator const &it) = default;

    template <typename Predicate>
    void skip(Predicate pred);
    template <typename Predicate>
    bool scan(Predicate pred);

    void next(void);
    reference get(void) const;
    bool equal(token_iterator const &it) const;
    bool ended(void) const;

private: // Private Property(ies)
    value_type token_;
    bool has_next_;
}; // class token_iterator

/************************************************
 * Implementation: class token_iterator
 ************************************************/

inline token_iterator::token_iterator(input_iterator const &begin,
                                      input_iterator const &end)
    : base_t(begin, end)
{
    skip(&std::iswspace);
    next();
}

inline token_iterator::token_iterator(std::wstring const &s)
    : token_iterator(s.begin(), s.end())
{
    // do nothing
}

template <typename Predicate>
inline void token_iterator::skip(Predicate pred)
{
    for ( ; it_ != end_ && pred(*it_); ++it_) { /* do nothing */ }
}

template <typename Predicate>
inline bool token_iterator::scan(Predicate pred)
{
    auto const begin = it_;
    skip(pred);

    bool scanned = begin != it_;
    if (scanned) { token_.assign(begin, it_); }

    return scanned;
}

inline void token_iterator::next(void)
{
    has_next_ = this->it_ != this->end_;
    if (!has_next_)
    {
        token_.clear();
        return;
    }

    if (!scan(&ischs) && !scan(&isfwalnum) && !scan(&std::iswalnum))
    {
        token_.assign(1, *it_);
        ++it_;
    }

    skip(&std::iswspace);
}

inline typename token_iterator::reference
token_iterator::get(void) const
{
    return token_;
}

inline bool token_iterator::equal(token_iterator const &it) const
{
    return this->it_ == it.it_ && has_next_ == it.has_next_;
}

inline bool token_iterator::ended(void) const
{
    return !has_next_;
}

} // namespace esapp

#endif // ESAPP_TOKENIZE_ITERATOR_HPP_
