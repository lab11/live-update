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
CMAKE_BINARY_DIR = //vagrant/apps/plc_4out/_build/arm-tfm

# Include any dependencies generated for this target.
include //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/depend.make

# Include the progress variables for this target.
include //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/progress.make

# Include the compile flags for this target's objects.
include //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/flags.make

//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/platform_sp.o: //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/flags.make
//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/platform_sp.o: //vagrant/ext/trusted-firmware-m/secure_fw/services/platform/platform_sp.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_4out/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/platform_sp.o"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tfm_platform.dir/platform_sp.o   -c //vagrant/ext/trusted-firmware-m/secure_fw/services/platform/platform_sp.c

//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/platform_sp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tfm_platform.dir/platform_sp.i"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/trusted-firmware-m/secure_fw/services/platform/platform_sp.c > CMakeFiles/tfm_platform.dir/platform_sp.i

//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/platform_sp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tfm_platform.dir/platform_sp.s"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/trusted-firmware-m/secure_fw/services/platform/platform_sp.c -o CMakeFiles/tfm_platform.dir/platform_sp.s

//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/tfm_platform_secure_api.o: //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/flags.make
//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/tfm_platform_secure_api.o: //vagrant/ext/trusted-firmware-m/secure_fw/services/platform/tfm_platform_secure_api.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_4out/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/tfm_platform_secure_api.o"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tfm_platform.dir/tfm_platform_secure_api.o   -c //vagrant/ext/trusted-firmware-m/secure_fw/services/platform/tfm_platform_secure_api.c

//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/tfm_platform_secure_api.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tfm_platform.dir/tfm_platform_secure_api.i"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/trusted-firmware-m/secure_fw/services/platform/tfm_platform_secure_api.c > CMakeFiles/tfm_platform.dir/tfm_platform_secure_api.i

//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/tfm_platform_secure_api.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tfm_platform.dir/tfm_platform_secure_api.s"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/trusted-firmware-m/secure_fw/services/platform/tfm_platform_secure_api.c -o CMakeFiles/tfm_platform.dir/tfm_platform_secure_api.s

# Object files for target tfm_platform
tfm_platform_OBJECTS = \
"CMakeFiles/tfm_platform.dir/platform_sp.o" \
"CMakeFiles/tfm_platform.dir/tfm_platform_secure_api.o"

# External object files for target tfm_platform
tfm_platform_EXTERNAL_OBJECTS =

//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/tfm_platform.a: //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/platform_sp.o
//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/tfm_platform.a: //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/tfm_platform_secure_api.o
//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/tfm_platform.a: //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/build.make
//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/tfm_platform.a: //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=//vagrant/apps/plc_4out/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C static library //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/tfm_platform.a"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform && $(CMAKE_COMMAND) -P //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/cmake_clean_target.cmake
	cd //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform && $(CMAKE_COMMAND) -E cmake_link_script //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/build: //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/tfm_platform.a

.PHONY : //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/build

//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/clean:
	cd //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform && $(CMAKE_COMMAND) -P //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/cmake_clean.cmake
.PHONY : //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/clean

//vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/depend:
	cd //vagrant/apps/plc_4out/_build/arm-tfm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" //vagrant/ext/trusted-firmware-m //vagrant/ext/trusted-firmware-m/secure_fw/services/platform //vagrant/apps/plc_4out/_build/arm-tfm //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : //vagrant/apps/plc_4out/_build/arm-tfm/app/secure_fw/services/platform/CMakeFiles/tfm_platform.dir/depend

