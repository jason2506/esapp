#ifndef SEG_HPP_INCLUDEED
#define SEG_HPP_INCLUDEED

#include <cmath>
#include <vector>
#include <string>

#include "trie.hpp"

namespace esa
{

/************************************************
 * Declaration: class Segmenter
 ************************************************/

class Segmenter
{
public: // Public Type(s)
    typedef FreqTrie::Sequence Sequence;

public: // Public Method(s)
    Segmenter(double lrv_exp, size_t max_iters = 10,
              size_t max_length = 30, double smooth = 0.0);

    void fit(const std::vector<Sequence> &sequences);
    std::vector<Sequence> segment(const Sequence &sequence) const;

private: // Private Type(s)
    typedef std::pair<double, std::vector<size_t> > SegResult;

private: // Private Method(s)
    std::vector<Sequence> segment(const Sequence &sequence,
                                  const std::vector<size_t> &seg) const;
    SegResult segment(const Sequence::const_iterator &begin,
                      const Sequence::const_iterator &end) const;

private: // Private Property(ies)
    FreqTrie trie_;
    double lrv_exp_;
    size_t max_iters_;
}; // class Segmenter

} // namespace esa

#endif // SEG_HPP_INCLUDEED
