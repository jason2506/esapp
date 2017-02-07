from conans import ConanFile, CMake


class EsappConan(ConanFile):

    name = 'esapp'
    version = '0.5.1'
    url = 'https://github.com/jason2506/esapp'
    license = 'BSD 3-Clause'
    author = 'Chi-En Wu'

    requires = (
        'dict/0.1.1@jason2506/testing',
    )

    settings = ('os', 'compiler', 'build_type', 'arch')
    generators = ('cmake', 'txt', 'env')
    options = {
        'enable_conan': [True, False],
        'python_exec_path': 'ANY',
    }
    default_options = (
        'enable_conan=True',
        'python_exec_path=None',
    )

    exports = (
        'CMakeLists.txt',
        'cmake/*.cmake',
        'include/*.hpp',
        'example/CMakeLists.txt',
        'example/example.cpp',
        'wrapper/CMakeLists.txt',
        'wrapper/python/CMakeLists.txt',
        'wrapper/python/esapp.cpp',
    )

    _available_wrappers = set([
        'python',
    ])

    @property
    def _wrappers(self):
        wrappers = (self.scope.wrappers or '').split(',')
        return set(wrapper for wrapper in wrappers if wrapper)

    def requirements(self):
        wrappers = self._wrappers
        if 'python' in wrappers:
            self.requires('pybind11/0.1.0@jason2506/testing', private=True)

        unknown_wrappers = wrappers - self._available_wrappers
        for wrapper in unknown_wrappers:
            self.output.warn('Unknown wrapper: {}'.format(wrapper))

    def config_options(self):
        wrappers = self._wrappers
        if 'python' not in wrappers:
            self.options.remove('python_exec_path')

    def build(self):
        extra_opts = []
        extra_opts.append('-DENABLE_CONAN={}'.format(
            self.options.enable_conan,
        ))
        extra_opts.append('-DCMAKE_INSTALL_PREFIX="{}"'.format(
            self.package_folder,
        ))

        wrappers = self._wrappers & self._available_wrappers
        for wrapper in wrappers:
            extra_opts.append('-DESAPP_WRAPPER_{}=ON'.format(wrapper.upper()))

        if 'python' in wrappers and self.options.python_exec_path:
            # http://pybind11.readthedocs.io/en/master/faq.html#cmake-doesn-t-detect-the-right-python-version
            extra_opts.append('-DPYTHON_EXECUTABLE:FILEPATH="{}"'.format(
                self.options.python_exec_path,
            ))

        cmake = CMake(self.settings)
        self.run('cmake "{src_dir}" {opts} {extra_opts}'.format(
            src_dir=self.conanfile_directory,
            opts=cmake.command_line,
            extra_opts=' '.join(extra_opts),
        ))
        self.run('cmake --build . {}'.format(cmake.build_config))

    def package(self):
        self.run('cmake --build . --target install')
