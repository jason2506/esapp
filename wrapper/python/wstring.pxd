from libc.stddef cimport wchar_t


cdef extern from '<string>' namespace 'std':

    size_t npos = -1

    cdef cppclass wstring:
        wstring() nogil except +
        wstring(wchar_t *) nogil except +
        wstring(wchar_t *, size_t) nogil except +
        wstring(wstring &) nogil except +
        wstring(size_t, wchar_t) nogil except +

        const wchar_t *c_str() nogil
        const wchar_t *data() nogil
        size_t size() nogil
        size_t max_size() nogil
        size_t length() nogil
        void resize(size_t) nogil
        void resize(size_t, wchar_t) nogil
        size_t capacity() nogil
        void reserve(size_t) nogil
        void clear() nogil
        bint empty() nogil

        wchar_t &at(size_t) nogil
        wchar_t &operator[](size_t) nogil
        int compare(wstring &) nogil

        wstring &append(wstring &) nogil
        wstring &append(wstring &, size_t, size_t) nogil
        wstring &append(wchar_t *) nogil
        wstring &append(wchar_t *, size_t) nogil
        wstring &append(size_t, wchar_t) nogil

        void push_back(wchar_t) nogil

        wstring &assign (wstring &) nogil
        wstring &assign (wstring &, size_t, size_t) nogil
        wstring &assign (wchar_t *, size_t) nogil
        wstring &assign (wchar_t *) nogil
        wstring &assign (size_t n, wchar_t) nogil

        wstring &insert(size_t, wstring &) nogil
        wstring &insert(size_t, wstring &, size_t, size_t) nogil
        wstring &insert(size_t, wchar_t *, size_t) nogil

        wstring &insert(size_t, wchar_t *) nogil
        wstring &insert(size_t, size_t, wchar_t) nogil

        size_t copy(wchar_t *, size_t, size_t) nogil

        size_t find(wstring &) nogil
        size_t find(wstring &, size_t) nogil
        size_t find(wchar_t *, size_t, size_t) nogil
        size_t find(wchar_t *, size_t) nogil
        size_t find(wchar_t, size_t) nogil

        size_t rfind(wstring &, size_t) nogil
        size_t rfind(wchar_t *, size_t, size_t) nogil
        size_t rfind(wchar_t *, size_t) nogil
        size_t rfind(wchar_t, size_t) nogil
        size_t rfind(wchar_t) nogil

        size_t find_first_of(wstring &, size_t) nogil
        size_t find_first_of(wchar_t *, size_t, size_t) nogil
        size_t find_first_of(wchar_t *, size_t) nogil
        size_t find_first_of(wchar_t, size_t) nogil
        size_t find_first_of(wchar_t) nogil

        size_t find_first_not_of(wstring &, size_t) nogil
        size_t find_first_not_of(wchar_t *, size_t, size_t) nogil
        size_t find_first_not_of(wchar_t *, size_t) nogil
        size_t find_first_not_of(wchar_t, size_t) nogil
        size_t find_first_not_of(wchar_t) nogil

        size_t find_last_of(wstring &, size_t) nogil
        size_t find_last_of(wchar_t *, size_t, size_t) nogil
        size_t find_last_of(wchar_t *, size_t) nogil
        size_t find_last_of(wchar_t, size_t) nogil
        size_t find_last_of(wchar_t) nogil

        size_t find_last_not_of(wstring &, size_t) nogil
        size_t find_last_not_of(wchar_t *, size_t, size_t) nogil
        size_t find_last_not_of(wchar_t *, size_t) nogil

        wstring substr(size_t, size_t) nogil
        wstring substr() nogil
        wstring substr(size_t) nogil

        size_t find_last_not_of(wchar_t, size_t) nogil
        size_t find_last_not_of(wchar_t) nogil

        #wstring &operator=(wstring &)
        #wstring &operator=(wchar_t *)
        #wstring &operator=(wchar_t)

        wstring operator+(wstring &) nogil
        wstring operator+(wchar_t *) nogil

        bint operator==(wstring &) nogil
        bint operator==(wchar_t *) nogil

        bint operator!=(wstring &) nogil
        bint operator!=(wchar_t *) nogil

        bint operator<(wstring &) nogil
        bint operator<(wchar_t *) nogil

        bint operator>(wstring &) nogil
        bint operator>(wchar_t *) nogil

        bint operator<=(wstring &) nogil
        bint operator<=(wchar_t *) nogil

        bint operator>=(wstring &) nogil
        bint operator>=(wchar_t *) nogil
