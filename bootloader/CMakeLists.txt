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

get_filename_component(STRIPPED_LIST_DIR ${CMAKE_CURRENT_LIST_DIR} NAME)
project("${STRIPPED_LIST_DIR}" LANGUAGES C ASM)

add_executable(${PROJECT_NAME}
    "src/bl2_main.c"
    "src/boot_record.c"
    "src/flash_map.c"
    "src/keys.c"
    "bootutil/src/loader.c"
    "bootutil/src/bootutil_misc.c"
    "bootutil/src/image_validate.c"
    "bootutil/src/image_rsa.c"
    "bootutil/src/caps.c"
)

# Build mbedTLS submodule using ARM cmake system
set(MBEDTLS_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../ext/mbedtls")
set(MBEDTLS_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/mbedtls")
set(MBEDTLS_INSTALL_DIR "${MBEDTLS_BINARY_DIR}/mbedtls_install")
list(JOIN COMMON_C_FLAGS " " COMMON_C_FLAGS_STR)
list(JOIN ARCH_C_FLAGS " " ARCH_C_FLAGS_STR)
set(MBEDTLS_C_FLAGS
    "-D__ARM_FEATURE_CMSE=3 -D__thumb2__ ${COMMON_C_FLAGS_STR} ${ARCH_C_FLAGS_STR} -DMBEDTLS_CONFIG_FILE=\\\\\\\"config-boot.h\\\\\\\" -I${CMAKE_CURRENT_LIST_DIR}/include"
    )
set(MBEDTLS_TARGET_NAME "mbedtls_mcuboot_lib")
set(MBEDTLS_DEBUG "OFF")
set(ARM_CPU_ARCHITECTURE "ARMv8-M.MAIN")
include("build_mbedtls")

target_include_directories(${PROJECT_NAME} PRIVATE
    "include/"
    "bootutil/include"    
    "${MBEDTLS_INSTALL_DIR}/include"
)

# Set appropriate compiler and linker flags
target_compile_options(${PROJECT_NAME} PUBLIC
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

target_compile_definitions(${PROJECT_NAME} PRIVATE
    MBEDTLS_CONFIG_FILE="config-boot.h"
    MCUBOOT_RAM_LOADING
    MCUBOOT_SIGN_RSA
    MCUBOOT_TARGET_CONFIG="include/flash_layout.h"
    MCUBOOT_USE_FLASH_AREA_GET_SECTORS
    MCUBOOT_VALIDATE_SLOT0
)

# Specify project dependencies
set(MBEDCRYPTO_LIB_PATH "${MBEDTLS_INSTALL_DIR}/lib/libmbedcrypto.a")

# Do weird linker file pre-processing (pp) 
# TODO remove this
set(FINAL_BL_LD_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/musca_bootloader.ld.i")
set(BL_LD_PP_TARGET "${PROJECT_NAME}_ldpp")
add_custom_target(${BL_LD_PP_TARGET}
    COMMENT "Pre-processing linker command file..."
    COMMAND ${CMAKE_C_COMPILER} -E -P -xc "-I${CMAKE_CURRENT_LIST_DIR}/../sdk/musca_a/include" -o ${FINAL_BL_LD_FILE_NAME} "${CMAKE_CURRENT_LIST_DIR}/../make/ld/musca_bootloader.ld" 
    DEPENDS "${CMAKE_CURRENT_LIST_DIR}/../make/ld/musca_bootloader.ld"
    BYPRODUCTS ${FINAL_BL_LD_FILE_NAME}
)
add_dependencies(${PROJECT_NAME} ${BL_LD_PP_TARGET})
get_directory_property(_ADDITIONAL_MAKE_CLEAN_FILES DIRECTORY "./" ADDITIONAL_MAKE_CLEAN_FILES)
set_directory_properties(PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${_ADDITIONAL_MAKE_CLEAN_FILES} ${FINAL_BL_LD_FILE_NAME}")

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        -T ${FINAL_BL_LD_FILE_NAME}
        -Xlinker -check-sections
        -Xlinker -fatal-warnings
        -Xlinker -Map=bootloader.map
        --entry=Reset_Handler
        --specs=nano.specs
        -Wl,--no-wchar-size-warning
        -Wl,--print-memory-usage
        ${ARCH_C_FLAGS}
    PRIVATE
        sdk 
        startup_bootloader
        ${MBEDCRYPTO_LIB_PATH}
)
add_dependencies(${PROJECT_NAME} ${MBEDTLS_TARGET_NAME}_install)

# Generate bin file 
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}
    ARGS -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
)

# Generate hex file for flashing
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD 
    DEPENDS ${PROJECT_NAME}.bin
    COMMAND srec_cat
    ARGS ${PROJECT_NAME}.bin -Binary -offset 0x200000 -o ${PROJECT_NAME}.hex -Intel
)

# make flash
add_custom_target(bootloader-flash
    COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex /Volumes/MBED/
)
add_dependencies(bootloader-flash ${PROJECT_NAME})

# make startdebug
set(GDB_DEBUG_SCRIPT "../../make/debug.gdb")
add_custom_target(bootloader-startdebug
    COMMAND ${GDBSERVER} ${GDBSERVER_ARGS} &
    COMMAND sleep 2
    COMMAND ${GDBCLIENT} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf --command=${GDB_DEBUG_SCRIPT}
)
add_dependencies(bootloader-startdebug ${PROJECT_NAME})

