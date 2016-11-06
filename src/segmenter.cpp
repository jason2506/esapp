/************************************************
 *  segmenter.cpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include "segmenter.hpp"

namespace esapp
{

namespace impl
{

segmenter::term_id const segmenter::BOUNDARY_ = 0;

/************************************************
 * Implementation: class segmenter
 ************************************************/

segmenter::segmenter(double lrv_exp, size_type max_iters)
    : lrv_exp_(lrv_exp), max_iters_(max_iters), term_id_map_({{0, BOUNDARY_}})
{
    // do nothing
}

::std::vector<::std::vector<::std::string>> segmenter::fit_and_segment(
    ::std::vector<::std::string> const &sequences)
{
    // pre-segment sequences by alphabets, numbers, and symbols
    auto tokens_it = make_filter_iterator(
        [] (::std::vector<term_type> const &token) { return ischs(token[0]); },
        make_flatten_iterator(
            make_map_iterator(
                [](::std::string const &s) { return token_iterator(s); },
                make_generator_adaptor(sequences.begin(), sequences.end())
            )
        )
    );

    term_id id = 0;
    while (tokens_it)
    {
        auto m = tokens_it->size();
        decltype(m) i = 0;
        ::std::vector<term_id> token(m);
        for (auto const &c : *tokens_it)
        {
            if (term_id_map_.find(c) == term_id_map_.end())
            {
                term_id_map_.emplace(c, ++id);
            }

            token[i++] = term_id_map_[c];
        }

        // calculate statistics of substrings
        index_.insert(token);
        ++tokens_it;
    }

    index_.optimize(lrv_exp_, max_iters_);

    decltype(fit_and_segment(sequences)) words_list;
    words_list.reserve(sequences.size());

    // generate segmented word lists
    size_type i = 0;
    for (auto const &sequence : sequences)
    {
        typename decltype(words_list)::value_type words;
        auto begin = sequence.begin();
        for (auto tokens_it = token_iterator(sequence); tokens_it; ++tokens_it)
        {
            auto end = tokens_it.position();
            auto ch = (*tokens_it)[0];
            if (ischs(ch))
            {
                auto num_segs = index_.num_segs(i);
                seg_pos_list::value_type prev_pos = 0;
                for (decltype(num_segs) j = 0; j < num_segs; j++)
                {
                    auto pos = index_.seg_pos(i, j);
                    words.emplace_back(begin + prev_pos * 3, begin + pos * 3);
                    prev_pos = pos;
                }

                i++;
            }
            else if (!::std::iswspace(ch))
            {
                words.emplace_back(begin, end);
            }

            begin = end;
        }

        words_list.push_back(words);
    }

    return words_list;
}

} // namespace impl

} // namespace esapp
