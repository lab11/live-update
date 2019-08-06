# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

set(LIVE_UPDATE_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
include(${LIVE_UPDATE_ROOT}/make/pic.cmake)

project(hello_world)

target_include_directories(pic PRIVATE
    include/
)

target_sources(pic PRIVATE
    src/main.c
)

