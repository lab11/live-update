# The set of languages for which implicit dependencies are needed:
set(CMAKE_DEPENDS_LANGUAGES
  "ASM"
  "C"
  )
# The set of files for implicit dependencies of each language:
set(CMAKE_DEPENDS_CHECK_ASM
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Source/gcc/startup_cmsdk_musca_bl2.S" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/__/__/__/platform/ext/target/musca_a/Device/Source/gcc/startup_cmsdk_musca_bl2.S.obj"
  )
set(CMAKE_ASM_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_ASM
  "BL2"
  "MBEDTLS_CONFIG_FILE=\"config-boot.h\""
  "MCUBOOT_RAM_LOADING"
  "MCUBOOT_SIGN_RSA"
  "MCUBOOT_TARGET_CONFIG=\"flash_layout.h\""
  "MCUBOOT_USE_FLASH_AREA_GET_SECTORS"
  "MCUBOOT_VALIDATE_SLOT0"
  "TFM_LEGACY_API"
  )

# The include file search paths:
set(CMAKE_ASM_TARGET_INCLUDE_PATH
  "//vagrant/ext/trusted-firmware-m/platform/ext/cmsis"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/CMSIS_Driver/Config"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Config"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Include"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Native_Driver"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/partition"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Libraries"
  "//vagrant/ext/trusted-firmware-m/platform/ext/common"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/CMSIS_Driver"
  "//vagrant/ext/trusted-firmware-m/platform/ext/driver"
  "//vagrant/ext/trusted-firmware-m"
  "//vagrant/ext/trusted-firmware-m/bl2/include"
  "//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/include"
  "//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/bootutil/include"
  "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/mbedtls/mbedtls_install/include"
  )
set(CMAKE_DEPENDS_CHECK_C
  "//vagrant/ext/trusted-firmware-m/platform/ext/common/uart_stdout.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/__/__/__/platform/ext/common/uart_stdout.o"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/CMSIS_Driver/Driver_QSPI_Flash.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/__/__/__/platform/ext/target/musca_a/CMSIS_Driver/Driver_QSPI_Flash.o"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/CMSIS_Driver/Driver_USART.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/__/__/__/platform/ext/target/musca_a/CMSIS_Driver/Driver_USART.o"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Source/device_definition.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/__/__/__/platform/ext/target/musca_a/Device/Source/device_definition.o"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Source/system_core_init.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/__/__/__/platform/ext/target/musca_a/Device/Source/system_core_init.o"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Libraries/mt25ql_flash_lib.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/__/__/__/platform/ext/target/musca_a/Libraries/mt25ql_flash_lib.o"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Native_Driver/musca_a1_scc_drv.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/__/__/__/platform/ext/target/musca_a/Native_Driver/musca_a1_scc_drv.o"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Native_Driver/qspi_ip6514e_drv.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/__/__/__/platform/ext/target/musca_a/Native_Driver/qspi_ip6514e_drv.o"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Native_Driver/uart_pl011_drv.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/__/__/__/platform/ext/target/musca_a/Native_Driver/uart_pl011_drv.o"
  "//vagrant/ext/trusted-firmware-m/bl2/src/boot_record.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/__/__/src/boot_record.o"
  "//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/bl2_main.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/bl2_main.o"
  "//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/bootutil/src/bootutil_misc.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/bootutil/src/bootutil_misc.o"
  "//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/bootutil/src/caps.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/bootutil/src/caps.o"
  "//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/bootutil/src/image_rsa.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/bootutil/src/image_rsa.o"
  "//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/bootutil/src/image_validate.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/bootutil/src/image_validate.o"
  "//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/bootutil/src/loader.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/bootutil/src/loader.o"
  "//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/flash_map.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/flash_map.o"
  "//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/keys.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/keys.o"
  )
set(CMAKE_C_COMPILER_ID "GNUARM")

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_C
  "BL2"
  "MBEDTLS_CONFIG_FILE=\"config-boot.h\""
  "MCUBOOT_RAM_LOADING"
  "MCUBOOT_SIGN_RSA"
  "MCUBOOT_TARGET_CONFIG=\"flash_layout.h\""
  "MCUBOOT_USE_FLASH_AREA_GET_SECTORS"
  "MCUBOOT_VALIDATE_SLOT0"
  "TFM_LEGACY_API"
  )

# The include file search paths:
set(CMAKE_C_TARGET_INCLUDE_PATH
  "//vagrant/ext/trusted-firmware-m/platform/ext/cmsis"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/CMSIS_Driver/Config"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Config"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Include"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Native_Driver"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/partition"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Libraries"
  "//vagrant/ext/trusted-firmware-m/platform/ext/common"
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/CMSIS_Driver"
  "//vagrant/ext/trusted-firmware-m/platform/ext/driver"
  "//vagrant/ext/trusted-firmware-m"
  "//vagrant/ext/trusted-firmware-m/bl2/include"
  "//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/include"
  "//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/bootutil/include"
  "//vagrant/apps/plc_led_blink/_build/arm-tfm/bl2/ext/mcuboot/mbedtls/mbedtls_install/include"
  )

# Targets to which this target links.
set(CMAKE_TARGET_LINKED_INFO_FILES
  )

# Fortran module output directory.
set(CMAKE_Fortran_TARGET_MODULE_DIR "")
