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
CMAKE_SOURCE_DIR = //vagrant/ext/mbedtls

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls

# Include any dependencies generated for this target.
include //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/depend.make

# Include the progress variables for this target.
include //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/progress.make

# Include the compile flags for this target's objects.
include //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/flags.make

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/debug.o: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/flags.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/debug.o: //vagrant/ext/mbedtls/library/debug.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/debug.o"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mbedtls.dir/debug.o   -c //vagrant/ext/mbedtls/library/debug.c

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/debug.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mbedtls.dir/debug.i"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/mbedtls/library/debug.c > CMakeFiles/mbedtls.dir/debug.i

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/debug.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mbedtls.dir/debug.s"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/mbedtls/library/debug.c -o CMakeFiles/mbedtls.dir/debug.s

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/net_sockets.o: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/flags.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/net_sockets.o: //vagrant/ext/mbedtls/library/net_sockets.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/net_sockets.o"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mbedtls.dir/net_sockets.o   -c //vagrant/ext/mbedtls/library/net_sockets.c

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/net_sockets.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mbedtls.dir/net_sockets.i"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/mbedtls/library/net_sockets.c > CMakeFiles/mbedtls.dir/net_sockets.i

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/net_sockets.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mbedtls.dir/net_sockets.s"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/mbedtls/library/net_sockets.c -o CMakeFiles/mbedtls.dir/net_sockets.s

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cache.o: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/flags.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cache.o: //vagrant/ext/mbedtls/library/ssl_cache.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cache.o"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mbedtls.dir/ssl_cache.o   -c //vagrant/ext/mbedtls/library/ssl_cache.c

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cache.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mbedtls.dir/ssl_cache.i"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/mbedtls/library/ssl_cache.c > CMakeFiles/mbedtls.dir/ssl_cache.i

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cache.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mbedtls.dir/ssl_cache.s"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/mbedtls/library/ssl_cache.c -o CMakeFiles/mbedtls.dir/ssl_cache.s

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_ciphersuites.o: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/flags.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_ciphersuites.o: //vagrant/ext/mbedtls/library/ssl_ciphersuites.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_ciphersuites.o"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mbedtls.dir/ssl_ciphersuites.o   -c //vagrant/ext/mbedtls/library/ssl_ciphersuites.c

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_ciphersuites.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mbedtls.dir/ssl_ciphersuites.i"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/mbedtls/library/ssl_ciphersuites.c > CMakeFiles/mbedtls.dir/ssl_ciphersuites.i

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_ciphersuites.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mbedtls.dir/ssl_ciphersuites.s"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/mbedtls/library/ssl_ciphersuites.c -o CMakeFiles/mbedtls.dir/ssl_ciphersuites.s

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cli.o: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/flags.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cli.o: //vagrant/ext/mbedtls/library/ssl_cli.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cli.o"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mbedtls.dir/ssl_cli.o   -c //vagrant/ext/mbedtls/library/ssl_cli.c

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cli.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mbedtls.dir/ssl_cli.i"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/mbedtls/library/ssl_cli.c > CMakeFiles/mbedtls.dir/ssl_cli.i

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cli.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mbedtls.dir/ssl_cli.s"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/mbedtls/library/ssl_cli.c -o CMakeFiles/mbedtls.dir/ssl_cli.s

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cookie.o: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/flags.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cookie.o: //vagrant/ext/mbedtls/library/ssl_cookie.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cookie.o"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mbedtls.dir/ssl_cookie.o   -c //vagrant/ext/mbedtls/library/ssl_cookie.c

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cookie.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mbedtls.dir/ssl_cookie.i"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/mbedtls/library/ssl_cookie.c > CMakeFiles/mbedtls.dir/ssl_cookie.i

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cookie.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mbedtls.dir/ssl_cookie.s"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/mbedtls/library/ssl_cookie.c -o CMakeFiles/mbedtls.dir/ssl_cookie.s

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_srv.o: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/flags.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_srv.o: //vagrant/ext/mbedtls/library/ssl_srv.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_srv.o"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mbedtls.dir/ssl_srv.o   -c //vagrant/ext/mbedtls/library/ssl_srv.c

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_srv.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mbedtls.dir/ssl_srv.i"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/mbedtls/library/ssl_srv.c > CMakeFiles/mbedtls.dir/ssl_srv.i

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_srv.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mbedtls.dir/ssl_srv.s"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/mbedtls/library/ssl_srv.c -o CMakeFiles/mbedtls.dir/ssl_srv.s

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_ticket.o: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/flags.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_ticket.o: //vagrant/ext/mbedtls/library/ssl_ticket.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_ticket.o"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mbedtls.dir/ssl_ticket.o   -c //vagrant/ext/mbedtls/library/ssl_ticket.c

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_ticket.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mbedtls.dir/ssl_ticket.i"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/mbedtls/library/ssl_ticket.c > CMakeFiles/mbedtls.dir/ssl_ticket.i

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_ticket.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mbedtls.dir/ssl_ticket.s"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/mbedtls/library/ssl_ticket.c -o CMakeFiles/mbedtls.dir/ssl_ticket.s

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_tls.o: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/flags.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_tls.o: //vagrant/ext/mbedtls/library/ssl_tls.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_tls.o"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mbedtls.dir/ssl_tls.o   -c //vagrant/ext/mbedtls/library/ssl_tls.c

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_tls.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mbedtls.dir/ssl_tls.i"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E //vagrant/ext/mbedtls/library/ssl_tls.c > CMakeFiles/mbedtls.dir/ssl_tls.i

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_tls.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mbedtls.dir/ssl_tls.s"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S //vagrant/ext/mbedtls/library/ssl_tls.c -o CMakeFiles/mbedtls.dir/ssl_tls.s

# Object files for target mbedtls
mbedtls_OBJECTS = \
"CMakeFiles/mbedtls.dir/debug.o" \
"CMakeFiles/mbedtls.dir/net_sockets.o" \
"CMakeFiles/mbedtls.dir/ssl_cache.o" \
"CMakeFiles/mbedtls.dir/ssl_ciphersuites.o" \
"CMakeFiles/mbedtls.dir/ssl_cli.o" \
"CMakeFiles/mbedtls.dir/ssl_cookie.o" \
"CMakeFiles/mbedtls.dir/ssl_srv.o" \
"CMakeFiles/mbedtls.dir/ssl_ticket.o" \
"CMakeFiles/mbedtls.dir/ssl_tls.o"

# External object files for target mbedtls
mbedtls_EXTERNAL_OBJECTS =

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/mbedtls.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/debug.o
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/mbedtls.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/net_sockets.o
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/mbedtls.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cache.o
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/mbedtls.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_ciphersuites.o
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/mbedtls.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cli.o
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/mbedtls.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_cookie.o
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/mbedtls.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_srv.o
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/mbedtls.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_ticket.o
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/mbedtls.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/ssl_tls.o
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/mbedtls.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/build.make
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/mbedtls.a: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking C static library //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/mbedtls.a"
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && $(CMAKE_COMMAND) -P //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/cmake_clean_target.cmake
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && $(CMAKE_COMMAND) -E cmake_link_script //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/build: //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/mbedtls.a

.PHONY : //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/build

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/clean:
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library && $(CMAKE_COMMAND) -P //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/cmake_clean.cmake
.PHONY : //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/clean

//vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/depend:
	cd //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" //vagrant/ext/mbedtls //vagrant/ext/mbedtls/library //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : //vagrant/apps/plc_led_blink/_build/arm-tfm/app/secure_fw/services/crypto/mbedtls/library/CMakeFiles/mbedtls.dir/depend

