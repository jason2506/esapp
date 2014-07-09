/************************************************
 *  encstr.cpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include "encstr.hpp"

namespace esapp
{

encoded_multistring::value_type const encoded_multistring::BOUNDARY_ = 0;

/************************************************
 * Implementation: class encoded_multistring
 ************************************************/

encoded_multistring::encoded_multistring(void)
    : char_id_map_({{0, BOUNDARY_}})
{
    // do nothing
}

encoded_multistring::encoded_multistring(sequence const &s)
    : encoded_multistring()
{
    append(s);
}

void encoded_multistring::append(sequence const &s)
{
    auto n = id_sequence_.size();
    offsets_.push_back(n);

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
}

} // namespace esapp
