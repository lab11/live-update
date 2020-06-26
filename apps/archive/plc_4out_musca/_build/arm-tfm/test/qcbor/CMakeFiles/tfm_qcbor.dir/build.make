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
include //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/depend.make

# Include the progress variables for this target.
include //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/progress.make

# Include the compile flags for this target's objects.
include //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/flags.make

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/ieee754.o: //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/flags.make
//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/ieee754.o: //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/ieee754.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_4out/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/ieee754.o"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tfm_qcbor.dir/src/ieee754.o   -c //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/ieee754.c

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/ieee754.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tfm_qcbor.dir/src/ieee754.i"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/ieee754.c > CMakeFiles/tfm_qcbor.dir/src/ieee754.i

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/ieee754.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tfm_qcbor.dir/src/ieee754.s"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/ieee754.c -o CMakeFiles/tfm_qcbor.dir/src/ieee754.s

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/qcbor_decode.o: //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/flags.make
//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/qcbor_decode.o: //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/qcbor_decode.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_4out/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/qcbor_decode.o"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tfm_qcbor.dir/src/qcbor_decode.o   -c //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/qcbor_decode.c

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/qcbor_decode.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tfm_qcbor.dir/src/qcbor_decode.i"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/qcbor_decode.c > CMakeFiles/tfm_qcbor.dir/src/qcbor_decode.i

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/qcbor_decode.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tfm_qcbor.dir/src/qcbor_decode.s"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/qcbor_decode.c -o CMakeFiles/tfm_qcbor.dir/src/qcbor_decode.s

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/qcbor_encode.o: //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/flags.make
//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/qcbor_encode.o: //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/qcbor_encode.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_4out/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/qcbor_encode.o"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tfm_qcbor.dir/src/qcbor_encode.o   -c //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/qcbor_encode.c

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/qcbor_encode.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tfm_qcbor.dir/src/qcbor_encode.i"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/qcbor_encode.c > CMakeFiles/tfm_qcbor.dir/src/qcbor_encode.i

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/qcbor_encode.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tfm_qcbor.dir/src/qcbor_encode.s"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/qcbor_encode.c -o CMakeFiles/tfm_qcbor.dir/src/qcbor_encode.s

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/UsefulBuf.o: //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/flags.make
//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/UsefulBuf.o: //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/UsefulBuf.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_4out/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/UsefulBuf.o"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tfm_qcbor.dir/src/UsefulBuf.o   -c //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/UsefulBuf.c

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/UsefulBuf.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tfm_qcbor.dir/src/UsefulBuf.i"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/UsefulBuf.c > CMakeFiles/tfm_qcbor.dir/src/UsefulBuf.i

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/UsefulBuf.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tfm_qcbor.dir/src/UsefulBuf.s"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/trusted-firmware-m/lib/ext/qcbor/src/UsefulBuf.c -o CMakeFiles/tfm_qcbor.dir/src/UsefulBuf.s

tfm_qcbor: //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/ieee754.o
tfm_qcbor: //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/qcbor_decode.o
tfm_qcbor: //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/qcbor_encode.o
tfm_qcbor: //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/src/UsefulBuf.o
tfm_qcbor: //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/build.make

.PHONY : tfm_qcbor

# Rule to build all files generated by this target.
//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/build: tfm_qcbor

.PHONY : //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/build

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/clean:
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor && $(CMAKE_COMMAND) -P //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/cmake_clean.cmake
.PHONY : //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/clean

//vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/depend:
	cd //vagrant/apps/plc_4out/_build/arm-tfm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" //vagrant/ext/trusted-firmware-m //vagrant/ext/trusted-firmware-m/lib/ext/qcbor //vagrant/apps/plc_4out/_build/arm-tfm //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : //vagrant/apps/plc_4out/_build/arm-tfm/test/qcbor/CMakeFiles/tfm_qcbor.dir/depend

