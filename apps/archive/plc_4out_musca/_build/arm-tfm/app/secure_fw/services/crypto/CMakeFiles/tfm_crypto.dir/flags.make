# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# compile C with /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc
C_FLAGS =    -march=armv8-m.main  -g -O0   -xc -std=c99 -fshort-enums -fshort-wchar -funsigned-char -msoft-float -mcmse --specs=nano.specs -Wall -Werror -Wno-format -Wno-return-type -Wno-unused-but-set-variable

C_DEFINES = -DBL2 -DMBEDTLS_CONFIG_FILE=\"platform/ext/common/tfm_mbedtls_config.h\" -DTFM_CRYPTO_ENGINE_MBEDTLS -DTFM_LEGACY_API -DTFM_LVL=3 -D__ARM_FEATURE_CMSE=3 -D__thumb2__

C_INCLUDES = -I//vagrant/ext/trusted-firmware-m/platform/ext/cmsis -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/CMSIS_Driver/Config -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Config -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Include -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Native_Driver -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/partition -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Libraries -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/CMSIS_Driver -I//vagrant/ext/trusted-firmware-m/platform/ext/driver -I//vagrant/ext/trusted-firmware-m -I//vagrant/ext/trusted-firmware-m/interface/include -I//vagrant/ext/trusted-firmware-m/secure_fw/spm -I//vagrant/ext/trusted-firmware-m/secure_fw/core -I//vagrant/ext/trusted-firmware-m/platform/ext/common -I//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/mbedtls_install/include 

