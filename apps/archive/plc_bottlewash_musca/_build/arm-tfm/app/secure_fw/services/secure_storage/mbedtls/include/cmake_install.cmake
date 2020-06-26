# Install script for directory: //vagrant/ext/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/services/secure_storage/mbedtls/mbedtls_install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "MinSizeRel")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "//vagrant/ext/mbedtls/include/mbedtls/aes.h"
    "//vagrant/ext/mbedtls/include/mbedtls/aesni.h"
    "//vagrant/ext/mbedtls/include/mbedtls/arc4.h"
    "//vagrant/ext/mbedtls/include/mbedtls/asn1.h"
    "//vagrant/ext/mbedtls/include/mbedtls/asn1write.h"
    "//vagrant/ext/mbedtls/include/mbedtls/base64.h"
    "//vagrant/ext/mbedtls/include/mbedtls/bignum.h"
    "//vagrant/ext/mbedtls/include/mbedtls/blowfish.h"
    "//vagrant/ext/mbedtls/include/mbedtls/bn_mul.h"
    "//vagrant/ext/mbedtls/include/mbedtls/camellia.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ccm.h"
    "//vagrant/ext/mbedtls/include/mbedtls/certs.h"
    "//vagrant/ext/mbedtls/include/mbedtls/check_config.h"
    "//vagrant/ext/mbedtls/include/mbedtls/cipher.h"
    "//vagrant/ext/mbedtls/include/mbedtls/cipher_internal.h"
    "//vagrant/ext/mbedtls/include/mbedtls/cmac.h"
    "//vagrant/ext/mbedtls/include/mbedtls/compat-1.3.h"
    "//vagrant/ext/mbedtls/include/mbedtls/config.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ctr_drbg.h"
    "//vagrant/ext/mbedtls/include/mbedtls/debug.h"
    "//vagrant/ext/mbedtls/include/mbedtls/des.h"
    "//vagrant/ext/mbedtls/include/mbedtls/dhm.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ecdh.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ecdsa.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ecjpake.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ecp.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ecp_internal.h"
    "//vagrant/ext/mbedtls/include/mbedtls/entropy.h"
    "//vagrant/ext/mbedtls/include/mbedtls/entropy_poll.h"
    "//vagrant/ext/mbedtls/include/mbedtls/error.h"
    "//vagrant/ext/mbedtls/include/mbedtls/gcm.h"
    "//vagrant/ext/mbedtls/include/mbedtls/havege.h"
    "//vagrant/ext/mbedtls/include/mbedtls/hmac_drbg.h"
    "//vagrant/ext/mbedtls/include/mbedtls/md.h"
    "//vagrant/ext/mbedtls/include/mbedtls/md2.h"
    "//vagrant/ext/mbedtls/include/mbedtls/md4.h"
    "//vagrant/ext/mbedtls/include/mbedtls/md5.h"
    "//vagrant/ext/mbedtls/include/mbedtls/md_internal.h"
    "//vagrant/ext/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "//vagrant/ext/mbedtls/include/mbedtls/net.h"
    "//vagrant/ext/mbedtls/include/mbedtls/net_sockets.h"
    "//vagrant/ext/mbedtls/include/mbedtls/oid.h"
    "//vagrant/ext/mbedtls/include/mbedtls/padlock.h"
    "//vagrant/ext/mbedtls/include/mbedtls/pem.h"
    "//vagrant/ext/mbedtls/include/mbedtls/pk.h"
    "//vagrant/ext/mbedtls/include/mbedtls/pk_internal.h"
    "//vagrant/ext/mbedtls/include/mbedtls/pkcs11.h"
    "//vagrant/ext/mbedtls/include/mbedtls/pkcs12.h"
    "//vagrant/ext/mbedtls/include/mbedtls/pkcs5.h"
    "//vagrant/ext/mbedtls/include/mbedtls/platform.h"
    "//vagrant/ext/mbedtls/include/mbedtls/platform_time.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ripemd160.h"
    "//vagrant/ext/mbedtls/include/mbedtls/rsa.h"
    "//vagrant/ext/mbedtls/include/mbedtls/rsa_internal.h"
    "//vagrant/ext/mbedtls/include/mbedtls/sha1.h"
    "//vagrant/ext/mbedtls/include/mbedtls/sha256.h"
    "//vagrant/ext/mbedtls/include/mbedtls/sha512.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ssl.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ssl_cache.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ssl_cookie.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ssl_internal.h"
    "//vagrant/ext/mbedtls/include/mbedtls/ssl_ticket.h"
    "//vagrant/ext/mbedtls/include/mbedtls/threading.h"
    "//vagrant/ext/mbedtls/include/mbedtls/timing.h"
    "//vagrant/ext/mbedtls/include/mbedtls/version.h"
    "//vagrant/ext/mbedtls/include/mbedtls/x509.h"
    "//vagrant/ext/mbedtls/include/mbedtls/x509_crl.h"
    "//vagrant/ext/mbedtls/include/mbedtls/x509_crt.h"
    "//vagrant/ext/mbedtls/include/mbedtls/x509_csr.h"
    "//vagrant/ext/mbedtls/include/mbedtls/xtea.h"
    )
endif()

