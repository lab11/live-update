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

if (NOT DEFINED SECURE_TARGET)
    message(FATAL_ERROR "secure/: mandatory parameter `SECURE_TARGET` not defined")
endif()

add_executable(${SECURE_TARGET}
    "main.c"
)

# Add include dirs here
#target_include_directories(${SECURE_TARGET} PRIVATE
#    "include/"
#)

# Set appropriate compiler and linker flags
target_compile_options(${SECURE_TARGET} PUBLIC
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
target_compile_definitions(${SECURE_TARGET} PRIVATE
    TFM_LVL=1
)

# Do weird linker file pre-processing (pp) 
# TODO remove this
set(FINAL_S_LD_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/musca_s.ld.i")
set(S_LD_PP_TARGET "${SECURE_TARGET}_ldpp")
add_custom_target(${S_LD_PP_TARGET}
    COMMENT "Pre-processing linker command file..."
    COMMAND ${CMAKE_C_COMPILER} -E -P -xc "-I${CMAKE_CURRENT_LIST_DIR}/../sdk/musca_a/include" -o ${FINAL_S_LD_FILE_NAME} "${CMAKE_CURRENT_LIST_DIR}/../make/ld/musca_s.ld" 
    DEPENDS "${CMAKE_CURRENT_LIST_DIR}/../make/ld/musca_s.ld"
    BYPRODUCTS ${FINAL_S_LD_FILE_NAME}
)
add_dependencies(${SECURE_TARGET} ${S_LD_PP_TARGET})
get_directory_property(_ADDITIONAL_MAKE_CLEAN_FILES DIRECTORY "./" ADDITIONAL_MAKE_CLEAN_FILES)
set_directory_properties(PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${_ADDITIONAL_MAKE_CLEAN_FILES} ${FINAL_S_LD_FILE_NAME}")

target_link_libraries(${SECURE_TARGET}
    PUBLIC
        -T ${FINAL_S_LD_FILE_NAME}
        -Xlinker -check-sections
        -Xlinker -fatal-warnings
        -Xlinker -Map=${SECURE_TARGET}.map
        --entry=Reset_Handler
        --specs=nano.specs
        -Wl,--no-wchar-size-warning
        -Wl,--print-memory-usage
        ${ARCH_C_FLAGS}
    PRIVATE
        sdk 
        startup_s
)

# Generate bin file
add_custom_command(
    TARGET ${SECURE_TARGET} POST_BUILD

    COMMAND ${CMAKE_OBJCOPY}
    ARGS -O binary $<TARGET_FILE:${SECURE_TARGET}> ${SECURE_TARGET}.bin
)

