/************************************************
 *  trie.hpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_TRIE_HPP_
#define ESAPP_TRIE_HPP_

#include <unordered_map>
#include <utility>
#include <memory>
#include <string>
#include <vector>

namespace esapp
{

/************************************************
 * Declaration: class freq_trie
 ************************************************/

class freq_trie
{
public: // Public Type(s)
    typedef wchar_t term_type;

    struct node;
    typedef std::shared_ptr<node> node_pointer;

public: // Public Method(s)
    freq_trie(void);
    freq_trie(freq_trie const &trie);

    template <typename Iterator>
    node_pointer insert(Iterator const &begin, Iterator const &end);

    template <typename Iterator>
    node_pointer const find(Iterator const &begin, Iterator const &end) const;

    template <typename Iterator>
    void increase(Iterator const &begin, Iterator const &end);

    template <typename Iterator>
    void decrease(Iterator const &begin, Iterator const &end);

    freq_trie &operator=(freq_trie const &node);

    void clear(void);

private: // Private Type(s)
    typedef std::unordered_map<term_type, node_pointer> NodeCollection;

private: // Private Property(ies)
    node_pointer root_;
}; // class freq_trie

/************************************************
 * Declaration: struct freq_trie::node
 ************************************************/

struct freq_trie::node
{
    node(void);

    freq_trie::node_pointer const get(term_type key) const;
    freq_trie::node_pointer get(term_type key, bool create = false);
    void clear(void);

    freq_trie::NodeCollection children;
    size_t f;
    double hl, hr;
}; // struct freq_trie::node

/************************************************
 * Implementation: class freq_trie
 ************************************************/

inline freq_trie::freq_trie(void)
    : root_(new node())
{
    // do nothing
}

inline freq_trie::freq_trie(freq_trie const &trie)
    : root_(new node(*(trie.root_)))
{
    // do nothing
}

inline freq_trie &freq_trie::operator=(freq_trie const &trie)
{
    *root_ = *(trie.root_);
    return *this;
}

template <typename Iterator>
freq_trie::node_pointer freq_trie::insert(Iterator const &begin,
                                          Iterator const &end)
{
    auto node = root_;
    for (auto it = begin; it != end; ++it)
    {
        node = node->get(*it, true);
    }

    return node;
}

template <typename Iterator>
freq_trie::node_pointer const freq_trie::find(Iterator const &begin,
                                              Iterator const &end) const
{
    auto node = root_;
    for (auto it = begin; it != end; ++it)
    {
        node = node->get(*it);
        if (!node) { break; }
    }

    return node;
}

template <typename Iterator>
void freq_trie::increase(Iterator const &begin, Iterator const &end)
{
    for (auto it_begin = begin; it_begin != end; ++it_begin)
    {
        auto node = root_;
        for (auto it = it_begin; it != end; ++it)
        {
            if (it_begin == begin && it + 1 == end) { continue; }

            node = node->get(*it);
            if (!node) { break; }

            node->f++;
        }
    }
}

template <typename Iterator>
void freq_trie::decrease(Iterator const &begin, Iterator const &end)
{
    for (auto it_begin = begin; it_begin != end; ++it_begin)
    {
        auto node = root_;
        for (auto it = it_begin; it != end; ++it)
        {
            if (it_begin == begin && it + 1 == end) { continue; }

            node = node->get(*it);
            if (!node) { break; }

            node->f--;
        }
    }
}

inline void freq_trie::clear(void)
{
    root_->clear();
}

/************************************************
 * Implementation: struct freq_trie::freq_trieNode
 ************************************************/

inline freq_trie::node::node(void)
    : f(0), hl(0), hr(0)
{
    // do nothing
}

inline freq_trie::node_pointer const freq_trie::node::get(term_type key) const
{
    auto it = children.find(key);
    return (it != children.end()) ? it->second : node_pointer();
}

inline freq_trie::node_pointer freq_trie::node::get(term_type key, bool create)
{
    auto it = children.find(key);
    if (it != children.end())   { return it->second; }
    else if (!create)           { return node_pointer(); }

    children.emplace(key, node_pointer(new node()));
    return children[key];
}

inline void freq_trie::node::clear(void)
{
    children.clear();
    f = hl = hr = 0;
}

} // namespace esapp

#endif // ESAPP_TRIE_HPP_
