/************************************************
 *  esapp.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_ESAPP_HPP_
#define ESAPP_ESAPP_HPP_

#include <memory>
#include <vector>
#include <string>

namespace esapp
{

namespace impl { class segmenter; }

/************************************************
 * Declaration: class segmenter
 ************************************************/

class segmenter
{
public: // Public Method(s)
    explicit segmenter(double lrv_exp, size_t max_iters = 10,
                       size_t max_len = 30, double smooth = 0.0);
    ::std::vector<::std::vector<::std::string>> fit_and_segment(
        ::std::vector<::std::string> const &sequences);
    ~segmenter(void);

private: // Private Property(ies)
    ::std::unique_ptr<impl::segmenter> impl_;
}; // class segmenter

} // namespace esapp

#endif // ESAPP_ESAPP_HPP_
