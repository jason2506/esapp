import argparse
import os
import subprocess
import sys


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
        subprocess.check_call(args, **kwargs)
    except subprocess.CalledProcessError as error:
        sys.exit(error.returncode)


def main():
    args = parse_args()

    check_call(['git', 'submodule', 'init'])
    check_call(['git', 'submodule', 'update', '--recursive'])

    source_dir = os.path.realpath(args.source_dir)
    build_dir = os.path.realpath(args.build_dir)

    check_call(['mkdir', '-p', build_dir])
    os.chdir(build_dir)

    check_call(['conan', 'install', '--build=outdated', source_dir])
    check_call(['conan', 'build', source_dir])


if __name__ == '__main__':
    main()
