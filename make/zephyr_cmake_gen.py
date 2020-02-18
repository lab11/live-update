import argparse
import sys

parser = argparse.ArgumentParser(description='Generate CMakeLists.txt file for app.')
parser.add_argument('project', help='project name to pass to CMake `project` for application')
parser.add_argument('sources', help='list of source files to pass to CMake `target_sources` for application')
parser.add_argument('--include_dirs', help='list of include directories to pass to CMake `target_include_directories` for application')

args = parser.parse_args()

contents = \
"""
cmake_minimum_required(VERSION 3.13.1)
include($ENV{{ZEPHYR_BASE}}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project({})\n
target_sources(app PRIVATE {})
""".format(args.project, args.sources)

if args.include_dirs:
    contents += "target_include_directories(app PRIVATE {})\n".format(args.include_dirs)

# Set relocation for application source files (ignore .o's, e.g. veneer interfaces)
c_sources = []
for s in args.sources.split(" "):
    if s.endswith(".c"):
        c_sources.append(s)
c_sources = " ".join(c_sources)

contents += "zephyr_code_relocate({} APPFLASH_TEXT_RODATA)\n".format(c_sources)
contents += "zephyr_code_relocate({} APPRAM_DATA_BSS)\n".format(c_sources)

print(contents)

