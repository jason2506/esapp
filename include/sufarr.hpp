/************************************************
 *  sufarr.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_SUFARR_HPP_
#define ESAPP_SUFARR_HPP_

#include <cstdlib>
#include <limits>
#include <algorithm>
#include <type_traits>
#include <vector>

namespace esapp
{

/************************************************
 * Declaration: class SuffixArray
 ************************************************/

class SuffixArray
{
public: // Public Type(s) - Part 1
    typedef unsigned short Term;
    typedef std::vector<Term> Sequence;

    typedef typename std::vector<size_t>::const_iterator Iterator;
    typedef typename std::vector<size_t>::const_iterator ConstIterator;

public: // Public Method(s)
    SuffixArray(void);
    SuffixArray(Sequence const &s);
    SuffixArray(Sequence const &s, size_t num_alphas);

    void construct(Sequence const &s);
    void construct(Sequence const &s, size_t num_alphas);
    void clear(void);

    size_t size(void) const;
    bool empty(void) const;

    Sequence const &data(void) const;
    Term term(size_t i) const;
    size_t rank(size_t i) const;
    size_t lcp(size_t i) const;
    size_t at(size_t i) const;
    size_t operator[](size_t idx) const;

    ConstIterator begin(void) const;
    ConstIterator end(void) const;

    ConstIterator find(Sequence const &t) const;
    size_t count(Sequence const &t) const;
    ConstIterator lower_bound(Sequence const &t) const;
    ConstIterator upper_bound(Sequence const &t) const;
    std::pair<ConstIterator, ConstIterator> equal_range(Sequence const &t) const;

private: // Private Method(s)
    void construct(void);
    void construct(size_t num_alphas);

    size_t calc_lcp(Sequence const &t, size_t i) const;
    bool is_lms(std::vector<bool> const &suf_types, size_t i, size_t len_s);

    template <typename T>
    void init_bkt(T const &s, std::vector<size_t> &bkt,
                  size_t num_chars, size_t num_alphas, bool end);

    template <typename T>
    void induce(T const &s, std::vector<bool> const &suf_types,
                std::vector<size_t> &sa, std::vector<size_t> &bkt,
                size_t num_chars, size_t num_alphas);

    template <typename T>
    void gen_sa(T const &s, std::vector<size_t> &sa, std::vector<size_t> &bkt,
                size_t num_chars, size_t num_alphas);

    void gen_isa(void);
    void gen_lcpa(void);

private: // Private Property(ies)
    Sequence s_;
    std::vector<size_t> sa_;
    std::vector<size_t> isa_;
    std::vector<size_t> lcpa_;
}; // class SuffixArray

/************************************************
 * Implementation: class SuffixArray
 ************************************************/

inline void SuffixArray::clear(void)
{
    s_.clear();
    sa_.clear();
    isa_.clear();
    lcpa_.clear();
}

inline size_t SuffixArray::size(void) const
{
    return sa_.size();
}

inline bool SuffixArray::empty(void) const
{
    return sa_.empty();
}

inline SuffixArray::Sequence const &SuffixArray::data(void) const
{
    return s_;
}

inline SuffixArray::Term SuffixArray::term(size_t i) const
{
    return s_[i];
}

inline size_t SuffixArray::rank(size_t i) const
{
    return isa_[i];
}

inline size_t SuffixArray::lcp(size_t i) const
{
    return lcpa_[i];
}

inline size_t SuffixArray::at(size_t i) const
{
    return sa_[i];
}

inline size_t SuffixArray::operator[](size_t i) const
{
    return sa_[i];
}

inline SuffixArray::ConstIterator SuffixArray::begin(void) const
{
    return sa_.begin();
}

inline SuffixArray::ConstIterator SuffixArray::end(void) const
{
    return sa_.end();
}

inline size_t SuffixArray::count(Sequence const &t) const
{
    return upper_bound(t) - lower_bound(t);
}

inline std::pair<SuffixArray::ConstIterator, SuffixArray::ConstIterator>
SuffixArray::equal_range(Sequence const &t) const
{
    return make_pair(lower_bound(t), upper_bound(t));
}

} // namespace esapp

#endif // ESAPP_SUFARR_HPP_
