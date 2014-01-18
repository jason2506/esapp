from libc.stddef cimport wchar_t


cdef extern from '<string>' namespace 'std':

    size_t npos = -1

    cdef cppclass wstring:
        wstring() nogil except +
        const wchar_t *c_str() nogil
        size_t size() nogil
        wchar_t &operator[](size_t) nogil
        wstring &assign (size_t n, wchar_t) nogil
