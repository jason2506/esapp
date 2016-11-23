/************************************************
 *  esapp.cpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include <string>
#include <vector>

#include <esapp/internal/segmenter.hpp>
#include <esapp/esapp.hpp>

namespace esapp {

/************************************************
 * Implementation: class segmenter
 ************************************************/

segmenter::segmenter(double lrv_exp, size_t max_iters)
    : impl_(new internal::segmenter(lrv_exp, max_iters)) {
    // do nothing
}

segmenter::~segmenter() {
    // do nothing
}

std::vector<std::vector<std::string>> segmenter::fit_and_segment(
        std::vector<std::string> const &sequences) {
    return impl_->fit_and_segment(sequences);
}

}  // namespace esapp
