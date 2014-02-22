#################################################
# sed.pxd
# ESA++
#
# Copyright (c) 2014, Chi-En Wu
# Distributed under The BSD 3-Clause License
#################################################

from libcpp.vector cimport vector
from libcpp.string cimport string


cdef extern from 'seg.hpp':
    cdef cppclass _Segmenter 'esapp::Segmenter':
        _Segmenter(double, size_t, size_t, double) nogil except +

        vector[vector[string]] fit_and_segment(const vector[string] &) nogil
        void fit(const vector[string] &) nogil
        vector[string] segment(const string &) nogil
        vector[vector[string]] segment(const vector[string] &) nogil
