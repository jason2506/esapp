/************************************************
 *  count.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_COUNT_HPP_
#define ESAPP_COUNT_HPP_

#include <cmath>
#include <algorithm>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "sufarr.hpp"
#include "trie.hpp"

namespace esapp
{

/************************************************
 * Declaration: class StringCounter
 ************************************************/

class StringCounter
{
public: // Public Type(s)
    typedef wchar_t Term;
    typedef std::basic_string<Term> Sequence;

public: // Public Method(s)
    StringCounter(size_t max_len = 30, double smooth = 0.0, Term boundary = 0);

    void fit(std::vector<Sequence> const &sequences);

    void set_pres(std::vector<size_t> pres, size_t p, size_t n);
    void unset_pres(std::vector<size_t> pres, size_t p, size_t n);

    double get_iv(size_t i, size_t n) const;
    double get_hl(size_t i, size_t n) const;
    double get_hr(size_t i, size_t n) const;

    void clear(void);

private: // Private Type(s)
    typedef SuffixArray::Term TermId;
    typedef std::vector<TermId> IdSequence;
    typedef std::unordered_map<TermId, size_t> TermCounts;

private: // Private Method(s)
    IdSequence init_char_id_map(Sequence const &s);
    IdSequence to_char_ids(Sequence const &s) const;

    void calc_avg(void);

    double entropy(TermCounts const &counts) const;

private: // Private Property(ies)
    size_t max_len_;
    double smooth_;
    Term boundary_;
    double h1_;

    std::vector<double> f_avgs_;
    std::vector<double> hl_avgs_;
    std::vector<double> hr_avgs_;
    std::vector<size_t> str_nums_;

    std::vector<size_t> count_min_lens_;
    std::unordered_map<Term, TermId> char_id_map_;

    FreqTrie trie_;
    SuffixArray sa_;

    static const TermId BOUNDARY_;
}; // class StringCounter

/************************************************
 * Implementation: class StringCounter
 ************************************************/

inline void StringCounter::clear(void)
{
    sa_.clear();
}

} // namespace esapp

#endif // ESAPP_COUNT_HPP_
