# -*- coding: utf-8 -*-

from cython cimport address
from cpython.unicode cimport PyUnicode_GetSize
from libcpp.vector cimport vector

from wstring cimport wstring
from unicode cimport PyUnicodeObject, PyUnicode_AsWideChar, PyUnicode_FromWideChar
from seg cimport _Segmenter


cdef vector[wstring] to_wstring_vector(list objs):
    cdef vector[wstring] wstr_vector
    cdef unicode obj
    for obj in objs:
        wstr_vector.push_back(to_wstring(obj))

    return wstr_vector


cdef wstring to_wstring(unicode obj):
    cdef wstring wstr
    cdef Py_ssize_t length
    length = PyUnicode_GetSize(obj)
    wstr.assign(length, 0)
    PyUnicode_AsWideChar(<PyUnicodeObject *>obj, address(wstr[0]), length)
    return wstr


cdef list from_wstring_vector(vector[wstring] wstr_vector):
    cdef wstring wstr
    return [from_wstring(wstr) for wstr in wstr_vector]


cdef unicode from_wstring(wstring wstr):
    return PyUnicode_FromWideChar(wstr.c_str(), wstr.size())


cdef class Segmenter(object):
    cdef _Segmenter *_segmenter

    def __cinit__(self, lrv_exp, max_iters=10, max_length=30, smooth=0.0):
        self._segmenter = new _Segmenter(lrv_exp, max_iters, max_length, smooth);

    def __dealloc__(self):
        del self._segmenter

    cpdef list fit_and_segment(self, list sequences):
        cdef vector[vector[wstring]] words_list
        cdef vector[wstring] words
        words_list = self._segmenter.fit_and_segment(to_wstring_vector(sequences))
        return [from_wstring_vector(words) for words in words_list]

    cpdef fit(self, list sequences):
        self._segmenter.fit(to_wstring_vector(sequences))

    cpdef list segment_all(self, list sequences):
        cdef vector[vector[wstring]] words_list
        cdef vector[wstring] words
        words_list = self._segmenter.segment(to_wstring_vector(sequences))
        return [from_wstring_vector(words) for words in words_list]

    cpdef list segment(self, unicode sequence):
        cdef vector[wstring] words
        words = self._segmenter.segment(to_wstring(sequence))
        return from_wstring_vector(words)
