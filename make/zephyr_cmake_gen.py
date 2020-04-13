import argparse
import sys

parser = argparse.ArgumentParser(description='Generate CMakeLists.txt file for app.')
parser.add_argument('project', help='project name to pass to CMake `project` for application')
parser.add_argument('sources', help='list of source files to pass to CMake `target_sources` for application')
parser.add_argument('--include_dirs', help='list of include directories to pass to CMake `target_include_directories` for application')
parser.add_argument('--no_app_linker', action='store_true', help='don\' include app linker script')

args = parser.parse_args()

contents = \
"""
cmake_minimum_required(VERSION 3.13.1)
include($ENV{{ZEPHYR_BASE}}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project({})\n
target_sources(app PRIVATE {})
""".format(args.project, args.sources)

# Set relocation for application source files (ignore .o's, e.g. veneer interfaces)
c_sources = []
tfm_headers = None
for s in args.sources.split(" "):
    if s.endswith(".c"):
        c_sources.append(s)
    if s.endswith("s_veneers.o"):
        tfm_headers = s

if tfm_headers:
    contents += "target_sources(zephyr PRIVATE {})\n".format(tfm_headers)
    for s in args.include_dirs.split(" "):
        if s.endswith("tfm/inc"):
            contents += "target_include_directories(zephyr_interface INTERFACE {})\n".format(s)

if args.include_dirs:
    contents += "target_include_directories(app PRIVATE {})\n".format(args.include_dirs)

if not args.no_app_linker:
    for c_src in c_sources:
        contents += "zephyr_code_relocate({} APPFLASH_TEXT_RODATA)\n".format(c_src)
        contents += "zephyr_code_relocate({} APPRAM_DATA_BSS)\n".format(c_src)

print(contents)

