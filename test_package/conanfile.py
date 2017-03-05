import os

from conans import ConanFile, CMake

username = os.getenv('CONAN_USERNAME', 'jason2506')
channel = os.getenv('CONAN_CHANNEL', 'testing')


class TestEsappConan(ConanFile):

    name = 'test-esapp'
    version = '0.1.0'

    requires = (
        'esapp/0.5.2@{username}/{channel}'.format(
            username=username,
            channel=channel,
        ),
    )

    settings = ('os', 'compiler', 'build_type', 'arch')
    generators = ('cmake', 'txt', 'env')

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake "{src_dir}" {opts}'.format(
            src_dir=self.conanfile_directory,
            opts=cmake.command_line,
        ))
        self.run('cmake --build . {}'.format(cmake.build_config))

    def test(self):
        self.run('cd bin && .{}example'.format(os.sep))
