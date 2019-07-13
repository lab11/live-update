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

if (NOT DEFINED NONSECURE_TARGET)
    message(FATAL_ERROR "app/: mandatory parameter `NONSECURE_TARGET` not defined")
endif()

add_executable(${NONSECURE_TARGET}
    "main.c"
)

# Add include dirs here
#target_include_directories(${NONSECURE_TARGET} PRIVATE
#    "include/"
#)

# Set appropriate compiler and linker flags
target_compile_options(${NONSECURE_TARGET} PUBLIC
    -xc 
    -std=c99
    ${COMMON_C_FLAGS}
    ${ARCH_C_FLAGS}
    -Wall
    -Werror
    -Wno-format
    -Wno-unused-variable
    -Wno-return-type
    -Wno-unused-but-set-variable
)

# Add compile definitions here
target_compile_definitions(${NONSECURE_TARGET} PRIVATE
    TFM_LVL=1
)

# Do weird linker file pre-processing (pp) 
# TODO remove this
message(STATUS "current list dir: ${CMAKE_CURRENT_LIST_DIR}")
set(FINAL_NS_LD_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/musca_ns.ld.i")
set(NS_LD_PP_TARGET "${NONSECURE_TARGET}_ldpp")
add_custom_target(${NS_LD_PP_TARGET}
    COMMENT "Pre-processing linker command file..."
    COMMAND ${CMAKE_C_COMPILER} -E -P -xc "-I${CMAKE_CURRENT_LIST_DIR}/../sdk/musca_a/include" -o ${FINAL_NS_LD_FILE_NAME} "${CMAKE_CURRENT_LIST_DIR}/../make/ld/musca_ns.ld" 
    DEPENDS "${CMAKE_CURRENT_LIST_DIR}/../make/ld/musca_ns.ld"
    BYPRODUCTS ${FINAL_NS_LD_FILE_NAME}
)
add_dependencies(${NONSECURE_TARGET} ${NS_LD_PP_TARGET})
get_directory_property(_ADDITIONAL_MAKE_CLEAN_FILES DIRECTORY "./" ADDITIONAL_MAKE_CLEAN_FILES)
set_directory_properties(PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${_ADDITIONAL_MAKE_CLEAN_FILES} ${FINAL_NS_LD_FILE_NAME}")

target_link_libraries(${NONSECURE_TARGET}
    PUBLIC
        -T ${FINAL_NS_LD_FILE_NAME}
        -Xlinker -check-sections
        -Xlinker -fatal-warnings
        -Xlinker -Map=${NONSECURE_TARGET}.map
        --entry=Reset_Handler
        --specs=nano.specs
        -Wl,--no-wchar-size-warning
        -Wl,--print-memory-usage
        ${ARCH_C_FLAGS}
    PRIVATE
        sdk 
        startup_ns
)

# Generate bin file
add_custom_command(
    TARGET ${NONSECURE_TARGET} POST_BUILD

    COMMAND ${CMAKE_OBJCOPY}
    ARGS -O binary $<TARGET_FILE:${NONSECURE_TARGET}> ${NONSECURE_TARGET}.bin
)

