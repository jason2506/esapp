from setuptools import setup, Extension
from Cython.Distutils import build_ext

esapp = Extension(
    'esapp',
    sources=['cython/esapp.pyx'],
    extra_compile_args=['-std=c++11'],
    install_requires=['cython'],
    include_dirs=['../../include'],
    library_dirs=['../../lib'],
    libraries=['esapp'],
    language='C++'
)

setup(
    cmdclass={'build_ext': build_ext},
    ext_modules=[esapp]
)
