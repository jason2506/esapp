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
 * Declaration: class FreqTrie
 ************************************************/

class FreqTrie
{
public: // Public Type(s)
    typedef wchar_t Term;

    struct Node;
    typedef std::shared_ptr<Node> NodePtr;

public: // Public Method(s)
    FreqTrie(void);
    FreqTrie(FreqTrie const &trie);

    template <typename Iterator>
    NodePtr insert(Iterator const &begin, Iterator const &end);

    template <typename Iterator>
    NodePtr const find(Iterator const &begin, Iterator const &end) const;

    template <typename Iterator>
    void increase(Iterator const &begin, Iterator const &end);

    template <typename Iterator>
    void decrease(Iterator const &begin, Iterator const &end);

    FreqTrie &operator=(FreqTrie const &node);

    void clear(void);

private: // Private Type(s)
    typedef std::unordered_map<Term, NodePtr> NodeCollection;

private: // Private Property(ies)
    NodePtr root_;
}; // class FreqTrie

/************************************************
 * Declaration: struct FreqTrie::Node
 ************************************************/

struct FreqTrie::Node
{
    Node(void);

    FreqTrie::NodePtr const get(Term key) const;
    FreqTrie::NodePtr get(Term key, bool create = false);
    void clear(void);

    FreqTrie::NodeCollection children;
    size_t f;
    double hl, hr;
}; // struct FreqTrie::Node

/************************************************
 * Implementation: class FreqTrie
 ************************************************/

inline FreqTrie::FreqTrie(void)
    : root_(new Node())
{
    // do nothing
}

inline FreqTrie::FreqTrie(FreqTrie const &trie)
    : root_(new Node(*(trie.root_)))
{
    // do nothing
}

inline FreqTrie &FreqTrie::operator=(FreqTrie const &trie)
{
    *root_ = *(trie.root_);
    return *this;
}

template <typename Iterator>
FreqTrie::NodePtr FreqTrie::insert(Iterator const &begin, Iterator const &end)
{
    auto node = root_;
    for (auto it = begin; it != end; ++it)
    {
        node = node->get(*it, true);
    }

    return node;
}

template <typename Iterator>
FreqTrie::NodePtr const FreqTrie::find(Iterator const &begin, Iterator const &end) const
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
void FreqTrie::increase(Iterator const &begin, Iterator const &end)
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
void FreqTrie::decrease(Iterator const &begin, Iterator const &end)
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

inline void FreqTrie::clear(void)
{
    root_->clear();
}

/************************************************
 * Implementation: struct FreqTrie::FreqTrieNode
 ************************************************/

inline FreqTrie::Node::Node(void)
    : f(0), hl(0), hr(0)
{
    // do nothing
}

inline FreqTrie::NodePtr const FreqTrie::Node::get(Term key) const
{
    auto it = children.find(key);
    return (it != children.end()) ? it->second : NodePtr();
}

inline FreqTrie::NodePtr FreqTrie::Node::get(Term key, bool create)
{
    auto it = children.find(key);
    if (it != children.end())   { return it->second; }
    else if (!create)           { return NodePtr(); }

    children.emplace(key, NodePtr(new Node()));
    return children[key];
}

inline void FreqTrie::Node::clear(void)
{
    children.clear();
    f = hl = hr = 0;
}

} // namespace esapp

#endif // ESAPP_TRIE_HPP_
