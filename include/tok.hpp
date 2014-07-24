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

template <typename Predicate>
inline void skip(std::wstring::const_iterator &it,
                 std::wstring::const_iterator const &end,
                 Predicate pred)
{
    for ( ; it != end && pred(*it); ++it) { /* do nothing */ }
}

template <typename Predicate>
inline bool scan(std::wstring::const_iterator &it,
                 std::wstring::const_iterator const &end,
                 std::wstring &token, Predicate pred)
{
    auto const begin = it;
    skip(it, end, pred);

    bool scanned = begin != it;
    if (scanned) { token.assign(begin, it); }

    return scanned;
}

inline std::wstring tokenize(std::wstring::const_iterator &it,
                             std::wstring::const_iterator const &end)
{
    decltype(tokenize(it, end)) token;
    if (!scan(it, end, token, &ischs) &&
        !scan(it, end, token, &isfwalnum) &&
        !scan(it, end, token, &std::iswalnum))
    {
        token.assign(1, *it);
        ++it;
    }

    skip(it, end, &std::iswspace);
    return token;
}

} // namespace esapp

#endif // ESAPP_TOK_HPP_
