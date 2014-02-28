/************************************************
 *  tok.cpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include "tok.hpp"

namespace esapp
{

/************************************************
 * Implementation: class TokenIterator
 ************************************************/

TokenIterator::TokenIterator(Sequence const &sequence)
    : TokenIterator(sequence.begin(), sequence.end())
{
    // do nothing
}

TokenIterator::TokenIterator(Iterator const &begin, Iterator const &end)
    : begin_(begin), end_(end), at_end_(begin_ == end_)
{
    if (!at_end_) { ++(*this); }
}

TokenIterator TokenIterator::end(void) const
{
    return TokenIterator(end_, end_);
}

bool TokenIterator::at_end(void) const
{
    return at_end_;
}

TokenIterator &TokenIterator::operator++(void)
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

TokenIterator TokenIterator::operator++(int)
{
    decltype((*this)++) it(*this);
    ++(*this);
    return it;
}

TokenIterator::reference TokenIterator::operator*(void) const
{
    return token_;
}

TokenIterator::pointer TokenIterator::operator->(void) const
{
    return &token_;
}

bool TokenIterator::operator==(TokenIterator const &it) const
{
    return begin_ == it.begin_ && at_end_ == it.at_end_;
}

bool TokenIterator::operator!=(TokenIterator const &it) const
{
    return !(*this == it);
}

} // namespace esa
