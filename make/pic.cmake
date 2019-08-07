# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)

# ---- PIC app ---- 
zephyr_get_compile_options_for_lang(C Z_COMPILE_OPTIONS)
set(PIC_FLAGS
    -fPIE
    -msingle-pic-base
    -mpic-register=r9
    -mno-pic-data-is-text-relative
)
set(LINKER_FLAGS
    -Wl,--emit-relocs
    -Wl,--unresolved-symbols=ignore-in-object-files -nostartfiles
    -pie
    -Wl,-gc-sections
)

add_executable(pic)
# Link zephyr first; we need the map file to resolve external function calls
add_dependencies(pic zephyr)
# Make sure we compile the pic object first
add_dependencies(app pic)

set_property(TARGET pic PROPERTY RUNTIME_OUTPUT_DIRECTORY app)

# Set options
target_compile_options(pic PRIVATE
    ${Z_COMPILE_OPTIONS}
    ${PIC_FLAGS}
)
target_link_libraries(pic
    -T ${LIVE_UPDATE_ROOT}/make/ld/pic.ld
    ${LINKER_FLAGS}
)
set_property(
    TARGET pic
    PROPERTY SUFFIX ".elf"
)

# Resolve external functions
execute_process(COMMAND git rev-parse HEAD
    OUTPUT_VARIABLE GIT_REV
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${LIVE_UPDATE_ROOT}
)

set(PIC_APP_ELF $<TARGET_FILE_DIR:pic>/relocated_$<TARGET_FILE_NAME:pic>)
add_custom_command(
    TARGET pic POST_BUILD
    COMMAND python
    ARGS relocate.py $<TARGET_FILE:pic> ${LIVE_UPDATE_ROOT}/.update/${GIT_REV}/zephyr.map ${LIVE_UPDATE_ROOT}/cmake_build/secure/secure.map -o ${PIC_APP_ELF}
    WORKING_DIRECTORY ${LIVE_UPDATE_ROOT}/scripts
)

# Generate binary file
add_custom_command(
    TARGET pic POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}
    ARGS -O binary ${PIC_APP_ELF} $<TARGET_FILE_DIR:pic>/pic.bin
)

# Link into binary object
add_custom_command(
    TARGET pic POST_BUILD
    COMMAND ${CMAKE_LINKER}
    ARGS -r -b binary -o $<TARGET_FILE_DIR:pic>/temp.bin.o $<TARGET_FILE_DIR:pic>/pic.bin
)

# Rename data section
add_custom_command(
    TARGET pic POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}
    ARGS --rename-section .data=.picapp $<TARGET_FILE_DIR:pic>/temp.bin.o $<TARGET_FILE_DIR:pic>/pic.bin.o
    BYPRODUCTS app/pic.bin.o
)

# ---- Non-PIC App ----

target_include_directories(app PRIVATE
    ${LIVE_UPDATE_ROOT}/app/include/
)

target_sources(app PRIVATE
    ${LIVE_UPDATE_ROOT}/app/src/loader.c
    $<TARGET_FILE_DIR:pic>/pic.bin.o
)

# Set aside special section for embedded PIC application
zephyr_linker_sources(SECTIONS ${LIVE_UPDATE_ROOT}/make/ld/app_section.ld)

