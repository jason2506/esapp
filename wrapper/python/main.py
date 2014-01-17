# -*- coding: utf-8 -*-

from esa import Segmenter


def main():
    sequences = [u'這是一隻可愛的小花貓',
                 u'一隻貓',
                 u'真可愛的貓',
                 u'這是一隻花貓',
                 u'小貓真可愛']

    segmenter = Segmenter(2.0, 10, 30, 3)
    segmenter.fit(sequences)
    for sequence in sequences:
        words = segmenter.segment(sequence)
        print u' '.join(word for word in words).encode('utf-8')


if __name__ == '__main__':
    main()
