/************************************************
 *  esapp.cpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include "esapp.hpp"
#include "segmenter.hpp"

namespace esapp
{

/************************************************
 * Implementation: class segmenter
 ************************************************/

segmenter::segmenter(double lrv_exp, size_t max_iters,
                     size_t max_len, double smooth)
    : impl_(new impl::segmenter(lrv_exp, max_iters, max_len, smooth))
{
    // do nothing
}

segmenter::~segmenter(void)
{
    // do nothing
}

std::vector<std::vector<std::string>> segmenter::fit_and_segment(
    std::vector<std::string> const &sequences)
{
    return impl_->fit_and_segment(sequences);
}

} // namespace esapp
