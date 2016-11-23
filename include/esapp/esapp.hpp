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
#include <string>
#include <vector>

namespace esapp {

// forward declaration
namespace internal { class segmenter; }

/************************************************
 * Declaration: class segmenter
 ************************************************/

class segmenter {
 public:  // Public Method(s)
    explicit segmenter(double lrv_exp, std::size_t max_iters = 10);
    ~segmenter();

    std::vector<std::vector<std::string>>
    fit_and_segment(std::vector<std::string> const &sequences);

 private:  // Private Property(ies)
    std::unique_ptr<internal::segmenter> impl_;
};  // class segmenter

}  // namespace esapp

#endif  // ESAPP_ESAPP_HPP_
