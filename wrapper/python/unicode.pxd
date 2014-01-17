from libc.stddef cimport wchar_t


cdef extern from '<Python.h>':

    ctypedef struct PyUnicodeObject:
        pass

    object PyUnicode_FromWideChar(wchar_t *, Py_ssize_t)
    Py_ssize_t PyUnicode_AsWideChar(PyUnicodeObject *, wchar_t *, Py_ssize_t)
