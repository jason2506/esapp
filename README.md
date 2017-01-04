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
$ conan install esapp/0.4.1@jason2506/testing -b outdated -g cmake
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


### Manually Compiling

If you do not intend to use Conan in your project, you can just clone this repository and manually build the package.

The simplest way is to use our build script:

```sh
$ ./build.py
```

which will create a `_build/` directory and then build the package inside it.

Note that Conan is still needed for downloading [dependencies](#dependencies) of _ESA++_. Otherwise, you must prepare those packages, setup CMake variables for finding them, and execute CMake commands to compile the package.

For instance,

```sh
$ cmake -H. -B_build -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INCLUDE_PATH=... \
    -DCMAKE_LIBRARY_PATH=... \
    -DCMAKE_PREFIX_PATH=... \
    -DCMAKE_MODULE_PATH=...
$ cmake --build _build
```

After compiling the package, resulting files including libraries and examples will be placed in the `_build` directory. You can manually copy necessary files (e.g., header files and libraries) to your project, or automatically install the package with `install` build target:

```sh
$ cmake --build _build --target install
```

## Dependencies

- [DICT](https://github.com/jason2506/dict) == 0.1.0


## References

- H. Feng, K. Chen, X. Deng, and W. Zheng, "Accessor variety criteria for Chinese word extraction," _Computational Linguistics_, vol. 30, no. 1, pp. 75–93, 2004.
- H. Wang, J. Zhu, S. Tang, and X. Fan, "A new Unsupervised approach to word segmentation," _Computational Linguistics_, vol. 37, no. 3, pp. 421–454, 2011.
- M. I. Abouelhoda, S. Kurtz, and E. Ohlebusch, "Replacing suffix trees with enhanced suffix arrays," _Journal of Discrete Algorithms_, vol. 2, no. 1, pp. 53–86, 2004.


## License

Copyright (c) 2014-2017, Chi-En Wu.

Distributed under [The BSD 3-Clause License](http://opensource.org/licenses/BSD-3-Clause).
