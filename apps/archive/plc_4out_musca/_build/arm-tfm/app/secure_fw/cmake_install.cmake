# Install script for directory: //vagrant/ext/trusted-firmware-m/secure_fw

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "//vagrant/apps/plc_4out/_build/arm-tfm/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/export/tfm/inc" TYPE DIRECTORY FILES "//vagrant/ext/trusted-firmware-m/secure_fw/../interface/include/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/export/tfm/src" TYPE DIRECTORY FILES "//vagrant/ext/trusted-firmware-m/secure_fw/../interface/src/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/export/tfm/veneers" TYPE FILE FILES "//vagrant/apps/plc_4out/_build/arm-tfm/app/s_veneers.o")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/outputs/MUSCA_A" TYPE FILE FILES
    "//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/tfm_s.axf"
    "//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/tfm_s.bin"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/outputs/fvp" TYPE FILE FILES
    "//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/tfm_s.axf"
    "//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/tfm_s.bin"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("//vagrant/apps/plc_4out/_build/arm-tfm/test/cmake_install.cmake")
  include("//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/crypto/cmake_install.cmake")
  include("//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/secure_storage/cmake_install.cmake")
  include("//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/audit_logging/cmake_install.cmake")
  include("//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/cmake_install.cmake")
  include("//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/initial_attestation/cmake_install.cmake")

endif()

