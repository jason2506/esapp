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

    typedef std::input_iterator_tag iterator_category;
    typedef Sequence value_type;
    typedef ptrdiff_t difference_type;
    typedef value_type const *pointer;
    typedef value_type const &reference;

private: // Private Type(s)
    typedef Sequence::const_iterator Iterator;

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
    void skip(Predicate pred)
    {
        for ( ; begin_ != end_ && pred(*begin_); ++begin_) { /* do nothing */ }
    }

    template <typename Predicate>
    bool scan(Predicate pred)
    {
        auto begin = begin_;
        skip(pred);

        bool scanned = begin != begin_;
        if (scanned)
        {
            token_.assign(begin, begin_);
        }

        return scanned;
    }

private: // Private Property(ies)
    Iterator begin_;
    Iterator end_;
    Sequence token_;
    bool at_end_;
}; // class TokenIterator

} // namespace esa

#endif // ESAPP_TOK_HPP_
