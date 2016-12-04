#!/usr/bin/env python3
import argparse
import os
import subprocess
import sys
import shlex
import distutils.spawn


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('source_dir', nargs='?', default='.',
                        help='specify source directory (default: ".")')
    parser.add_argument('-o', default='_build', dest='build_dir',
                        help='specify build directory (default: "_build")')
    args = parser.parse_args()

    return args


def check_call(args, **kwargs):
    try:
        subprocess.check_call(shlex.split(args), **kwargs)
    except subprocess.CalledProcessError as error:
        sys.exit(error.returncode)


def check_environment():
    check_exec_names = ['cmake', 'conan']

    for name in check_exec_names:
        exe = distutils.spawn.find_executable(name)
        if not exe:
            print("Please install \"{name}\" first.".format(name=name))
            sys.exit(1)


def main():
    args = parse_args()

    check_environment()

    check_call('git submodule init')
    check_call('git submodule update --recursive')

    source_dir = os.path.realpath(args.source_dir)
    build_dir = os.path.realpath(args.build_dir)

    check_call('mkdir -p {name}'.format(name=build_dir) )
    os.chdir(build_dir)

    check_call('conan install --build=outdated {src_dir}'.format(src_dir=source_dir))
    check_call('conan build {src_dir}'.format(src_dir=source_dir))


if __name__ == '__main__':
    main()
