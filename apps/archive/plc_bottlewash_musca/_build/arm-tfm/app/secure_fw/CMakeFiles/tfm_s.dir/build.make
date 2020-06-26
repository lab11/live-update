# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = //vagrant/ext/trusted-firmware-m

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = //vagrant/apps/plc_bottlewash/_build/arm-tfm

# Include any dependencies generated for this target.
include //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/depend.make

# Include the progress variables for this target.
include //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/progress.make

# Include the compile flags for this target's objects.
include //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/flags.make

# Object files for target tfm_s
tfm_s_OBJECTS =

# External object files for target tfm_s
tfm_s_EXTERNAL_OBJECTS = \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/spm/spm_api.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/ns_callable/tfm_veneers.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/ns_callable/tfm_audit_veneers.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/ns_callable/tfm_gpio_veneers.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/ns_callable/tfm_flash_veneers.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/ns_callable/tfm_platform_veneers.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/core/tfm_core.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/core/tfm_handler.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/core/tfm_secure_api.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/core/tfm_spm_services.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/core/tfm_nspm.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/core/tfm_boot_data.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Device/Source/system_core_init.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Device/Source/device_definition.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/common/uart_stdout.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Native_Driver/uart_pl011_drv.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Native_Driver/musca_a1_scc_drv.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Native_Driver/qspi_ip6514e_drv.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Libraries/mt25ql_flash_lib.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/target_cfg.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/common/tfm_initial_attestation_key_material.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/dummy_crypto_keys.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/dummy_nv_counters.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/CMSIS_Driver/Driver_USART.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/CMSIS_Driver/Driver_QSPI_Flash.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/dummy_boot_seed.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/dummy_device_id.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Native_Driver/mpc_sie200_drv.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Native_Driver/ppc_sse200_drv.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/spm_hal.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/attest_hal.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Native_Driver/mpu_armv8m_drv.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/tfm_platform_system.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/CMSIS_Driver/Driver_MPC.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/CMSIS_Driver/Driver_PPC.o" \
"//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Device/Source/gcc/startup_cmsdk_musca_s.S.obj"

//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/spm/spm_api.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/ns_callable/tfm_veneers.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/ns_callable/tfm_audit_veneers.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/ns_callable/tfm_gpio_veneers.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/ns_callable/tfm_flash_veneers.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/ns_callable/tfm_platform_veneers.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/core/tfm_core.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/core/tfm_handler.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/core/tfm_secure_api.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/core/tfm_spm_services.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/core/tfm_nspm.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/core/tfm_boot_data.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Device/Source/system_core_init.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Device/Source/device_definition.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/common/uart_stdout.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Native_Driver/uart_pl011_drv.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Native_Driver/musca_a1_scc_drv.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Native_Driver/qspi_ip6514e_drv.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Libraries/mt25ql_flash_lib.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/target_cfg.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/common/tfm_initial_attestation_key_material.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/dummy_crypto_keys.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/dummy_nv_counters.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/CMSIS_Driver/Driver_USART.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/CMSIS_Driver/Driver_QSPI_Flash.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/dummy_boot_seed.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/dummy_device_id.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Native_Driver/mpc_sie200_drv.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Native_Driver/ppc_sse200_drv.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/spm_hal.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/attest_hal.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Native_Driver/mpu_armv8m_drv.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/tfm_platform_system.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/CMSIS_Driver/Driver_MPC.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/CMSIS_Driver/Driver_PPC.o
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s_obj_lib.dir/__/platform/ext/target/musca_a/Device/Source/gcc/startup_cmsdk_musca_s.S.obj
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/build.make
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/services/initial_attestation/tfm_attest.a
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/services/crypto/tfm_crypto.a
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/services/secure_storage/tfm_storage.a
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/services/audit_logging/tfm_audit.a
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/services/platform/tfm_platform.a
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/services/initial_attestation/tfm_attest.a
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.ld.i
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=//vagrant/apps/plc_bottlewash/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Linking C executable //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf"
	cd //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw && $(CMAKE_COMMAND) -E cmake_link_script //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/link.txt --verbose=$(VERBOSE)
	cd //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-objcopy -O binary //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.bin

# Rule to build all files generated by this target.
//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/build: //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/tfm_s.axf

.PHONY : //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/build

//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/clean:
	cd //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw && $(CMAKE_COMMAND) -P //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/cmake_clean.cmake
.PHONY : //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/clean

//vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/depend:
	cd //vagrant/apps/plc_bottlewash/_build/arm-tfm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" //vagrant/ext/trusted-firmware-m //vagrant/ext/trusted-firmware-m/secure_fw //vagrant/apps/plc_bottlewash/_build/arm-tfm //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : //vagrant/apps/plc_bottlewash/_build/arm-tfm/app/secure_fw/CMakeFiles/tfm_s.dir/depend

