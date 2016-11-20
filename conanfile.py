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
            self.requires('gtest/1.7.0@lasote/stable')

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake "%s" %s -DBUILD_SHARED_LIBS=%s -DBUILD_TESTING=%s' % (
            self.conanfile_directory,
            cmake.command_line,
            self.options.shared,
            self.options.build_tests,
        ))
        self.run('cmake --build . %s' % cmake.build_config)

    def package(self):
        self.copy('*.cmake', dst='', src='cmake')
        self.copy('*.hpp', dst='include', src='include')
        self.copy('*.a', dst='lib', src='lib')
        self.copy('*.lib', dst='lib', src='lib')
        self.copy('*.so*', dst='lib', src='lib')
        self.copy('*.dylib*', dst='lib', src='lib')

    def package_info(self):
        self.cpp_info.libs = ['esapp']
