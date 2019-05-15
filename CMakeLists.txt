#-------------------------------------------------------------------------------
# Copyright (c) 2017-2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.7)

get_filename_component(TFM_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../arm-tfm/trusted-firmware-m ABSOLUTE)

# Tell cmake where our modules can be found
list(APPEND CMAKE_MODULE_PATH ${TFM_ROOT_DIR}/cmake)

# Include common stuff to control cmake.
include("Common/BuildSys")

set(PROJ_CONFIG "${CMAKE_CURRENT_LIST_DIR}/config.cmake")
embedded_project_start(${PROJ_CONFIG})

project(tfm_ns LANGUAGES ASM C)
embedded_project_fixup()

# include platform again to build features
set(BUILD_CMSIS_CORE ON)
set(BUILD_RETARGET ON)
set(BUILD_NATIVE_DRIVERS ON)
set(BUILD_STARTUP ON)
set(BUILD_CMSIS_DRIVERS ON)
include(${PLATFORM_CMAKE_FILE})

# Set up various directories
set(APP_DIR ${CMAKE_CURRENT_LIST_DIR})
set(INTERFACE_DIR ${TFM_ROOT_DIR}/interface)

if (NOT DEFINED BL2)
	message(FATAL_ERROR "Incomplete build configuration: BL2 is undefined. ")
endif()
set(MCUBOOT_DIR ${TFM_ROOT_DIR}/bl2/ext/mcuboot)

if(NOT EXISTS ${CMSIS_5_DIR})
	message(FATAL_ERROR "Missing CMSIS_5. Please clone the CMSIS_5 repo to directory \"${CMSIS_5_DIR}\".")
endif()
get_filename_component(CMSIS_5_DIR ${TFM_ROOT_DIR}/../CMSIS_5 ABSOLUTE)

# Generate functions for payload creation
message("DIR: ${MCUBOOT_DIR}/MCUBoot.cmake")
include("${MCUBOOT_DIR}/MCUBoot.cmake")

set(NS_APP_SRC
    "${CMSIS_5_DIR}/CMSIS/RTOS2/RTX/Config/RTX_Config.c"
	"${CMSIS_5_DIR}/CMSIS/RTOS2/RTX/Source/rtx_lib.c"
	"${APP_DIR}/main_ns.c"
    "${APP_DIR}/blink.c"
    "${APP_DIR}/update.c"
    "${INTERFACE_DIR}/src/tfm_ns_lock_rtx.c"
)

if(NOT DEFINED NS_SCATTER_FILE_NAME)
	message(FATAL_ERROR "ERROR: Incomplete Configuration: NS_SCATTER_FILE_NAME not defined, Include this file from a Config*.cmake")
endif()
embedded_set_target_linker_file(TARGET ${PROJECT_NAME} PATH "${NS_SCATTER_FILE_NAME}")

#Create an object library to avoid compiling all source files twice, when two executables
#with different memory map need to be linked(BL2 non-swapping)
set(PROJECT_OBJ_LIB ${PROJECT_NAME}_obj_lib)
add_library(${PROJECT_OBJ_LIB} OBJECT ${ALL_SRC_C} ${ALL_SRC_C_NS} ${ALL_SRC_ASM_NS} ${NS_APP_SRC})

#Set common compiler flags
config_setting_shared_compiler_flags(${PROJECT_OBJ_LIB})

#Set macro definitions
target_compile_definitions(${PROJECT_OBJ_LIB} PRIVATE __thumb2__ __DOMAIN_NS=1 __ARM_FEATURE_CMSE=3 LOG_MSG_HANDLER_MODE_PRINTF_ENABLED)

#Set include directories.
embedded_target_include_directories(TARGET ${PROJECT_OBJ_LIB} PATH ${INTERFACE_DIR}/include ABSOLUTE APPEND)
embedded_target_include_directories(TARGET ${PROJECT_OBJ_LIB} PATH ${TFM_ROOT_DIR} ABSOLUTE APPEND)
embedded_target_include_directories(TARGET ${PROJECT_OBJ_LIB} PATH ${TFM_ROOT_DIR}/secure_fw/spm ABSOLUTE APPEND)
embedded_target_include_directories(TARGET ${PROJECT_OBJ_LIB} PATH ${TFM_ROOT_DIR}/secure_fw/core ABSOLUTE APPEND)
embedded_target_include_directories(TARGET ${PROJECT_OBJ_LIB} PATH ${CMSIS_5_DIR}/CMSIS/RTOS2/RTX/Include ABSOLUTE APPEND)
embedded_target_include_directories(TARGET ${PROJECT_OBJ_LIB} PATH ${CMSIS_5_DIR}/CMSIS/RTOS2/Include ABSOLUTE APPEND)
embedded_target_include_directories(TARGET ${PROJECT_OBJ_LIB} PATH ${CMSIS_5_DIR}/CMSIS/RTOS2/RTX/Config ABSOLUTE APPEND)

# Link application to primary memory region
set(EXE_NAME ${PROJECT_NAME})
set(S_BIN tfm_s)
set(FULL_NAME tfm_full)
set(SIGN_NAME tfm_sign)
set(VENEER_NAME s_veneers.o)

# Create linker target: add object library to executable
add_executable(${EXE_NAME} $<TARGET_OBJECTS:${PROJECT_OBJ_LIB}>)

# Set common linker flags
config_setting_shared_linker_flags(${EXE_NAME})
embedded_set_target_linker_file(TARGET ${EXE_NAME} PATH "${NS_SCATTER_FILE_NAME}")

# Add the RTX library
if(NOT DEFINED RTX_LIB_PATH)
    message(FATAL_ERROR "ERROR: Incomplete Configuration: RTX_LIB_PATH is not defined.")
endif()
target_link_libraries(${EXE_NAME} "${RTX_LIB_PATH}")

if(NOT DEFINED PLATFORM_LINK_INCLUDES)
    message(FATAL_ERROR "ERROR: Incomplete Configuration: PLATFORM_LINK_INCLUDES is not defined.")
endif()
embedded_set_target_link_includes(TARGET ${EXE_NAME} INCLUDES "${PLATFORM_LINK_INCLUDES}")

# Generate binary file from axf
compiler_generate_binary_output(${EXE_NAME})

# Generate MCUBoot compatible payload
mcuboot_create_boot_payload(S_BIN    ${S_BIN}
                            NS_BIN   ${EXE_NAME}
                            FULL_BIN ${FULL_NAME}
                            SIGN_BIN ${SIGN_NAME}
                            POSTFIX  "")
embedded_set_target_link_defines(TARGET ${EXE_NAME} DEFINES "BL2")

# Ensure secure_fw is built before our executable.
add_dependencies(${EXE_NAME} ${S_BIN})

# Add the veneers to the executable.
set(S_VENEER_FILE "${CMAKE_CURRENT_BINARY_DIR}/${VENEER_NAME}")
set_property(TARGET ${EXE_NAME} APPEND PROPERTY LINK_LIBRARIES ${S_VENEER_FILE})

# Collect executables to common location: build/install/outputs/
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.axf
              ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.bin
        DESTINATION outputs/${TARGET_PLATFORM}/)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.axf
              ${CMAKE_CURRENT_BINARY_DIR}/${EXE_NAME}.bin
        DESTINATION outputs/fvp/)

if (NOT TARGET_TFM_S_EXISTED)
	set(S_VENEER_FILE_LOCATION "${CMAKE_CURRENT_BINARY_DIR}")
    add_subdirectory(${TFM_ROOT_DIR}/secure_fw ${CMAKE_CURRENT_BINARY_DIR}/secure_fw)
endif()

# Finally let CMake system apply changes after the whole project is defined.
embedded_project_end(${PROJECT_NAME})
embedded_project_end(${PROJECT_OBJ_LIB})

# MCUBoot
add_subdirectory(${TFM_ROOT_DIR}/bl2/ext/mcuboot bl2/ext/mcuboot)

