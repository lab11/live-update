# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

set(LIVE_UPDATE_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
include(${LIVE_UPDATE_ROOT}/make/pic.cmake)

project(pacemaker)

target_include_directories(pic PRIVATE
    include/
    ${LIVE_UPDATE_ROOT}/app/include/
)

target_sources(pic PRIVATE
    src/main.c
    src/avi.c
    src/lri.c
    src/vrp.c
    #${LIVE_UPDATE_ROOT}/cmake_build/secure/sg_implib.o
)

