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
CMAKE_BINARY_DIR = //vagrant/apps/plc_led_blink/_build/arm-tfm

# Include any dependencies generated for this target.
include //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/depend.make

# Include the progress variables for this target.
include //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/progress.make

# Include the compile flags for this target's objects.
include //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/flags.make

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/tfm_audit_secure_api.o: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/flags.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/tfm_audit_secure_api.o: //vagrant/ext/trusted-firmware-m/secure_fw/services/audit_logging/tfm_audit_secure_api.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/tfm_audit_secure_api.o"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tfm_audit.dir/tfm_audit_secure_api.o   -c //vagrant/ext/trusted-firmware-m/secure_fw/services/audit_logging/tfm_audit_secure_api.c

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/tfm_audit_secure_api.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tfm_audit.dir/tfm_audit_secure_api.i"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/trusted-firmware-m/secure_fw/services/audit_logging/tfm_audit_secure_api.c > CMakeFiles/tfm_audit.dir/tfm_audit_secure_api.i

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/tfm_audit_secure_api.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tfm_audit.dir/tfm_audit_secure_api.s"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/trusted-firmware-m/secure_fw/services/audit_logging/tfm_audit_secure_api.c -o CMakeFiles/tfm_audit.dir/tfm_audit_secure_api.s

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/audit_core.o: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/flags.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/audit_core.o: //vagrant/ext/trusted-firmware-m/secure_fw/services/audit_logging/audit_core.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/audit_core.o"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tfm_audit.dir/audit_core.o   -c //vagrant/ext/trusted-firmware-m/secure_fw/services/audit_logging/audit_core.c

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/audit_core.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tfm_audit.dir/audit_core.i"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/trusted-firmware-m/secure_fw/services/audit_logging/audit_core.c > CMakeFiles/tfm_audit.dir/audit_core.i

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/audit_core.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tfm_audit.dir/audit_core.s"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/trusted-firmware-m/secure_fw/services/audit_logging/audit_core.c -o CMakeFiles/tfm_audit.dir/audit_core.s

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/audit_wrappers.o: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/flags.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/audit_wrappers.o: //vagrant/ext/trusted-firmware-m/secure_fw/services/audit_logging/audit_wrappers.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/audit_wrappers.o"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tfm_audit.dir/audit_wrappers.o   -c //vagrant/ext/trusted-firmware-m/secure_fw/services/audit_logging/audit_wrappers.c

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/audit_wrappers.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tfm_audit.dir/audit_wrappers.i"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/trusted-firmware-m/secure_fw/services/audit_logging/audit_wrappers.c > CMakeFiles/tfm_audit.dir/audit_wrappers.i

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/audit_wrappers.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tfm_audit.dir/audit_wrappers.s"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/trusted-firmware-m/secure_fw/services/audit_logging/audit_wrappers.c -o CMakeFiles/tfm_audit.dir/audit_wrappers.s

# Object files for target tfm_audit
tfm_audit_OBJECTS = \
"CMakeFiles/tfm_audit.dir/tfm_audit_secure_api.o" \
"CMakeFiles/tfm_audit.dir/audit_core.o" \
"CMakeFiles/tfm_audit.dir/audit_wrappers.o"

# External object files for target tfm_audit
tfm_audit_EXTERNAL_OBJECTS =

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/tfm_audit.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/tfm_audit_secure_api.o
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/tfm_audit.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/audit_core.o
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/tfm_audit.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/audit_wrappers.o
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/tfm_audit.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/build.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/tfm_audit.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C static library //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/tfm_audit.a"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging && $(CMAKE_COMMAND) -P //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/cmake_clean_target.cmake
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging && $(CMAKE_COMMAND) -E cmake_link_script //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/build: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/tfm_audit.a

.PHONY : //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/build

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/clean:
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging && $(CMAKE_COMMAND) -P //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/cmake_clean.cmake
.PHONY : //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/clean

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/depend:
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" //vagrant/ext/trusted-firmware-m //vagrant/ext/trusted-firmware-m/secure_fw/services/audit_logging //vagrant/apps/plc_led_blink/_build/arm-tfm //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/audit_logging/CMakeFiles/tfm_audit.dir/depend

