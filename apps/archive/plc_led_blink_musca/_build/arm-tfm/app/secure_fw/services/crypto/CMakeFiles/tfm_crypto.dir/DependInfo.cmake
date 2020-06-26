# The set of languages for which implicit dependencies are needed:
set(CMAKE_DEPENDS_LANGUAGES
  "C"
  )
# The set of files for implicit dependencies of each language:
set(CMAKE_DEPENDS_CHECK_C
  "//vagrant/ext/trusted-firmware-m/secure_fw/services/crypto/crypto_aead.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/CMakeFiles/tfm_crypto.dir/crypto_aead.o"
  "//vagrant/ext/trusted-firmware-m/secure_fw/services/crypto/crypto_alloc.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/CMakeFiles/tfm_crypto.dir/crypto_alloc.o"
  "//vagrant/ext/trusted-firmware-m/secure_fw/services/crypto/crypto_cipher.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/CMakeFiles/tfm_crypto.dir/crypto_cipher.o"
  "//vagrant/ext/trusted-firmware-m/secure_fw/services/crypto/crypto_engine.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/CMakeFiles/tfm_crypto.dir/crypto_engine.o"
  "//vagrant/ext/trusted-firmware-m/secure_fw/services/crypto/crypto_hash.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/CMakeFiles/tfm_crypto.dir/crypto_hash.o"
  "//vagrant/ext/trusted-firmware-m/secure_fw/services/crypto/crypto_init.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/CMakeFiles/tfm_crypto.dir/crypto_init.o"
  "//vagrant/ext/trusted-firmware-m/secure_fw/services/crypto/crypto_key.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/CMakeFiles/tfm_crypto.dir/crypto_key.o"
  "//vagrant/ext/trusted-firmware-m/secure_fw/services/crypto/crypto_mac.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/CMakeFiles/tfm_crypto.dir/crypto_mac.o"
  "//vagrant/ext/trusted-firmware-m/secure_fw/services/crypto/tfm_crypto_secure_api.c" "//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/CMakeFiles/tfm_crypto.dir/tfm_crypto_secure_api.o"
  )
set(CMAKE_C_COMPILER_ID "GNUARM")

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_C
  "BL2"
  "MBEDTLS_CONFIG_FILE=\"platform/ext/common/tfm_mbedtls_config.h\""
  "TFM_CRYPTO_ENGINE_MBEDTLS"
  "TFM_LEGACY_API"
  "TFM_LVL=3"
  "__ARM_FEATURE_CMSE=3"
  "__thumb2__"
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
  "//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/CMSIS_Driver"
  "//vagrant/ext/trusted-firmware-m/platform/ext/driver"
  "//vagrant/ext/trusted-firmware-m"
  "//vagrant/ext/trusted-firmware-m/interface/include"
  "//vagrant/ext/trusted-firmware-m/secure_fw/spm"
  "//vagrant/ext/trusted-firmware-m/secure_fw/core"
  "//vagrant/ext/trusted-firmware-m/platform/ext/common"
  "//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/mbedtls_install/include"
  )

# Targets to which this target links.
set(CMAKE_TARGET_LINKED_INFO_FILES
  )

# Fortran module output directory.
set(CMAKE_Fortran_TARGET_MODULE_DIR "")
