# Install script for directory: //vagrant/ext/trusted-firmware-m/app

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "//vagrant/apps/plc_bottlewash/_build/arm-tfm/install")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/outputs/MUSCA_A" TYPE FILE FILES "//vagrant/apps/plc_bottlewash/_build/arm-tfm/tfm_sign.bin")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/outputs/MUSCA_A" TYPE FILE FILES "//vagrant/apps/plc_bottlewash/_build/arm-tfm/tfm_full.bin")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/outputs/fvp" TYPE FILE RENAME "tfm_s_ns_concatenated.bin" FILES "//vagrant/apps/plc_bottlewash/_build/arm-tfm/tfm_full.bin")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/outputs/fvp" TYPE FILE RENAME "tfm_s_ns_signed.bin" FILES "//vagrant/apps/plc_bottlewash/_build/arm-tfm/tfm_sign.bin")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/outputs/MUSCA_A" TYPE FILE FILES
    "//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/tfm_ns.axf"
    "//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/tfm_ns.bin"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/outputs/fvp" TYPE FILE FILES
    "//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/tfm_ns.axf"
    "//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/tfm_ns.bin"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/cmake_install.cmake")

endif()

