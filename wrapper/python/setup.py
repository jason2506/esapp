from setuptools import setup, Extension
from Cython.Distutils import build_ext

esapp = Extension(
    'esapp',
    sources=['cython/esapp.pyx'],
    extra_compile_args=['-std=c++14'],
    libraries=['esapp'],
    language='C++'
)

setup(
    setup_requires=['cython>=0.x'],
    cmdclass={'build_ext': build_ext},
    ext_modules=[esapp]
)
