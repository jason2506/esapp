/************************************************
 *  trie.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef TRIE_HPP_INCLUDEED
#define TRIE_HPP_INCLUDEED

#include <algorithm>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#ifndef USE_GOOGLE_HASH_MAP
    #include <unordered_map>
#else
    #include <sparsehash/sparse_hash_map>
#endif

namespace esapp
{

/************************************************
 * Declaration: class FreqTrie
 ************************************************/

class FreqTrie
{
public: // Public Type(s)
    typedef wchar_t Char;
    typedef std::basic_string<Char> Sequence;

public: // Public Method(s)
    FreqTrie(size_t max_depth = 30, double smooth = 0.0, Char boundary = '\0');
    FreqTrie(FreqTrie const &trie);
    ~FreqTrie(void);

    FreqTrie &operator=(FreqTrie const &node);

    void increase(Sequence const &sequence, bool include_self = true);
    void increase(std::vector<Sequence> const &sequences, bool include_self = true);
    void decrease(Sequence const &sequence, bool include_self = true);
    void decrease(std::vector<Sequence> const &sequences, bool include_self = true);

    void update_hsp1(void);
    void update_fm(void);
    void update_iv(void);

    double get_hl(Sequence const &sequence) const;
    double get_hl(Sequence::const_iterator const &begin,
                  Sequence::const_iterator const &end) const;
    double get_hr(Sequence const &sequence) const;
    double get_hr(Sequence::const_iterator const &begin,
                  Sequence::const_iterator const &end) const;
    double get_iv(Sequence const &sequence) const;
    double get_iv(Sequence::const_iterator const &begin,
                  Sequence::const_iterator const &end) const;

    size_t depth(void) const;
    size_t max_depth(void) const;
    void clear(void);

private: // Private Type(s)
    struct FreqTrieNode;
    template<typename T> class BaseIterator;

    typedef FreqTrie::BaseIterator<FreqTrie::FreqTrieNode> Iterator;
    typedef FreqTrie::BaseIterator<FreqTrie::FreqTrieNode const> ConstIterator;

#ifndef USE_GOOGLE_HASH_MAP
    typedef std::unordered_map<Char, FreqTrie::FreqTrieNode *> NodeCollection;
    typedef std::unordered_map<Char, size_t> CharCounts;
#else
    typedef google::sparse_hash_map<Char, FreqTrie::FreqTrieNode *> NodeCollection;
    typedef google::sparse_hash_map<Char, size_t> CharCounts;
#endif

private: // Private Method(s)
    Iterator begin(void);
    ConstIterator begin(void) const;
    Iterator end(void);
    ConstIterator end(void) const;

    FreqTrieNode const *find(Sequence const &sequence) const;
    FreqTrieNode const *find(Sequence::const_iterator const &begin,
                             Sequence::const_iterator const &end) const;

    double entropy(CharCounts counts, size_t num_events);
    void clear_fm(void);

private: // Private Property(ies)
    FreqTrieNode *root_;
    double *freq_avg_;
    size_t max_depth_;
    double smooth_;
    Char boundary_;
}; // class FreqTrie

/************************************************
 * Declaration: class FreqTrie::Iterator
 ************************************************/

template<typename T>
class FreqTrie::BaseIterator
{
public: // Public Type(s)
    typedef std::pair<size_t, T *> Value;

public: // Public Method(s)
    BaseIterator(void);
    BaseIterator(T *node);

    BaseIterator &operator++(void);
    BaseIterator operator++(int);

    Value const &operator*(void) const;
    Value const *operator->(void) const;

    bool operator==(BaseIterator const &it) const;
    bool operator!=(BaseIterator const &it) const;

private: // Private Property(ies)
    std::stack<Value> stack_;
};

/************************************************
 * Declaration: struct FreqTrie::FreqTrieNode
 ************************************************/

struct FreqTrie::FreqTrieNode
{
    FreqTrieNode(void);
    FreqTrieNode(FreqTrieNode const &node);
    ~FreqTrieNode(void);

    FreqTrieNode &operator=(FreqTrieNode const &node);

    FreqTrieNode const *get(Char key) const;
    FreqTrieNode *get(Char key, bool create=false);
    size_t depth(void) const;
    void clear(void);

    FreqTrie::NodeCollection children;
    FreqTrie::CharCounts sp1l;
    FreqTrie::CharCounts sp1r;
    size_t f;
    double hl, hr, iv;
}; // struct FreqTrie::FreqTrieNode

} // namespace esa

#endif // TRIE_HPP_INCLUDEED
