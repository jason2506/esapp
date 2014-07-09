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
 * Declaration: class tokenizer
 ************************************************/

class tokenizer
{
public: // Public Type(s)
    typedef std::wstring sequence;

public: // Public Method(s)
    tokenizer(sequence const &s);

    void reset(void);
    sequence next(void);
    bool has_next(void) const;

private: // Private Method(s)
    template <typename Predicate>
    void skip(Predicate pred);

    template <typename Predicate>
    bool scan(sequence &token, Predicate pred);

private: // Private Property(ies)
    sequence const &s_;
    sequence::const_iterator it_;
}; // class tokenizer

/************************************************
 * Implementation: class tokenizer
 ************************************************/

inline tokenizer::tokenizer(sequence const &s)
    : s_(s), it_(s.begin())
{
    skip(&std::iswspace);
}

inline void tokenizer::reset(void)
{
    it_ = s_.begin();
}

inline tokenizer::sequence tokenizer::next(void)
{
    decltype(next()) token;
    if (!scan(token, &ischs) &&
        !scan(token, &isfwalnum) &&
        !scan(token, &std::iswalnum))
    {
        token.assign(1, *it_);
        ++it_;
    }

    skip(&std::iswspace);
    return token;
}

inline bool tokenizer::has_next(void) const
{
    return it_ != s_.end();
}

template <typename Predicate>
inline void tokenizer::skip(Predicate pred)
{
    for ( ; has_next() && pred(*it_); ++it_) { /* do nothing */ }
}

template <typename Predicate>
inline bool tokenizer::scan(sequence &token, Predicate pred)
{
    auto begin = it_;
    skip(pred);

    bool scanned = begin != it_;
    if (scanned) { token.assign(begin, it_); }

    return scanned;
}

} // namespace esapp

#endif // ESAPP_TOK_HPP_
