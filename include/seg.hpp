#ifndef SEG_HPP_INCLUDEED
#define SEG_HPP_INCLUDEED

#include <cmath>
#include <string>
#include <vector>

#include "trie.hpp"

namespace esa
{

/************************************************
 * Declaration: class Segmenter
 ************************************************/

class Segmenter
{
public: // Public Method(s)
    Segmenter(double lrv_exp, size_t max_iters = 10,
              size_t max_length = 30, double smooth = 0.0);

    void fit(const std::vector<std::wstring> &sequences);
    std::vector<std::wstring> segment(const std::wstring &sequence) const;

private: // Private Type(s)
    typedef std::pair<double, std::vector<size_t> > SegResult;

private: // Private Method(s)
    std::vector<std::wstring> segment(const std::wstring &sequence,
                                      const std::vector<size_t> &seg) const;
    SegResult segment(const std::wstring::const_iterator &begin,
                      const std::wstring::const_iterator &end) const;

private: // Private Property(ies)
    FreqTrie trie_;
    double lrv_exp_;
    size_t max_iters_;
}; // class Segmenter

} // namespace esa

#endif // SEG_HPP_INCLUDEED
