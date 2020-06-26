# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# compile ASM with /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc
# compile C with /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc
ASM_FLAGS =    -march=armv8-m.main  -g  

ASM_DEFINES = -DBL2 -DMBEDTLS_CONFIG_FILE=\"config-boot.h\" -DMCUBOOT_RAM_LOADING -DMCUBOOT_SIGN_RSA -DMCUBOOT_TARGET_CONFIG=\"flash_layout.h\" -DMCUBOOT_USE_FLASH_AREA_GET_SECTORS -DMCUBOOT_VALIDATE_SLOT0 -DTFM_LEGACY_API

ASM_INCLUDES = -I//vagrant/ext/trusted-firmware-m/platform/ext/cmsis -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/CMSIS_Driver/Config -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Config -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Include -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Native_Driver -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/partition -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Libraries -I//vagrant/ext/trusted-firmware-m/platform/ext/common -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/CMSIS_Driver -I//vagrant/ext/trusted-firmware-m/platform/ext/driver -I//vagrant/ext/trusted-firmware-m -I//vagrant/ext/trusted-firmware-m/bl2/include -I//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/include -I//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/bootutil/include -I//vagrant/apps/plc_bottlewash/_build/arm-tfm/bl2/ext/mcuboot/mbedtls/mbedtls_install/include 

C_FLAGS =    -march=armv8-m.main  -g -O0   -xc -std=c99 -fshort-enums -fshort-wchar -funsigned-char -msoft-float -mcmse --specs=nano.specs -Wall -Werror -Wno-format -Wno-return-type -Wno-unused-but-set-variable

C_DEFINES = -DBL2 -DMBEDTLS_CONFIG_FILE=\"config-boot.h\" -DMCUBOOT_RAM_LOADING -DMCUBOOT_SIGN_RSA -DMCUBOOT_TARGET_CONFIG=\"flash_layout.h\" -DMCUBOOT_USE_FLASH_AREA_GET_SECTORS -DMCUBOOT_VALIDATE_SLOT0 -DTFM_LEGACY_API

C_INCLUDES = -I//vagrant/ext/trusted-firmware-m/platform/ext/cmsis -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/CMSIS_Driver/Config -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Config -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Include -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Native_Driver -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/partition -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Libraries -I//vagrant/ext/trusted-firmware-m/platform/ext/common -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/CMSIS_Driver -I//vagrant/ext/trusted-firmware-m/platform/ext/driver -I//vagrant/ext/trusted-firmware-m -I//vagrant/ext/trusted-firmware-m/bl2/include -I//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/include -I//vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot/bootutil/include -I//vagrant/apps/plc_bottlewash/_build/arm-tfm/bl2/ext/mcuboot/mbedtls/mbedtls_install/include 

# Custom defines: //vagrant/apps/plc_bottlewash/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot.dir/__/__/__/platform/ext/target/musca_a/Device/Source/gcc/startup_cmsdk_musca_bl2.S.obj_DEFINES = __STARTUP_CLEAR_BSS_MULTIPLE;__STARTUP_COPY_MULTIPLE

