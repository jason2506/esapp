/************************************************
 *  freq_trie.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2017, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_INTERNAL_FREQ_TRIE_HPP_
#define ESAPP_INTERNAL_FREQ_TRIE_HPP_

#include <memory>
#include <unordered_map>
#include <utility>

namespace esapp {

/************************************************
 * Declaration: class freq_trie<T>
 ************************************************/

template <typename T>
class freq_trie {
 public:  // Public Type(s)
    struct node;
    using raw_node_ptr = node *;
    using const_raw_node_ptr = node const *;
    using term_type = T;
    using size_type = std::size_t;

 public:  // Public Method(s)
    freq_trie();

    raw_node_ptr get_root();
    const_raw_node_ptr get_root() const;

    template <typename Iterator>
    const_raw_node_ptr find(Iterator const &begin, Iterator const &end) const;

    template <typename Iterator>
    void decrease(Iterator const &begin, Iterator const &end);

    void clear();

 private:  // Private Type(s)
    using node_ptr = std::unique_ptr<node>;
    using node_collection = std::unordered_map<term_type, node_ptr>;

 private:  // Private Property(ies)
    node_ptr root_;
};  // class freq_trie<T>

/************************************************
 * Declaration: struct freq_trie<T>::node
 ************************************************/

template <typename T>
struct freq_trie<T>::node {
    node();

    const_raw_node_ptr get(term_type key) const;
    raw_node_ptr get(term_type key);
    std::pair<bool, raw_node_ptr> get_or_create(term_type key);
    void clear();

    node_collection children;
    size_type f, avl, avr;
};  // struct freq_trie<T>::node

/************************************************
 * Implementation: class freq_trie<T>
 ************************************************/

template <typename T>
inline freq_trie<T>::freq_trie()
    : root_(new node()) {
    // do nothing
}

template <typename T>
inline typename freq_trie<T>::raw_node_ptr freq_trie<T>::get_root() {
    return root_.get();
}

template <typename T>
inline typename freq_trie<T>::const_raw_node_ptr freq_trie<T>::get_root() const {
    return root_.get();
}

template <typename T>
template <typename Iterator>
typename freq_trie<T>::const_raw_node_ptr freq_trie<T>::find(
        Iterator const &begin, Iterator const &end) const {
    auto node = root_.get();
    for (auto it = begin; it != end; ++it) {
        node = node->get(*it);
        if (!node) { return nullptr; }
    }

    return node;
}

template <typename T>
template <typename Iterator>
void freq_trie<T>::decrease(Iterator const &begin, Iterator const &end) {
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

template <typename T>
inline void freq_trie<T>::clear() {
    root_->clear();
}

/************************************************
 * Implementation: struct freq_trie<T>::node
 ************************************************/

template <typename T>
inline freq_trie<T>::node::node()
    : f(1), avl(1), avr(1) {
    // do nothing
}

template <typename T>
inline typename freq_trie<T>::const_raw_node_ptr freq_trie<T>::node::get(term_type key) const {
    auto it = children.find(key);
    return (it == children.end()) ? nullptr : it->second.get();
}

template <typename T>
inline typename freq_trie<T>::raw_node_ptr freq_trie<T>::node::get(term_type key) {
    auto it = children.find(key);
    return (it == children.end()) ? nullptr : it->second.get();
}

template <typename T>
inline std::pair<bool, typename freq_trie<T>::raw_node_ptr>
freq_trie<T>::node::get_or_create(term_type key) {
    auto ptr = get(key);
    if (ptr) {
        return {false, ptr};
    }

    children.emplace(key, node_ptr(new node()));
    return {true, children[key].get()};
}

template <typename T>
inline void freq_trie<T>::node::clear() {
    children.clear();
    f = avl = avr = 0;
}

}  // namespace esapp

#endif  // ESAPP_INTERNAL_FREQ_TRIE_HPP_
