#ifndef SEG_HPP_INCLUDEED
#define SEG_HPP_INCLUDEED

#include <cmath>
#include <string>
#include <vector>

#include "trie.hpp"

namespace esapp
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

    std::vector<std::vector<Sequence> > fit_and_segment(const std::vector<Sequence> &sequences);
    void fit(const std::vector<Sequence> &sequences);
    std::vector<std::vector<Sequence> > segment(const std::vector<Sequence> &sequences) const;
    std::vector<Sequence> segment(const Sequence &sequence) const;

private: // Private Type(s)
    typedef std::vector<size_t> Seg;
    typedef std::pair<double, Seg> SegResult;

private: // Private Method(s)
    std::vector<Sequence> segment(const Sequence &sequence,
                                  const Seg &seg) const;
    SegResult segment(const Sequence::const_iterator &begin,
                      const Sequence::const_iterator &end) const;

private: // Private Property(ies)
    FreqTrie trie_;
    double lrv_exp_;
    size_t max_iters_;
}; // class Segmenter

} // namespace esa

#endif // SEG_HPP_INCLUDEED
