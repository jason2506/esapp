/************************************************
 *  segmenter.hpp
 *  ESA++
 *
 *  Copyright (c) 2014-2017, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#ifndef ESAPP_SEGMENTER_HPP_
#define ESAPP_SEGMENTER_HPP_

#include <cassert>
#include <cwctype>

#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

#include <dict/text_index.hpp>
#include <dict/with_lcp.hpp>

#include "internal/with_segments.hpp"
#include "internal/decode_utf8.hpp"

namespace esapp {

/************************************************
 * Declaration: class segmenter
 ************************************************/

class segmenter {
 public:  // Public Type(s)
    using size_type = std::size_t;

 public:  // Public Method(s)
    explicit segmenter(double lrv_exp);

    template <typename ForwardIterator>
    void fit(ForwardIterator begin, ForwardIterator end);
    void optimize(size_type n_iters);
    template <typename WordType, typename ForwardIterator>
    std::vector<WordType> segment_into(ForwardIterator begin, ForwardIterator end) const;
    template <typename ForwardIterator>
    std::vector<std::string> segment(ForwardIterator begin, ForwardIterator end) const;

 private:  // Private Type(s)
    using text_index = dict::text_index<
        dict::with_lcp<
            internal::with_segments<30>::policy
        >::policy
    >;
    using term_id = text_index::term_type;
    using term_type = std::wint_t;

 private:  // Private Static Method(s)
    template <typename ForwardIterator, typename Predicate>  // NOLINTNEXTLINE(runtime/references)
    static term_type scan_while(ForwardIterator &scanned_it, ForwardIterator &it,
                                ForwardIterator end, Predicate f);

 private:  // Private Property(ies)
    double lrv_exp_;
    std::unordered_map<term_type, term_id> term_id_map_;
    text_index index_;
};  // class segmenter

/************************************************
 * Inline Helper Function(s)
 ************************************************/

inline int ischs(std::wint_t c) {
    return c >= u'\u4E00' && c <= u'\u9FFF';
}

inline int isfwalnum(std::wint_t c) {
    return (c >= u'Ａ' && c <= u'Ｚ') ||
           (c >= u'ａ' && c <= u'ｚ') ||
           (c >= u'０' && c <= u'９');
}

/************************************************
 * Implementation: class segmenter
 ************************************************/

inline segmenter::segmenter(double lrv_exp)
    : lrv_exp_(lrv_exp), term_id_map_({{0, 0}}) {
    // do nothing
}

template <typename ForwardIterator>
void segmenter::fit(ForwardIterator it, ForwardIterator end) {
    term_id id = term_id_map_.size();
    std::vector<term_id> token;
    while (it != end) {
        auto term = internal::decode_utf8<term_type>(it, end);
        if (ischs(term)) {
            token.clear();
            do {
                if (term_id_map_.find(term) == term_id_map_.end()) {
                    term_id_map_.emplace(term, id++);
                }

                token.push_back(term_id_map_[term]);
            } while (it != end && ischs(term = internal::decode_utf8<term_type>(it, end)));

            index_.insert(token);
        }
    }
}

inline void segmenter::optimize(size_type n_iters) {
    index_.optimize(lrv_exp_, n_iters);
}

template <typename WordType, typename ForwardIterator>
std::vector<WordType> segmenter::segment_into(ForwardIterator it, ForwardIterator end) const {
    if (it == end) { return {}; }

    decltype(segment_into<WordType>(it, end)) words;
    std::vector<term_id> token;

    auto word_begin = it;
    auto term = internal::decode_utf8<term_type>(it, end);
    while (it != end) {
        auto word_end = it;
        if (ischs(term)) {
            token.clear();
            do {
                word_end = it;
                auto term_id_it = term_id_map_.find(term);
                if (term_id_it == term_id_map_.end()) {
                    token.push_back(std::numeric_limits<term_id>::max());
                } else {
                    token.push_back(term_id_it->second);
                }
            } while (it != end && ischs(term = internal::decode_utf8<term_type>(it, end)));

            auto seg_pos_vec = index_.segment(token, lrv_exp_);
            decltype(seg_pos_vec)::value_type prev_pos = 0;
            for (auto pos : seg_pos_vec) {
                assert(pos > prev_pos);
                words.emplace_back(word_begin + prev_pos * 3, word_begin + pos * 3);
                prev_pos = pos;
            }
        } else if (std::iswspace(term)) {
            term = scan_while(it, word_end, end, std::iswspace);
        } else {
            if (isfwalnum(term)) {
                term = scan_while(it, word_end, end, isfwalnum);
            } else if (std::iswalnum(term)) {
                term = scan_while(it, word_end, end, std::iswalnum);
            } else {
                term = internal::decode_utf8<term_type>(it, end);
            }

            assert(word_begin != word_end);
            words.emplace_back(word_begin, word_end);
        }

        word_begin = word_end;
    }

    if (word_begin != it) {
        words.emplace_back(word_begin, it);
    }

    return words;
}

template <typename ForwardIterator>
std::vector<std::string> segmenter::segment(ForwardIterator it, ForwardIterator end) const {
    return segment_into<std::string>(it, end);
}

template <typename ForwardIterator, typename Predicate>
segmenter::term_type segmenter::scan_while(ForwardIterator &scanned_it, ForwardIterator &it,
                                           ForwardIterator end, Predicate f) {
    assert(scanned_it != end);

    term_type term;
    while (f(term = internal::decode_utf8<term_type>(scanned_it, end))
            && (it = scanned_it) != end) {
        // do nothing
    }

    return term;
}

}  // namespace esapp

#endif  // ESAPP_SEGMENTER_HPP_
