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
include //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/depend.make

# Include the progress variables for this target.
include //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/progress.make

# Include the compile flags for this target's objects.
include //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/flags.make

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_sign1_verify.o: //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/flags.make
//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_sign1_verify.o: //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_sign1_verify.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_4out/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_sign1_verify.o"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_sign1_verify.o   -c //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_sign1_verify.c

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_sign1_verify.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_sign1_verify.i"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_sign1_verify.c > CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_sign1_verify.i

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_sign1_verify.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_sign1_verify.s"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_sign1_verify.c -o CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_sign1_verify.s

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_util.o: //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/flags.make
//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_util.o: //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_util.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_4out/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_util.o"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_util.o   -c //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_util.c

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_util.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_util.i"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_util.c > CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_util.i

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_util.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_util.s"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_util.c -o CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_util.s

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_hash.o: //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/flags.make
//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_hash.o: //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_psa_crypto_hash.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_4out/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_hash.o"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_hash.o   -c //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_psa_crypto_hash.c

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_hash.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_hash.i"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_psa_crypto_hash.c > CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_hash.i

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_hash.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_hash.s"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_psa_crypto_hash.c -o CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_hash.s

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_verify.o: //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/flags.make
//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_verify.o: //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_psa_crypto_verify.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_4out/_build/arm-tfm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_verify.o"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_verify.o   -c //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_psa_crypto_verify.c

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_verify.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_verify.i"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_psa_crypto_verify.c > CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_verify.i

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_verify.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_verify.s"
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/trusted-firmware-m/lib/t_cose/src/t_cose_psa_crypto_verify.c -o CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_verify.s

tfm_t_cose_verify: //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_sign1_verify.o
tfm_t_cose_verify: //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_util.o
tfm_t_cose_verify: //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_hash.o
tfm_t_cose_verify: //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/src/t_cose_psa_crypto_verify.o
tfm_t_cose_verify: //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/build.make

.PHONY : tfm_t_cose_verify

# Rule to build all files generated by this target.
//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/build: tfm_t_cose_verify

.PHONY : //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/build

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/clean:
	cd //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose && $(CMAKE_COMMAND) -P //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/cmake_clean.cmake
.PHONY : //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/clean

//vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/depend:
	cd //vagrant/apps/plc_4out/_build/arm-tfm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" //vagrant/ext/trusted-firmware-m //vagrant/ext/trusted-firmware-m/lib/t_cose //vagrant/apps/plc_4out/_build/arm-tfm //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : //vagrant/apps/plc_4out/_build/arm-tfm/test/t_cose/CMakeFiles/tfm_t_cose_verify.dir/depend

