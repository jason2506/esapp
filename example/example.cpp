#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

#include "str_conv.hpp"
#include "seg.hpp"

using namespace std;
using namespace esa;

int main (void)
{
    vector<wstring> sequences;
    sequences.push_back(s2ws("這是一隻可愛的小花貓"));
    sequences.push_back(s2ws("一隻貓"));
    sequences.push_back(s2ws("真可愛的貓"));
    sequences.push_back(s2ws("這是一隻花貓"));
    sequences.push_back(s2ws("小貓真可愛"));

    Segmenter segmenter(2.0, 10, 30, 3);
    segmenter.fit(sequences);
    for (vector<wstring>::const_iterator it = sequences.begin();
         it != sequences.end(); ++it)
    {
        vector<wstring> s = segmenter.segment(*it);
        for (vector<wstring>::const_iterator sit = s.begin();
             sit != s.end(); ++sit)
        {
            wcout << *sit << ' ';
        }

        wcout << endl;
    }

    return EXIT_SUCCESS;
}
