# -*- coding: utf-8 -*-

#################################################
# example.py
# ESA++
#
# Copyright (c) 2014-2017, Chi-En Wu
# Distributed under The BSD 3-Clause License
#################################################

from __future__ import print_function, unicode_literals

from esapp_python import Segmenter


def main():
    sequences = ['這是一隻可愛的小花貓',
                 '一隻貓',
                 '真可愛的貓',
                 '這是一隻花貓',
                 '小貓真可愛']

    segmenter = Segmenter()

    for s in sequences:
        segmenter.fit(s)

    segmenter.optimize(10)
    for s in sequences:
        words = segmenter.segment(s)
        print(' '.join(words))


if __name__ == '__main__':
    main()
