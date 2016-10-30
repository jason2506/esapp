/************************************************
 *  encoded_multistring.cpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include "encoded_multistring.hpp"

namespace esapp
{

namespace impl
{

encoded_multistring::value_type const encoded_multistring::BOUNDARY_ = 0;

/************************************************
 * Implementation: class encoded_multistring
 ************************************************/

void encoded_multistring::push_back(sequence const &s)
{
    value_type i = char_id_map_.size();
    for (auto const &c : s)
    {
        if (char_id_map_.find(c) == char_id_map_.end())
        {
            char_id_map_[c] = i++;
        }

        id_sequence_.push_back(char_id_map_[c]);
    }

    id_sequence_.push_back(BOUNDARY_);
    offsets_.push_back(id_sequence_.size());
}

} // namespace impl

} // namespace esapp
