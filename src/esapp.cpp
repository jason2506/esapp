/************************************************
 *  esapp.cpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include <esapp/esapp.hpp>
#include <esapp/segmenter.hpp>

namespace esapp
{

/************************************************
 * Implementation: class segmenter
 ************************************************/

segmenter::segmenter(double lrv_exp, size_t max_iters)
    : impl_(new impl::segmenter(lrv_exp, max_iters))
{
    // do nothing
}

segmenter::~segmenter(void)
{
    // do nothing
}

::std::vector<::std::vector<::std::string>> segmenter::fit_and_segment(
    ::std::vector<::std::string> const &sequences)
{
    return impl_->fit_and_segment(sequences);
}

} // namespace esapp
