# ESA++


## Usage Example

See [`test_package/example.cpp`](test_package/example.cpp).


## Instructions

### Requirements

- [CMake](https://cmake.org) >= 3.1
- C++ compiler which supports features of C++14

### Installing with Conan (and CMake)

The recommended way to use _ESA++_ package in your project is to install the package with [Conan](https://conan.io).

Assume that your project is built with CMake, you can just execute the following command in your __build directory__:

```sh
$ conan install esapp/0.5.0@jason2506/testing -b outdated -g cmake
```

The `install` command will download the package (together with its dependencies) and generate `conanbuildinfo.cmake` file in the current directory.

Additionally, you need to include `conanbuildinfo.cmake` and then add `conan_basic_setup()` command into your `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.1)
project(myproj)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
```

This will setup necessary CMake variables for finding installed libraries and related files.

Now, you can use `find_package()` and `target_link_libraries()` commands to locate and link the package. For example,

```cmake
find_package(ESA++)

if(ESA++_FOUND)
    add_executable(myexec mycode.cpp)
    target_link_libraries(myexec ESA++::ESA++)
endif()
```

The final step is to build your project with CMake, like:

```sh
$ cmake [SOURCE_DIR] -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```

Please check [Conan Docs](http://docs.conan.io/en/latest/) for more details about how to use conan packages, generators and much more.


### Installing without Conan

You can also install the package without the help of Conan. _ESA++_ is a header-only library. Therefore, all you need to do is to copy header files (contained in the `include/` directory) to your project and manually install all [dependencies](#dependencies) of _ESA++_.


### Building Python Wrapper

There is a Python wrapper for _ESA++_ package. One way of building it is to execute `conan` commands with `--scope wrappers=python` option, like

```sh
$ mkdir _build && cd _build
$ conan install .. --build outdated --scope wrappers=python
$ conan build ..
```

Conan will install all necessary dependencies and build the wrapper.

Alternatively, you can install [dependencies](#dependencies) yourself, setup CMake variables for finding that, and enable `ESAPP_WRAPPER_PYTHON` option:

```sh
$ cmake -H. -B_build -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INCLUDE_PATH=... \
    -DCMAKE_LIBRARY_PATH=... \
    -DCMAKE_PREFIX_PATH=... \
    -DCMAKE_MODULE_PATH=... \
    -DESAPP_WRAPPER_PYTHON=ON
$ cmake --build _build
```

After compiling, you will get `esapp_python.so` (or `esapp_python.dll`) in `_build/wrapper/python/`. You can directly import this module in your Python code:

```python
from esapp_python import Segmenter
```

See [`wrapper/python/example.py`](wrapper/python/example.py).


## Dependencies

- [DICT](https://github.com/jason2506/dict) == 0.1.1
- [pybind11](https://github.com/pybind/pybind11) >= 2.0.0
    * only required if you want to build the python wrapper


## References

- H. Feng, K. Chen, X. Deng, and W. Zheng, "Accessor variety criteria for Chinese word extraction," _Computational Linguistics_, vol. 30, no. 1, pp. 75–93, 2004.
- H. Wang, J. Zhu, S. Tang, and X. Fan, "A new Unsupervised approach to word segmentation," _Computational Linguistics_, vol. 37, no. 3, pp. 421–454, 2011.
- M. I. Abouelhoda, S. Kurtz, and E. Ohlebusch, "Replacing suffix trees with enhanced suffix arrays," _Journal of Discrete Algorithms_, vol. 2, no. 1, pp. 53–86, 2004.


## License

Copyright (c) 2014-2017, Chi-En Wu.

Distributed under [The BSD 3-Clause License](http://opensource.org/licenses/BSD-3-Clause).
