/************************************************
 *  tok.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_TOK_HPP_
#define ESAPP_TOK_HPP_

#include <cwctype>
#include <string>

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
 * Declaration: class tokenize_iterator
 ************************************************/

class tokenize_iterator : public generator<tokenize_iterator,
                                           std::wstring::const_iterator,
                                           std::wstring>
{
private: // Private Type(s)
    typedef generator<tokenize_iterator,
                      std::wstring::const_iterator,
                      std::wstring> supercls_t;

public: // Public Type(s)
    typedef typename supercls_t::iterator_category iterator_category;
    typedef typename supercls_t::value_type value_type;
    typedef typename supercls_t::reference reference;
    typedef typename supercls_t::pointer pointer;
    typedef typename supercls_t::difference_type difference_type;
    typedef typename supercls_t::input_iterator input_iterator;

public: // Public Method(s)
    tokenize_iterator(input_iterator const &begin, input_iterator const &end);
    tokenize_iterator(tokenize_iterator const &it);

    template <typename Predicate>
    void skip(Predicate pred);
    template <typename Predicate>
    bool scan(Predicate pred);

    void next(void);
    reference dereference(void) const;
    bool equal(tokenize_iterator const &it) const;

private: // Private Property(ies)
    value_type token_;
    bool has_next_;
}; // class tokenize_iterator

/************************************************
 * Implementation: class tokenize_iterator
 ************************************************/

inline tokenize_iterator::tokenize_iterator(input_iterator const &begin,
                                            input_iterator const &end)
    : supercls_t(begin, end)
{
    skip(&std::iswspace);
    next();
}

inline tokenize_iterator::tokenize_iterator(tokenize_iterator const &it)
    : supercls_t(it.it_, it.end_), token_(it.token_), has_next_(it.has_next_)
{
    // do nothing
}

template <typename Predicate>
inline void tokenize_iterator::skip(Predicate pred)
{
    for ( ; it_ != end_ && pred(*it_); ++it_) { /* do nothing */ }
}

template <typename Predicate>
inline bool tokenize_iterator::scan(Predicate pred)
{
    auto const begin = it_;
    skip(pred);

    bool scanned = begin != it_;
    if (scanned) { token_.assign(begin, it_); }

    return scanned;
}

inline void tokenize_iterator::next(void)
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

inline typename tokenize_iterator::reference
tokenize_iterator::dereference(void) const
{
    return token_;
}

inline bool tokenize_iterator::equal(tokenize_iterator const &it) const
{
    return this->it_ == it.it_ && this->end_ == it.end_ &&
           token_ == it.token_ && has_next_ == it.has_next_;
}

} // namespace esapp

#endif // ESAPP_TOK_HPP_
