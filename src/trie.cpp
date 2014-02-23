/************************************************
 *  trie.cpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include "trie.hpp"

namespace esapp
{

/************************************************
 * Implementation: class FreqTrie
 ************************************************/

FreqTrie::FreqTrie(size_t max_depth, double smooth, Char boundary)
    : freq_avg_(nullptr), max_depth_(max_depth),
      smooth_(smooth), boundary_(boundary)
{
    root_ = new FreqTrieNode();
}

FreqTrie::FreqTrie(FreqTrie const &trie)
    : freq_avg_(nullptr), max_depth_(trie.max_depth_),
      smooth_(trie.smooth_), boundary_(trie.boundary_)
{
    root_ = new FreqTrieNode(*(trie.root_));
    if (trie.freq_avg_)
    {
        auto trie_depth = trie.depth();
        freq_avg_ = new double[trie_depth];
        memcpy(freq_avg_, trie.freq_avg_, sizeof(double) * trie_depth);
    }
}

FreqTrie::~FreqTrie(void)
{
    clear_fm();
    delete root_;
}

FreqTrie &FreqTrie::operator=(FreqTrie const &trie)
{
    *root_ = *(trie.root_);

    clear_fm();
    if (trie.freq_avg_)
    {
        auto trie_depth = trie.depth();
        freq_avg_ = new double[trie_depth];
        memcpy(freq_avg_, trie.freq_avg_, sizeof(double) * trie_depth);
    }

    return *this;
}

void FreqTrie::increase(Sequence const &sequence, bool include_self)
{
    auto n = sequence.size();
    for (decltype(n) i = 0; i < n; ++i)
    {
        auto *node = root_;
        for (decltype(n) j = i; j < n && j - i < max_depth_; ++j)
        {
            if (!include_self && i == 0 && j == n - 1) { continue; }

            node = node->get(sequence[j], true);
            node->f++;

            auto x = (i > 0)     ? sequence[i - 1] : boundary_;
            auto y = (j < n - 1) ? sequence[j + 1] : boundary_;
            node->sp1l[x]++;
            node->sp1r[y]++;
        }
    }
}

void FreqTrie::increase(std::vector<Sequence> const &sequences,
                        bool include_self)
{
    for (auto const &sequence : sequences)
    {
        increase(sequence, include_self);
    }
}

void FreqTrie::decrease(Sequence const &sequence, bool include_self)
{
    auto n = sequence.size();
    for (decltype(n) i = 0; i < n; ++i)
    {
        auto *node = root_;
        for (decltype(n) j = i; j < n && j - i < max_depth_; ++j)
        {
            if (!include_self && i == 0 && j == n - 1) { continue; }

            node = &(node->children[sequence[j]]);
            node->f--;
        }
    }
}

void FreqTrie::decrease(std::vector<Sequence> const &sequences,
                        bool include_self)
{
    for (auto const &sequence : sequences)
    {
        decrease(sequence, include_self);
    }
}

void FreqTrie::update_hsp1(void)
{
    auto trie_depth = depth();
    auto num_alphabets = root_->children.size() + 1;

    auto *nums = new size_t[trie_depth];
    auto *hl_avg = new double[trie_depth];
    auto *hr_avg = new double[trie_depth];

    memset(nums, 0, sizeof(size_t) * trie_depth);
    memset(hl_avg, 0, sizeof(double) * trie_depth);
    memset(hr_avg, 0, sizeof(double) * trie_depth);

    for (auto const &node : *this)
    {
        auto node_depth = node.first - 1;
        node.second->hl = entropy(node.second->sp1l, num_alphabets);
        node.second->hr = entropy(node.second->sp1r, num_alphabets);
        hl_avg[node_depth] += node.second->hl;
        hr_avg[node_depth] += node.second->hr;
        nums[node_depth]++;
    }

    for (decltype(trie_depth) i = 0; i < trie_depth; ++i)
    {
        hl_avg[i] /= nums[i];
        hr_avg[i] /= nums[i];
    }

    for (auto const &node : *this)
    {
        auto node_depth = node.first - 1;
        node.second->hl /= hl_avg[node_depth];
        node.second->hr /= hr_avg[node_depth];
    }

    delete [] nums;
    delete [] hl_avg;
    delete [] hr_avg;
}

void FreqTrie::update_fm(void)
{
    clear_fm();

    auto trie_depth = depth();
    auto *nums = new size_t[trie_depth];
    freq_avg_ = new double[trie_depth];

    memset(nums, 0, sizeof(size_t) * trie_depth);
    memset(freq_avg_, 0, sizeof(double) * trie_depth);

    for (auto const &node : *this)
    {
        auto node_depth = node.first - 1;
        freq_avg_[node_depth] += node.second->f;
        nums[node_depth]++;
    }

    for (decltype(trie_depth) i = 0; i < trie_depth; ++i)
    {
        freq_avg_[i] /= nums[i];
    }

    delete [] nums;
}

void FreqTrie::update_iv(void)
{
    if (freq_avg_ == nullptr) { throw std::exception(); }

    for (auto const &node : *this)
    {
        auto node_depth = node.first - 1;
        node.second->iv = pow(node.second->f / freq_avg_[node_depth],
                              node_depth + 1);
    }
}

double FreqTrie::get_hl(Sequence const &sequence) const
{
    return get_hl(sequence.begin(), sequence.end());
}

double FreqTrie::get_hl(Sequence::const_iterator const &begin,
                        Sequence::const_iterator const &end) const
{
    auto const *node = find(begin, end);
    if (node == nullptr) { return -1; }

    return node->hl;
}

double FreqTrie::get_hr(Sequence const &sequence) const
{
    return get_hr(sequence.begin(), sequence.end());
}

double FreqTrie::get_hr(Sequence::const_iterator const &begin,
                        Sequence::const_iterator const &end) const
{
    auto const *node = find(begin, end);
    if (node == nullptr) { return -1; }

    return node->hr;
}

double FreqTrie::get_iv(Sequence const &sequence) const
{
    return get_iv(sequence.begin(), sequence.end());
}

double FreqTrie::get_iv(Sequence::const_iterator const &begin,
                        Sequence::const_iterator const &end) const
{
    auto const *node = find(begin, end);
    if (node == nullptr) { return -1; }

    return node->iv;
}

size_t FreqTrie::depth(void) const
{
    return root_->depth();
}

size_t FreqTrie::max_depth(void) const
{
    return max_depth_;
}

FreqTrie::Iterator FreqTrie::begin(void)
{
    return ++Iterator(root_);
}

FreqTrie::ConstIterator FreqTrie::begin(void) const
{
    return ++ConstIterator(root_);
}

FreqTrie::Iterator FreqTrie::end(void)
{
    return Iterator();
}

FreqTrie::ConstIterator FreqTrie::end(void) const
{
    return ConstIterator();
}

FreqTrie::FreqTrieNode const *FreqTrie::find(Sequence const &sequence) const
{
    return find(sequence.begin(), sequence.end());
}

FreqTrie::FreqTrieNode const *FreqTrie::find(
    Sequence::const_iterator const &begin,
    Sequence::const_iterator const &end) const
{
    auto *node = root_;
    for (auto it = begin; it != end; ++it)
    {
        node = node->get(*it);
        if (node == nullptr) { break; }
    }

    return node;
}

double FreqTrie::entropy(CharCounts counts, size_t num_events)
{
    auto n = num_events * smooth_;
    for (auto const &count : counts)
    {
        n += count.second;
    }

    auto h = 0.0;
    for (auto const &count : counts)
    {
        auto p = (count.second + smooth_) / n;
        h -= p * log(p);
    }

    if (smooth_ > 0)
    {
        auto num_zeros = num_events - counts.size();
        auto p = smooth_ / n;
        h -= num_zeros * (p * log(p));
    }

    return h;
}

void FreqTrie::clear_fm(void)
{
    if (freq_avg_ != nullptr)
    {
        delete [] freq_avg_;
    }

    freq_avg_ = nullptr;
}

void FreqTrie::clear(void)
{
    root_->clear();
    clear_fm();
}

/************************************************
 * Implementation: class FreqTrie::BaseIterator
 ************************************************/

template<typename T>
FreqTrie::BaseIterator<T>::BaseIterator(void)
    : stack_()
{
    // do nothing
}

template<typename T>
FreqTrie::BaseIterator<T>::BaseIterator(T *node)
    : stack_()
{
    stack_.push(std::make_pair(0, node));
}

template<typename T>
FreqTrie::BaseIterator<T> &FreqTrie::BaseIterator<T>::operator++(void)
{
    auto top = stack_.top();
    stack_.pop();

    auto depth = top.first + 1;
    auto const *node = top.second;
    for (auto const &child : node->children)
    {
        auto node_ptr = const_cast<T *>(&(child.second));
        stack_.push(std::make_pair(depth, node_ptr));
    }

    return *this;
}

template<typename T>
FreqTrie::BaseIterator<T> FreqTrie::BaseIterator<T>::operator++(int)
{
    decltype((*this)++) it(*this);
    ++(*this);
    return it;
}

template<typename T>
typename FreqTrie::BaseIterator<T>::Value const &
FreqTrie::BaseIterator<T>::operator*(void) const
{
    return stack_.top();
}

template<typename T>
typename FreqTrie::BaseIterator<T>::Value const *
FreqTrie::BaseIterator<T>::operator->(void) const
{
    return &(stack_.top());
}

template<typename T>
bool FreqTrie::BaseIterator<T>::operator==(BaseIterator const &it) const
{
    if (it.stack_.empty())      { return stack_.empty(); }
    else if (stack_.empty())    { return false; }

    return stack_.top() == it.stack_.top();
}

template<typename T>
bool FreqTrie::BaseIterator<T>::operator!=(BaseIterator const &it) const
{
    return !(*this == it);
}

/************************************************
 * Implementation: struct FreqTrie::FreqTrieNode
 ************************************************/

FreqTrie::FreqTrieNode::FreqTrieNode(void) : f(0), hl(0), hr(0), iv(0)
{
    // do nothing
}

FreqTrie::FreqTrieNode::~FreqTrieNode(void)
{
    clear();
}

FreqTrie::FreqTrieNode const *FreqTrie::FreqTrieNode::get(Char key) const
{
    auto it = children.find(key);
    if (it != children.end())
    {
        return &(it->second);
    }

    return nullptr;
}

FreqTrie::FreqTrieNode *FreqTrie::FreqTrieNode::get(Char key, bool create)
{
    auto it = children.find(key);
    if (it != children.end())   { return &(it->second); }
    else if (!create)           { return nullptr; }

    children.emplace(key);
    return &(children[key]);
}

size_t FreqTrie::FreqTrieNode::depth(void) const
{
    if (children.empty()) { return 0; }

    decltype(depth()) max_depth = 0;
    for (auto const &child : children)
    {
        auto depth = child.second.depth();
        if (depth > max_depth)
        {
            max_depth = depth;
        }
    }

    return max_depth + 1;
}

void FreqTrie::FreqTrieNode::clear(void)
{
    children.clear();
    f = hl = hr = iv = 0;
}

} // namespace esa
