/************************************************
 *  example.cpp
 *  ESA++
 *
 *  Copyright (c) 2014, Chi-En Wu
 *  Distributed under The BSD 3-Clause License
 ************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

#include "seg.hpp"

using namespace std;
using namespace esapp;

int main (void)
{
    wcout.imbue(locale("zh_tw.UTF-8"));

    vector<wstring> sequences = {
        L"這是一隻可愛的小花貓",
        L"一隻貓",
        L"真可愛的貓",
        L"這是一隻花貓",
        L"小貓真可愛"
    };

    Segmenter segmenter(2.0, 10, 30, 3);

    /*
    segmenter.fit(sequences);
    for (auto const &sequence : sequences)
    {
        auto words = segmenter.segment(sequence);
        copy(words.begin(), words.end(), ostream_iterator<wstring, wchar_t>(wcout, L" "));
        wcout << endl;
    }
    //*/

    auto words_list = segmenter.fit_and_segment(sequences);
    for (auto const &words : words_list)
    {
        copy(words.begin(), words.end(), ostream_iterator<wstring, wchar_t>(wcout, L" "));
        wcout << endl;
    }

    return EXIT_SUCCESS;
}
