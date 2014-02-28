/************************************************
 *  tok.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_TOK_HPP_
#define ESAPP_TOK_HPP_

#include <cctype>
#include <array>
#include <string>

namespace esapp
{

/************************************************
 * Inline Helper Function(s)
 ************************************************/

inline bool ischs(int c)
{
    return c >= u'\u4E00' && c <= u'\u9FFF';
}

/************************************************
 * Declaration: class TokenIterator
 ************************************************/

class TokenIterator
{
public: // Public Type(s)
    typedef std::wstring Sequence;
    typedef Sequence::const_iterator Iterator;

    typedef std::input_iterator_tag iterator_category;
    typedef Sequence value_type;
    typedef ptrdiff_t difference_type;
    typedef value_type const *pointer;
    typedef value_type const &reference;

public: // Public Method(s)
    TokenIterator(Sequence const &sequence);
    TokenIterator(Iterator const &begin, Iterator const &end);

    TokenIterator end(void) const;
    bool at_end(void) const;

    TokenIterator &operator++(void);
    TokenIterator operator++(int);

    reference operator*(void) const;
    pointer operator->(void) const;

    bool operator==(TokenIterator const &it) const;
    bool operator!=(TokenIterator const &it) const;

private: // Private Method(s)
    template <typename Predicate>
    void skip(Predicate pred);

    template <typename Predicate>
    bool scan(Predicate pred);

private: // Private Property(ies)
    Iterator begin_;
    Iterator end_;
    Sequence token_;
    bool at_end_;
}; // class TokenIterator

/************************************************
 * Implementation: class TokenIterator
 ************************************************/

inline TokenIterator::TokenIterator(Sequence const &sequence)
    : TokenIterator(sequence.begin(), sequence.end())
{
    // do nothing
}

inline TokenIterator::TokenIterator(Iterator const &begin, Iterator const &end)
    : begin_(begin), end_(end), at_end_(begin_ == end_)
{
    if (!at_end_) { ++(*this); }
}

inline TokenIterator TokenIterator::end(void) const
{
    return TokenIterator(end_, end_);
}

inline bool TokenIterator::at_end(void) const
{
    return at_end_;
}

inline TokenIterator &TokenIterator::operator++(void)
{
    skip(&isspace);
    if (begin_ == end_)
    {
        at_end_ = true;
        token_.clear();
    }
    else if (!scan(&ischs) && !scan(&isalnum))
    {
        token_.assign(1, *begin_);
        ++begin_;
    }

    return *this;
}

inline TokenIterator TokenIterator::operator++(int)
{
    decltype((*this)++) it(*this);
    ++(*this);
    return it;
}

inline TokenIterator::reference TokenIterator::operator*(void) const
{
    return token_;
}

inline TokenIterator::pointer TokenIterator::operator->(void) const
{
    return &token_;
}

inline bool TokenIterator::operator==(TokenIterator const &it) const
{
    return begin_ == it.begin_ && at_end_ == it.at_end_;
}

inline bool TokenIterator::operator!=(TokenIterator const &it) const
{
    return !(*this == it);
}

template <typename Predicate>
inline void TokenIterator::skip(Predicate pred)
{
    for ( ; begin_ != end_ && pred(*begin_); ++begin_) { /* do nothing */ }
}

template <typename Predicate>
inline bool TokenIterator::scan(Predicate pred)
{
    auto begin = begin_;
    skip(pred);

    bool scanned = begin != begin_;
    if (scanned) { token_.assign(begin, begin_); }

    return scanned;
}

} // namespace esa

#endif // ESAPP_TOK_HPP_
