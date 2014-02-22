# -*- coding: utf-8 -*-

#################################################
# example.px
# ESA++
#
# Copyright (c) 2014, Chi-En Wu
# Distributed under The BSD 3-Clause License
#################################################

from __future__ import print_function, unicode_literals

from esa import Segmenter


def main():
    sequences = ['這是一隻可愛的小花貓',
                 '一隻貓',
                 '真可愛的貓',
                 '這是一隻花貓',
                 '小貓真可愛']

    segmenter = Segmenter(2.0, 10, 30, 3)

    '''
    segmenter.fit(sequences)
    for sequence in sequences:
        words = segmenter.segment(sequence)
        print(' '.join(words))
    #'''

    words_list = segmenter.fit_and_segment(sequences)
    for words in words_list:
        print(' '.join(words))


if __name__ == '__main__':
    main()
