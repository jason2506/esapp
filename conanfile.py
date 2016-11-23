from conans import ConanFile, CMake


class EsappConan(ConanFile):

    name = 'esapp'
    version = '0.4.1'
    url = 'https://github.com/jason2506/esapp'
    license = 'BSD 3-Clause'
    author = 'Chi-En Wu'

    requires = (
        'literator/0.1.0@jason2506/testing',
        'desa/0.1.0@jason2506/testing'
    )

    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'cmake'
    options = {
        'shared': [True, False],
        'build_tests': [True, False],
    }
    default_options = (
        'desa:build_tests=False',
        'gtest:shared=False',
        'shared=False',
        'build_tests=True',
    )

    exports = (
        'CMakeLists.txt',
        'cmake/*.cmake',
        'include/*.hpp',
        'src/CMakeLists.txt',
        'src/*.cpp',
        'test/CMakeLists.txt',
        'test/*.cpp',
    )

    def requirements(self):
        if self.options.build_tests:
            self.requires('gtest/1.8.0@lasote/stable')

    def build(self):
        cmake = CMake(self.settings)

        args = []
        args.append('-DENABLE_CONAN=%s' % self.options.enable_conan)
        args.append('-DBUILD_SHARED_LIBS=%s' % self.options.shared)
        args.append('-DBUILD_TESTING=%s' % self.options.build_tests)
        args.append('-DCMAKE_INSTALL_PREFIX="%s"' % self.package_folder)

        self.run('cmake "%s" %s %s' % (
            self.conanfile_directory,
            cmake.command_line,
            ' '.join(args)
        ))
        self.run('cmake --build . --target install %s' % cmake.build_config)

    def package_info(self):
        self.cpp_info.libs = ['esapp']
