from distutils.core import setup, Extension

setup(
    ext_modules=[
        Extension('esa',
                  sources=['esa_wrap.cpp'],
                  include_dirs=['../../include'],
                  library_dirs=['../../lib'],
                  libraries=['esa'],
                  language='C++')
    ]
)
