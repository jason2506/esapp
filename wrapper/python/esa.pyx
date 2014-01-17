# -*- coding: utf-8 -*-

from cython cimport address
from cpython.unicode cimport PyUnicode_GetSize
from libcpp.vector cimport vector

from wstring cimport wstring
from unicode cimport PyUnicodeObject, PyUnicode_AsWideChar, PyUnicode_FromWideChar
from seg cimport _Segmenter


cdef class Segmenter(object):
    cdef _Segmenter *_segmenter

    def __cinit__(self, lrv_exp, max_iters=10, max_length=30, smooth=0.0):
        self._segmenter = new _Segmenter(lrv_exp, max_iters, max_length, smooth);

    def __dealloc__(self):
        del self._segmenter

    cpdef fit(self, list sequences):
        cdef vector[wstring] conv_sequences
        cdef wstring ws
        cdef Py_ssize_t length
        for sequence in sequences:
            length = PyUnicode_GetSize(sequence)
            ws.assign(length, 0)
            PyUnicode_AsWideChar(<PyUnicodeObject *>sequence, address(ws[0]), length)
            conv_sequences.push_back(ws)

        self._segmenter.fit(conv_sequences)

    cpdef list segment(self, unicode sequence):
        cdef vector[wstring] result
        cdef wstring ws
        length = PyUnicode_GetSize(sequence)
        ws.assign(length, 0)
        PyUnicode_AsWideChar(<PyUnicodeObject *>sequence, address(ws[0]), length)

        words = self._segmenter.segment(ws)
        return [PyUnicode_FromWideChar(word.c_str(), word.size()) for word in words]
