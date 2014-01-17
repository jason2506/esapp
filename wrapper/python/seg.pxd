from libcpp.vector cimport vector

from wstring cimport wstring


cdef extern from 'seg.hpp':
    cdef cppclass _Segmenter 'esa::Segmenter':
        _Segmenter(double, size_t, size_t, double) nogil except +

        void fit(const vector[wstring]) nogil
        vector[wstring] segment(const wstring) nogil
