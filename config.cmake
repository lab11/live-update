set(CMAKE_BUILD_TYPE "Debug")
set(TARGET_PLATFORM "MUSCA_A")

set(CMSIS_5_DIR "${TFM_ROOT_DIR}/../CMSIS_5")
set(PLATFORM_CMAKE_FILE "${TFM_ROOT_DIR}/platform/ext/musca_a.cmake")

# Set ARM TF-M options
set(REGRESSION False)
set(CORE_TEST False)
set(PSA_API_TEST False)

set(TFM_LVL 1)

# Set correct options for Musca-A TFM
set(BL2 True)
set(MCUBOOT_NO_SWAP False)
set(MCUBOOT_RAM_LOADING True)

set(COMPILER "GNUARM")

# Common configuration options (ARM TF-M)
set(BUILD_CMSIS_CORE OFF)
set(BUILD_RETARGET OFF)
set(BUILD_NATIVE_DRIVERS OFF)
set(BUILD_TIME OFF)
set(BUILD_STARTUP OFF)
set(BUILD_TARGET_CFG OFF)
set(BUILD_TARGET_HARDWARE_KEYS OFF)
set(BUILD_TARGET_NV_COUNTERS OFF)
set(BUILD_CMSIS_DRIVERS OFF)
set(BUILD_UART_STDOUT OFF)
set(BUILD_FLASH OFF)
set(BUILD_BOOT_SEED OFF)
set(BUILD_DEVICE_ID OFF)

# might need to copy and edit this file
# include to resolve circular dependencies
include(${PLATFORM_CMAKE_FILE})

include("Common/FindGNUARM")
include("Common/${GNUARM_MODULE}")

if(NOT DEFINED IMAGE_VERSION)
    set(IMAGE_VERSION 0.0.0+0)
endif()

set(SHARED_FLAGS
    -fshort-enums
    -fshort-wchar
    -funsigned-char 
    -msoft-float
    -mcmse
    --specs=nano.specs
)
list_to_string(SHARED_FLAGS_STR ${SHARED_FLAGS})

set(CFLAGS
    ${SHARED_FLAGS}
    -xc
    -std=c99
    -Wall -Werror
    -Wno-format
    -Wno-return-type
    -Wno-unused-but-set-variable
)

# --no-wchar-size-warning flag is added because TF-M sources are compiled with
# short wchars, however the standard library is compiled with normal wchar, and
# this generates linker time warnings. TF-M code does not use wchar, so the
# warning can be suppressed.
set(LDFLAGS
    -Xlinker
    -check-sections
    -Xlinker -fatal-warnings
    --entry=Reset_Handler
    -Wl,--no-wchar-size-warning
    --specs=nano.specs
    -Wl,--print-memory-usage
)

function(config_setting_shared_compiler_flags tgt)
    embedded_set_target_compile_flags(TARGET ${tgt} LANGUAGE C FLAGS ${CFLAGS})
endfunction()

function(config_setting_shared_linker_flags tgt)
    embedded_set_target_link_flags(TARGET ${tgt} FLAGS ${LDFLAGS})
endfunction()

set(TFM_PARTITION_TEST_CORE OFF)
set(CORE_TEST_POSITIVE OFF)
set(CORE_TEST_INTERACTIVE OFF)
set(TEST_FRAMEWORK_S OFF)
set(REFERENCE_PLATFORM OFF)
set(TFM_PARTITION_TEST_SECURE_SERVICES OFF)
set(SERVICES_TEST_ENABLED OFF)
set(TEST_FRAMEWORK_S  OFF)
set(TEST_FRAMEWORK_NS OFF)
set(TFM_PSA_API OFF)
set(TFM_LEGACY_API ON)
set(CORE_TEST_IPC OFF)

set(TFM_NS_CLIENT_IDENTIFICATION OFF)
set(SECURE_UART1 OFF)

# TFM feature configuration
add_definitions(-DTFM_LEGACY_API)
add_definitions(-DBL2)

# Set mbedTLS compiler flags for TFM services
set(MBEDTLS_C_FLAGS_SERVICES "-D__ARM_FEATURE_CMSE=3 -D__thumb2__ ${SHARED_FLAGS_STR} -DMBEDTLS_CONFIG_FILE=\\\\\\\"tfm_mbedtls_config.h\\\\\\\" -I${TFM_ROOT_DIR}/platform/ext/common")

# Secure storage flags
set(SST_ENCRYPTION ON)
set(SST_ROLLBACK_PROTECTION OFF)
set(SST_CREATE_FLASH_LAYOUT OFF)
set(SST_VALIDATE_METADATA_FROM_FLASH ON)
set(SST_RAM_FS OFF)
set(SST_TEST_NV_COUNTERS OFF)

set(MBEDTLS_DEBUG OFF)

# Set mbedTLS compiler flags for BL2 bootloader
set(MBEDTLS_C_FLAGS_BL2 "-D__ARM_FEATURE_CMSE=3 -D__thumb2__ ${SHARED_FLAGS_STR} -DMBEDTLS_CONFIG_FILE=\\\\\\\"config-boot.h\\\\\\\" -I${TFM_ROOT_DIR}/bl2/ext/mcuboot/include")
