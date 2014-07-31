from setuptools import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(
    cmdclass={'build_ext': build_ext},
    ext_modules=[
        Extension('esapp',
                  sources=['cython/esapp.pyx'],
                  extra_compile_args=['-std=c++11', '-stdlib=libc++'],
                  include_dirs=['../../include'],
                  library_dirs=['../../lib'],
                  libraries=['esapp'],
                  language='C++')
    ]
)
