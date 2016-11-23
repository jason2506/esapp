/************************************************
 *  freq_trie.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_FREQ_TRIE_HPP_
#define ESAPP_FREQ_TRIE_HPP_

#include <unordered_map>
#include <utility>
#include <memory>
#include <string>
#include <vector>

namespace esapp {

/************************************************
 * Declaration: class freq_trie
 ************************************************/

class freq_trie {
 public:  // Public Type(s)
    struct node;
    typedef node *raw_node_ptr;
    typedef node const *const_raw_node_ptr;
    typedef std::uint16_t term_type;
    typedef std::size_t size_type;

 public:  // Public Method(s)
    freq_trie();

    raw_node_ptr get_root();
    const_raw_node_ptr get_root() const;

    template <typename Iterator>
    const_raw_node_ptr find(Iterator const &begin, Iterator const &end) const;

    template <typename Iterator>
    void increase(Iterator const &begin, Iterator const &end);

    template <typename Iterator>
    void decrease(Iterator const &begin, Iterator const &end);

    void clear();

 private:  // Private Type(s)
    typedef std::unique_ptr<node> node_ptr;
    typedef std::unordered_map<term_type, node_ptr> node_collection;

 private:  // Private Property(ies)
    node_ptr root_;
};  // class freq_trie

/************************************************
 * Declaration: struct freq_trie::node
 ************************************************/

struct freq_trie::node {
    node();

    const_raw_node_ptr get(term_type key) const;
    raw_node_ptr get(term_type key, bool create = false);
    void clear();

    node_collection children;
    size_type f, avl, avr;
};  // struct freq_trie::node

/************************************************
 * Implementation: class freq_trie
 ************************************************/

inline freq_trie::freq_trie()
    : root_(new node()) {
    // do nothing
}

inline freq_trie::raw_node_ptr freq_trie::get_root() {
    return root_.get();
}

inline freq_trie::const_raw_node_ptr freq_trie::get_root() const {
    return root_.get();
}

template <typename Iterator>
freq_trie::const_raw_node_ptr freq_trie::find(Iterator const &begin,
                                              Iterator const &end) const {
    auto node = root_.get();
    for (auto it = begin; it != end; ++it) {
        node = node->get(*it);
        if (!node) { return nullptr; }
    }

    return node;
}

template <typename Iterator>
void freq_trie::increase(Iterator const &begin, Iterator const &end) {
    for (auto it_begin = begin; it_begin != end; ++it_begin) {
        auto node = root_.get();
        for (auto it = it_begin; it != end; ++it) {
            if (it_begin == begin && it + 1 == end) { break; }

            node = node->get(*it);
            if (!node) { break; }

            node->f++;
        }
    }
}

template <typename Iterator>
void freq_trie::decrease(Iterator const &begin, Iterator const &end) {
    for (auto it_begin = begin; it_begin != end; ++it_begin) {
        auto node = root_.get();
        for (auto it = it_begin; it != end; ++it) {
            if (it_begin == begin && it + 1 == end) { break; }

            node = node->get(*it);
            if (!node) { break; }

            node->f--;
        }
    }
}

inline void freq_trie::clear() {
    root_->clear();
}

/************************************************
 * Implementation: struct freq_trie::node
 ************************************************/

inline freq_trie::node::node()
    : f(1), avl(1), avr(1) {
    // do nothing
}

inline freq_trie::const_raw_node_ptr freq_trie::node::get(term_type key) const {
    auto it = children.find(key);
    return (it != children.end()) ? it->second.get() : nullptr;
}

inline freq_trie::raw_node_ptr freq_trie::node::get(term_type key, bool create) {
    auto it = children.find(key);
    if (it != children.end()) {
        return it->second.get();
    } else if (!create) {
        return nullptr;
    }

    children.emplace(key, node_ptr(new node()));
    return children[key].get();
}

inline void freq_trie::node::clear() {
    children.clear();
    f = avl = avr = 0;
}

}  // namespace esapp

#endif  // ESAPP_FREQ_TRIE_HPP_
