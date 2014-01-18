#ifndef TRIE_HPP_INCLUDEED
#define TRIE_HPP_INCLUDEED

#include <algorithm>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#ifndef USE_GOOGLE_HASH_MAP
    #include <tr1/unordered_map>
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
    FreqTrie(const FreqTrie &trie);
    ~FreqTrie(void);

    FreqTrie &operator=(const FreqTrie &node);

    void increase(const Sequence &sequence, bool include_self = true);
    void increase(const std::vector<Sequence> &sequences, bool include_self = true);
    void decrease(const Sequence &sequence, bool include_self = true);
    void decrease(const std::vector<Sequence> &sequences, bool include_self = true);

    void update_hsp1(void);
    void update_fm(void);
    void update_iv(void);

    double get_hl(const Sequence &sequence) const;
    double get_hl(const Sequence::const_iterator &begin,
                  const Sequence::const_iterator &end) const;
    double get_hr(const Sequence &sequence) const;
    double get_hr(const Sequence::const_iterator &begin,
                  const Sequence::const_iterator &end) const;
    double get_iv(const Sequence &sequence) const;
    double get_iv(const Sequence::const_iterator &begin,
                  const Sequence::const_iterator &end) const;

    size_t depth(void) const;
    size_t max_depth(void) const;
    void clear(void);

private: // Private Type(s)
    struct FreqTrieNode;
    template<typename T> class BaseIterator;

    typedef FreqTrie::BaseIterator<FreqTrie::FreqTrieNode> Iterator;
    typedef FreqTrie::BaseIterator<const FreqTrie::FreqTrieNode> ConstIterator;

#ifndef USE_GOOGLE_HASH_MAP
    typedef std::tr1::unordered_map<Char, FreqTrie::FreqTrieNode *> NodeCollection;
    typedef std::tr1::unordered_map<Char, size_t> CharCounts;
#else
    typedef google::sparse_hash_map<Char, FreqTrie::FreqTrieNode *> NodeCollection;
    typedef google::sparse_hash_map<Char, size_t> CharCounts;
#endif

private: // Private Method(s)
    Iterator begin(void);
    ConstIterator begin(void) const;
    Iterator end(void);
    ConstIterator end(void) const;

    FreqTrieNode const *find(const Sequence &sequence) const;
    FreqTrieNode const *find(const Sequence::const_iterator &begin,
                             const Sequence::const_iterator &end) const;

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

    Value &operator*(void) const;
    Value const *operator->(void) const;

    bool operator==(const BaseIterator &it) const;
    bool operator!=(const BaseIterator &it) const;

private: // Private Property(ies)
    std::stack<Value> stack_;
};

/************************************************
 * Declaration: struct FreqTrie::FreqTrieNode
 ************************************************/

struct FreqTrie::FreqTrieNode
{
    FreqTrieNode(void);
    FreqTrieNode(const FreqTrieNode &node);
    ~FreqTrieNode(void);

    FreqTrieNode &operator=(const FreqTrieNode &node);

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
