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

# Utility rule file for mcuboot_ldpp.

# Include the progress variables for this target.
include //vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot_ldpp.dir/progress.make

//vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot_ldpp: //vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Source/gcc/musca_bl2.ld
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=//vagrant/apps/plc_4out/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Pre-processing linker command file //vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Source/gcc/musca_bl2.ld..."
	cd //vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc -E -P -xc -DBL2 -I//vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/partition -o //vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot/mcuboot.ld.i //vagrant/ext/trusted-firmware-m/platform/ext/target/musca_a/Device/Source/gcc/musca_bl2.ld

mcuboot_ldpp: //vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot_ldpp
mcuboot_ldpp: //vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot_ldpp.dir/build.make

.PHONY : mcuboot_ldpp

# Rule to build all files generated by this target.
//vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot_ldpp.dir/build: mcuboot_ldpp

.PHONY : //vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot_ldpp.dir/build

//vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot_ldpp.dir/clean:
	cd //vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot && $(CMAKE_COMMAND) -P //vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot_ldpp.dir/cmake_clean.cmake
.PHONY : //vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot_ldpp.dir/clean

//vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot_ldpp.dir/depend:
	cd //vagrant/apps/plc_4out/_build/arm-tfm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" //vagrant/ext/trusted-firmware-m //vagrant/ext/trusted-firmware-m/bl2/ext/mcuboot //vagrant/apps/plc_4out/_build/arm-tfm //vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot //vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot_ldpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : //vagrant/apps/plc_4out/_build/arm-tfm/bl2/ext/mcuboot/CMakeFiles/mcuboot_ldpp.dir/depend

