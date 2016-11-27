#################################################
# sed.pxd
# ESA++
#
# Copyright (c) 2014-2016, Chi-En Wu
# Distributed under The BSD 3-Clause License
#################################################

from libcpp.vector cimport vector
from libcpp.string cimport string


cdef extern from 'esapp/segmenter.hpp':
    cdef cppclass _Segmenter 'esapp::segmenter':
        _Segmenter(double, size_t) nogil except +

        vector[vector[string]] fit_and_segment(const vector[string] &) nogil
