# CMake config
cmake_minimum_required(VERSION 2.8.12)
project(DISCON)
enable_language(C)

set(SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/discon.c")
file(TO_CMAKE_PATH ${SOURCE_PATH} SOURCE_PATH)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type: Debug Release" FORCE)
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_SHARE_LINKER_FLAGS "-shared")

#if(APPLE OR UNIX)
#  set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -DIMPLICIT_DLLEXPORT")
#endif()

if(APPLE)
  set(CMAKE_MACOSX_RPATH 1)
endif()

add_library(DISCON SHARED ${SOURCE_PATH})
set_target_properties(DISCON PROPERTIES PREFIX "" SUFFIX ".dll")

# if this project is built standlone
#if (${CMAKE_CURRENT_LIST_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
#  set(INSTALL_DEST "${CMAKE_SOURCE_DIR}/..")
# otherwise
#else()
#  set(INSTALL_DEST "${CMAKE_BINARY_DIR}/reg_tests/glue-codes/openfast/5MW_Baseline/ServoData")
#endif()
#
#install(TARGETS DISCON DESTINATION ${INSTALL_DEST})
