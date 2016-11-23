/************************************************
 *  segmenter.cpp
 *  ESA++
 *
 *  Copyright (c) 2014-2016, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include <esapp/segmenter.hpp>

namespace esapp
{

namespace impl
{

/************************************************
 * Implementation: class segmenter
 ************************************************/

segmenter::segmenter(double lrv_exp, size_type max_iters)
    : lrv_exp_(lrv_exp), max_iters_(max_iters), term_id_map_({{0, 0}})
{
    // do nothing
}

std::vector<std::vector<std::string>> segmenter::fit_and_segment(
    std::vector<std::string> const &sequences)
{
    term_id id = 0;
    for (auto const &sequence : sequences)
    {
        auto it = token_iterator(sequence.begin(), sequence.end());
        auto end = token_iterator(sequence.end(), sequence.end());

        for ( ; it != end; ++it)
        {
            auto const &token = *it;
            if (!ischs(token[0])) { continue; }

            decltype(token.size()) i = 0;
            std::vector<term_id> s(token.size());
            for (auto const &c : token)
            {
                if (term_id_map_.find(c) == term_id_map_.end())
                {
                    term_id_map_.emplace(c, ++id);
                }

                s[i++] = term_id_map_[c];
            }

            index_.insert(s);
        }
    }

    index_.optimize(lrv_exp_, max_iters_);

    decltype(fit_and_segment(sequences)) words_list;
    words_list.reserve(sequences.size());

    // generate segmented word lists
    size_type i = 0;
    for (auto const &sequence : sequences)
    {
        auto it = token_iterator(sequence.begin(), sequence.end());
        auto end = token_iterator(sequence.end(), sequence.end());

        typename decltype(words_list)::value_type words;
        auto word_begin = sequence.begin();
        for ( ; it != end; ++it)
        {
            auto word_end = it.base();
            ++word_end;

            auto ch = (*it)[0];
            if (ischs(ch))
            {
                auto num_segs = index_.num_segs(i);
                seg_pos_list::value_type prev_pos = 0;
                for (decltype(num_segs) j = 0; j < num_segs; j++)
                {
                    auto pos = index_.seg_pos(i, j);
                    words.emplace_back(word_begin + prev_pos * 3, word_begin + pos * 3);
                    prev_pos = pos;
                }

                i++;
            }
            else if (!std::iswspace(ch))
            {
                words.emplace_back(word_begin, word_end);
            }

            word_begin = word_end;
        }

        words_list.push_back(words);
    }

    return words_list;
}

} // namespace impl

} // namespace esapp
