
cmake_minimum_required(VERSION 3.1...3.14)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Add source files here
add_library(sdk
    "driver/src/uart_stdout.c" 
    "tfm/src/tfm_boot_data.c"
    "tfm/src/tfm_core.c"
    "tfm/src/tfm_handler.c"
    "tfm/src/tfm_nspm.c"
    "tfm/src/tfm_initial_attestation_key_material.c"
    "tfm/src/tfm_spm_services.c"
    "musca_a/src/Driver_MPC.c"
    "musca_a/src/Driver_PPC.c"
    "musca_a/src/Driver_QSPI_Flash.c"
    "musca_a/src/Driver_USART.c"
    "musca_a/src/attest_hal.c"
    "musca_a/src/device_definition.c"
    "musca_a/src/dummy_boot_seed.c"
    "musca_a/src/dummy_crypto_keys.c"
    "musca_a/src/dummy_device_id.c"
    "musca_a/src/dummy_nv_counters.c"
    "musca_a/src/gpio_cmsdk_drv.c"
    "musca_a/src/mpc_sie200_drv.c"
    "musca_a/src/mpu_armv8m_drv.c"
    "musca_a/src/mt25ql_flash_lib.c"
    "musca_a/src/musca_a1_scc_drv.c"
    "musca_a/src/ppc_sse200_drv.c"
    "musca_a/src/qspi_ip6514e_drv.c"
    "musca_a/src/spm_hal.c"
    "musca_a/src/system_core_init.c"
    "musca_a/src/target_cfg.c"
    "musca_a/src/tfm_platform_system.c"
    "musca_a/src/timer_cmsdk_drv.c"
    "musca_a/src/uart_pl011_drv.c"
)

target_include_directories(sdk PUBLIC
    "cmsis/"
    "tfm/include/"
    "driver/include/"
    "musca_a/include/"
)

target_compile_options(sdk PUBLIC
    -xc
    -std=c99
    ${COMMON_C_FLAGS}
    ${ARCH_C_FLAGS}
)

target_compile_definitions(sdk PRIVATE
    TFM_LVL=1
    DAUTH_CHIP_DEFAULT APPEND
)

