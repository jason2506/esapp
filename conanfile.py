from conans import ConanFile, CMake


class EsappConan(ConanFile):

    name = 'esapp'
    version = '0.4.1'
    url = 'https://github.com/jason2506/esapp'
    license = 'BSD 3-Clause'
    author = 'Chi-En Wu'

    requires = 'desa/0.1.0@jason2506/testing'

    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = ('cmake', 'txt', 'env')
    options = {
        'enable_conan': [True, False],
    }
    default_options = (
        'enable_conan=True',
    )

    exports = (
        'CMakeLists.txt',
        'cmake/*.cmake',
        'include/*.hpp'
    )

    def build(self):
        cmake = CMake(self.settings)

        args = []
        args.append('-DENABLE_CONAN=%s' % self.options.enable_conan)
        args.append('-DCMAKE_INSTALL_PREFIX="%s"' % self.package_folder)

        self.run('cmake "%s" %s %s' % (
            self.conanfile_directory,
            cmake.command_line,
            ' '.join(args)
        ))
        self.run('cmake --build .')

    def package(self):
        cmake = CMake(self.settings)
        self.run('cmake --build . --target install %s' % cmake.build_config)
