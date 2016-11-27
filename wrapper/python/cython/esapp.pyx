# -*- coding: utf-8 -*-

#################################################
# esapp.pyx
# ESA++
#
# Copyright (c) 2014-2016, Chi-En Wu
# Distributed under The BSD 3-Clause License
#################################################

from libcpp.vector cimport vector
from libcpp.string cimport string

from segmenter cimport _Segmenter


cdef class Segmenter(object):
    cdef _Segmenter *_segmenter

    def __cinit__(self, lrv_exp):
        self._segmenter = new _Segmenter(lrv_exp);

    def __dealloc__(self):
        del self._segmenter

    cpdef void fit(self, s):
        cdef string cpp_s = s.encode('utf-8')
        self._segmenter.fit(cpp_s.begin(), cpp_s.end())

    cpdef void optimize(self, n_iters):
        self._segmenter.optimize(n_iters);

    cpdef list segment(self, s):
        cdef string cpp_s = s.encode('utf-8')
        cdef vector[string] words = self._segmenter.segment(cpp_s.begin(), cpp_s.end())
        return [word.decode('utf-8') for word in words]
