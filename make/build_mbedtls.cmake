#-------------------------------------------------------------------------------
# Modified July 2019, Jean-Luc Watson
# Copyright (c) 2017-2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.7...3.14)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

include(ExternalProject)

if(NOT DEFINED MBEDTLS_DEBUG)
    message(STATUS "MBEDTLS_DEBUG set to 'OFF'.")
    set(MBEDTLS_DEBUG "OFF")
endif()

if(NOT DEFINED MBEDTLS_SOURCE_DIR)
    message(FATAL_ERROR "MBEDTLS_SOURCE_DIR not defined.")
endif()

if(NOT DEFINED MBEDTLS_INSTALL_DIR)
    message(FATAL_ERROR "MBEDTLS_INSTALL_DIR not defined.")
endif()

if(NOT DEFINED MBEDTLS_C_FLAGS)
    message(FATAL_ERROR "MBEDTLS_C_FLAGS not defined.")
endif()

if(NOT DEFINED MBEDTLS_TARGET_NAME)
    message(STATUS "MBEDTLS_TARGET_NAME set to 'mbedtls'")
    set(MBEDTLS_TARGET_NAME "mbedtls")
endif()
if (TARGET ${MBEDTLS_TARGET_NAME})
	message(FATAL_ERROR "A target with name ${MBEDTLS_TARGET_NAME} is already\
defined. Please set MBEDTLS_TARGET_NAME to a unique value.")
endif()

if(MBEDTLS_DEBUG)
    set(MBEDTLS_BUILD_TYPE "Debug")
else()
    set(MBEDTLS_BUILD_TYPE "MinSizeRel")
endif()

function(get_mbedtls_version)
	file(READ "${MBEDTLS_SOURCE_DIR}/include/mbedtls/version.h" MBEDTLS_VER_FILE)
	string(REGEX REPLACE ".*#define[ ]+MBEDTLS_VERSION_STRING[^\"]+\"+([0-9.]+)\".*" "\\1"
		_MBEDTLS_VER ${MBEDTLS_VER_FILE})
	set(MBEDTLS_VERSION ${_MBEDTLS_VER} PARENT_SCOPE)
endfunction()

get_mbedtls_version()

# Define where mbedtls intermediate output files are stored.
set(MBEDTLS_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/mbedtls")
# Make sure that the cross-compilation toolchain knows how to create .a libs
set(CMAKE_C_CREATE_STATIC_LIBRARY "<CMAKE_AR> cr <TARGET> <LINK_FLAGS> <OBJECTS> ")

ExternalProject_Add(${MBEDTLS_TARGET_NAME}
        SOURCE_DIR ${MBEDTLS_SOURCE_DIR}
        BINARY_DIR ${MBEDTLS_BINARY_DIR}
        CMAKE_ARGS -DENABLE_TESTING=OFF -DENABLE_PROGRAMS=OFF
        CMAKE_ARGS -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${MBEDTLS_BUILD_TYPE}
        # Workaround for MbedTLS issue https://github.com/ARMmbed/mbedtls/issues/1496
        if(MBEDTLS_VERSION VERSION_GREATER "2.7.0")
            CMAKE_ARGS -DCMAKE_HOST_UNIX:bool=true
        endif()
        # C compiler settings
        CMAKE_CACHE_ARGS -DCMAKE_C_COMPILER:string=${CMAKE_C_COMPILER}
        CMAKE_CACHE_ARGS -DCMAKE_C_FLAGS:string=${MBEDTLS_C_FLAGS}
        CMAKE_CACHE_ARGS -DCMAKE_C_OUTPUT_EXTENSION:string=.o
        CMAKE_CACHE_ARGS -DCMAKE_C_COMPILER_WORKS:bool=true
        # Archiver settings
        CMAKE_CACHE_ARGS -DCMAKE_AR:string=${CMAKE_AR}
        CMAKE_CACHE_ARGS -DCMAKE_C_CREATE_STATIC_LIBRARY:internal=${CMAKE_C_CREATE_STATIC_LIBRARY}
        CMAKE_CACHE_ARGS -DCMAKE_C_LINK_EXECUTABLE:string=${CMAKE_C_LINK_EXECUTABLE}
        # Install location
        CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:string=${MBEDTLS_INSTALL_DIR}
    )

# Add an install target to force installation after each mbedtls build. Without
# this target installation happens only when a clean mbedtls build is executed.
add_custom_target(${MBEDTLS_TARGET_NAME}_install
	 COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/mbedtls -- install
	 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mbedtls
	 COMMENT "Installing mbedtls to ${MBEDTLS_INSTALL_DIR}"
	 VERBATIM)
#Make install rule depend on mbedtls library build
add_dependencies(${MBEDTLS_TARGET_NAME}_install ${MBEDTLS_TARGET_NAME})

