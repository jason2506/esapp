/************************************************
 *  freq_trie.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2015, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_FREQ_TRIE_HPP_
#define ESAPP_FREQ_TRIE_HPP_

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
    struct data;
    typedef data *data_ptr;
    typedef data const *const_data_ptr;
    typedef wchar_t term_type;

public: // Public Method(s)
    freq_trie(void);

    template <typename Iterator>
    data_ptr insert(Iterator const &begin, Iterator const &end);

    template <typename Iterator>
    const_data_ptr find(Iterator const &begin, Iterator const &end) const;

    template <typename Iterator>
    void increase(Iterator const &begin, Iterator const &end);

    template <typename Iterator>
    void decrease(Iterator const &begin, Iterator const &end);

    void clear(void);

private: // Private Type(s)
    struct node;
    typedef node *raw_node_ptr;
    typedef node const *const_raw_node_ptr;
    typedef ::std::unique_ptr<node> node_ptr;
    typedef ::std::unordered_map<term_type, node_ptr> node_collection;

private: // Private Property(ies)
    node_ptr root_;
}; // class freq_trie

/************************************************
 * Declaration: struct freq_trie::data
 ************************************************/

struct freq_trie::data
{
    data(void);

    size_t f;
    double hl, hr;
}; // struct freq_trie::data

/************************************************
 * Declaration: struct freq_trie::node
 ************************************************/

struct freq_trie::node
{
    node(void);

    const_raw_node_ptr get(term_type key) const;
    raw_node_ptr get(term_type key, bool create = false);
    void clear(void);

    node_collection children;
    data node_data;
}; // struct freq_trie::node

/************************************************
 * Implementation: class freq_trie
 ************************************************/

inline freq_trie::freq_trie(void)
    : root_(new node())
{
    // do nothing
}

template <typename Iterator>
freq_trie::data_ptr freq_trie::insert(Iterator const &begin,
                                      Iterator const &end)
{
    auto node = root_.get();
    for (auto it = begin; it != end; ++it)
    {
        node = node->get(*it, true);
    }

    return &node->node_data;
}

template <typename Iterator>
freq_trie::const_data_ptr freq_trie::find(Iterator const &begin,
                                          Iterator const &end) const
{
    auto node = root_.get();
    for (auto it = begin; it != end; ++it)
    {
        node = node->get(*it);
        if (!node) { return nullptr; }
    }

    return &node->node_data;
}

template <typename Iterator>
void freq_trie::increase(Iterator const &begin, Iterator const &end)
{
    for (auto it_begin = begin; it_begin != end; ++it_begin)
    {
        auto node = root_.get();
        for (auto it = it_begin; it != end; ++it)
        {
            if (it_begin == begin && it + 1 == end) { continue; }

            node = node->get(*it);
            if (!node) { break; }

            node->node_data.f++;
        }
    }
}

template <typename Iterator>
void freq_trie::decrease(Iterator const &begin, Iterator const &end)
{
    for (auto it_begin = begin; it_begin != end; ++it_begin)
    {
        auto node = root_.get();
        for (auto it = it_begin; it != end; ++it)
        {
            if (it_begin == begin && it + 1 == end) { continue; }

            node = node->get(*it);
            if (!node) { break; }

            node->node_data.f--;
        }
    }
}

inline void freq_trie::clear(void)
{
    root_->clear();
}

/************************************************
 * Implementation: struct freq_trie::node
 ************************************************/

inline freq_trie::data::data(void)
    : f(0), hl(0), hr(0)
{
    // do nothing
}

/************************************************
 * Implementation: struct freq_trie::node
 ************************************************/

inline freq_trie::node::node(void)
    : node_data()
{
    // do nothing
}

inline freq_trie::const_raw_node_ptr freq_trie::node::get(term_type key) const
{
    auto it = children.find(key);
    return (it != children.end()) ? it->second.get() : nullptr;
}

inline freq_trie::raw_node_ptr freq_trie::node::get(term_type key, bool create)
{
    auto it = children.find(key);
    if (it != children.end())   { return it->second.get(); }
    else if (!create)           { return nullptr; }

    children.emplace(key, node_ptr(new node()));
    return children[key].get();
}

inline void freq_trie::node::clear(void)
{
    children.clear();
    node_data.f = node_data.hl = node_data.hr = 0;
}

} // namespace esapp

#endif // ESAPP_FREQ_TRIE_HPP_
