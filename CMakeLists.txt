#------------------------------------------------------------------------------
# Modified July 2019, Jean-Luc Watson
# Copyright (c) 2017-2018, Arm Limited. All rights reserved.
# # SPDX-License-Identifier: BSD-3-Clause
#
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.1...3.14)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# TODO: figure out why Zephyr doesn't run if not in debug mode
set(CMAKE_BUILD_TYPE "Debug")

function(debug_print_compilers NUM)
    message(STATUS "${NUM}>>> CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
    message(STATUS "${NUM}>>> CMAKE_ASM_COMPILER: ${CMAKE_ASM_COMPILER}")
    message(STATUS "${NUM}>>> CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
    message(STATUS "${NUM}>>> CMAKE_OBJCOPY: ${CMAKE_OBJCOPY}")
    message(STATUS "${NUM}>>> CMAKE_OBJDUMP: ${CMAKE_OBJDUMP}")
    message(STATUS "${NUM}>>> CMAKE_AR: ${CMAKE_AR}")
    message(STATUS "${NUM}>>> CMAKE_RANLIB: ${CMAKE_RANLIB}")
endfunction(debug_print_compilers)

# Specify where modules can be loaded from
list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/make
)

# Turn off random CMake flags on OS X
set(CMAKE_OSX_SYSROOT "/")
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)

# CMake configuration.
include("project_config")

get_filename_component(STRIPPED_LIST_DIR ${CMAKE_CURRENT_LIST_DIR} NAME)
project("${STRIPPED_LIST_DIR}" LANGUAGES C ASM)

# Including this again because calling project() wipes out most of the config
# vars for some reason. Grr.
include("project_config")

# Platform-specific dependencies
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/sdk" "sdk")
# Startup scripts
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/startup" "startup")

# Add bootloader project; must be built explicitly
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/bootloader" "bootloader" EXCLUDE_FROM_ALL)

# Add secure executable
set(SECURE_TARGET "secure")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/${SECURE_TARGET}" "${SECURE_TARGET}")

# Add non-secure executable (DISABLED)
#set(NONSECURE_TARGET "app")
#add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/${NONSECURE_TARGET}" "${NONSECURE_TARGET}")

# Add Zephyr OS build as NS executable (ENABLED)
set(NONSECURE_TARGET "zephyr")
set(ZEPHYR_DIR ${CMAKE_CURRENT_LIST_DIR}/zephyros/zephyr)
add_custom_target(${NONSECURE_TARGET} ALL)
add_custom_command(
    TARGET ${NONSECURE_TARGET} POST_BUILD

    COMMAND west build -b v2m_musca_nonsecure ${CMAKE_CURRENT_LIST_DIR}/app/hello_world/
    WORKING_DIRECTORY ${ZEPHYR_DIR}
)

# Generate image to flash to the board
add_custom_target(${PROJECT_NAME} ALL)
add_dependencies(${PROJECT_NAME} ${SECURE_TARGET} ${NONSECURE_TARGET})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD

    COMMAND ${CMAKE_CURRENT_LIST_DIR}/bootloader/scripts/assemble.py
    ARGS -l ${CMAKE_CURRENT_LIST_DIR}/sdk/musca_a/include/flash_layout.h
        -s ${CMAKE_CURRENT_BINARY_DIR}/${SECURE_TARGET}/${SECURE_TARGET}.bin
        # TODO: generalize
        #-n ${CMAKE_CURRENT_BINARY_DIR}/${NONSECURE_TARGET}/${NONSECURE_TARGET}.bin
        -n ${CMAKE_CURRENT_LIST_DIR}/zephyros/zephyr/build/zephyr/${NONSECURE_TARGET}.bin
        -o ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_combined.bin

    COMMAND ${PYTHONCLIENT}
    ARGS ${CMAKE_CURRENT_LIST_DIR}/bootloader/scripts/imgtool.py
        sign
        --layout ${CMAKE_CURRENT_LIST_DIR}/sdk/musca_a/include/flash_layout.h
        -k ${CMAKE_CURRENT_LIST_DIR}/bootloader/keys/root-rsa-2048.pem
        --align 1
        -H 0x400
        --pad 0x100000
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_combined.bin
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_signed.bin

    COMMAND srec_cat
    ARGS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_signed.bin -Binary -offset 0x220000 -o ${PROJECT_NAME}.hex -Intel
)

# Generate dump for analysis
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD

    COMMAND ${CMAKE_OBJDUMP}
    ARGS -D $<TARGET_FILE:${SECURE_TARGET}> > ${SECURE_TARGET}.dump

    # TODO: Fix
    #COMMAND ${CMAKE_OBJDUMP}
    #ARGS -D $<TARGET_FILE:${NONSECURE_TARGET}> > ${NONSECURE_TARGET}.dump
)

# Save output to commit-specific repository for update generation
set(LU_GIT_REV_FILE /tmp/liveupdate-git-rev)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD

    COMMAND git
    ARGS rev-parse HEAD > ${LU_GIT_REV_FILE}

    COMMAND rm
    ARGS -rf ${CMAKE_CURRENT_LIST_DIR}/.update/$(< ${LU_GIT_REV_FILE})/

    COMMAND mkdir
    ARGS ${CMAKE_CURRENT_LIST_DIR}/.update/$(< ${LU_GIT_REV_FILE})/

    COMMAND cp
    ARGS ${CMAKE_CURRENT_LIST_DIR}/zephyros/zephyr/build/zephyr/zephyr.map
        ${CMAKE_CURRENT_LIST_DIR}/zephyros/zephyr/build/app/relocated_pic.elf
        ${CMAKE_CURRENT_LIST_DIR}/.update/$(< ${LU_GIT_REV_FILE})/
)

# make flash
add_custom_target(flash
    COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex /Volumes/MUSCA/
)
add_dependencies(flash ${PROJECT_NAME})

# make startdebug
set(GDB_DEBUG_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/make/debug.gdb")
add_custom_target(startdebug
    COMMAND ${GDBSERVER} ${GDBSERVER_ARGS} &
    COMMAND sleep 2
    COMMAND ${GDBCLIENT} ${CMAKE_CURRENT_BINARY_DIR}/../zephyros/zephyr/build/${NONSECURE_TARGET}/${NONSECURE_TARGET}.elf --command=${GDB_DEBUG_SCRIPT}
)
add_dependencies(startdebug ${PROJECT_NAME})

# make ns-startdebug
#add_custom_target(ns-startdebug
#    COMMAND ${GDBSERVER} ${GDBSERVER_ARGS} &
#    COMMAND sleep 2
#    COMMAND ${GDBCLIENT} ${CMAKE_CURRENT_BINARY_DIR}/${NONSECURE_TARGET}/${NONSECURE_TARGET}.elf --command=${GDB_DEBUG_SCRIPT}
#)
#add_dependencies(ns-startdebug ${PROJECT_NAME})

# make clean-zephyr
add_custom_target(clean-zephyr
    COMMAND make clean
    WORKING_DIRECTORY ${ZEPHYR_DIR}/build
)

