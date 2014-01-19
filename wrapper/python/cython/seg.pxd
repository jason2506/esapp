#################################################
# sed.pxd
# ESA++
#
# Copyright (c) 2014, Chi-En Wu
# Distributed under The BSD 3-Clause License
#################################################

from libcpp.vector cimport vector

from wstring cimport wstring


cdef extern from 'seg.hpp':
    cdef cppclass _Segmenter 'esapp::Segmenter':
        _Segmenter(double, size_t, size_t, double) nogil except +

        vector[vector[wstring]] fit_and_segment(const vector[wstring]) nogil
        void fit(const vector[wstring]) nogil
        vector[wstring] segment(const wstring) nogil
        vector[vector[wstring]] segment(const vector[wstring]) nogil
