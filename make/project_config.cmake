#
# Sets shared CMake-specific configuration for project targets
#

# Set CMake system specifiers
set(CMAKE_SYSTEM_NAME "Generic" CACHE INTERNAL "" FORCE)
set(CMAKE_SYSTEM_PROCESSOR "arm" CACHE INTERNAL "" FORCE)

# Configure arm-none-eabi toolchain
set(TOOLCHAIN_PATH /usr/local/gcc-arm-none-eabi-8-2018-q4-major/bin/)
set(TOOLCHAIN_PREFIX ${TOOLCHAIN_PATH}arm-none-eabi-)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}objdump)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}ar)
set(CMAKE_RANLIB ${TOOLCHAIN_PREFIX}ranlib)

set(CMAKE_C_COMPILER_ID_RUN 1)
set(CMAKE_C_COMPILER_FORCED true)
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(CMAKE_C_FLAGS_DEBUG "-g -O0")

set(GDBCLIENT ${TOOLCHAIN_PREFIX}gdb)
set(GDBSERVER "pyocd")
set(GDBSERVER_ARGS
    "gdbserver"
)

set(PYTHONCLIENT python3)

# Apply CPU-specific flags
if (NOT CMAKE_C_FLAGS MATCHES ".*{CMAKE_C_FLAGS_CPU}.*")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_CPU}")
endif()

if (NOT CMAKE_ASM_FLAGS MATCHES ".*{CMAKE_ASM_FLAGS_CPU}.*")
    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${CMAKE_ASM_FLAGS_CPU}")
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_LINK_FLAGS_CPU}")

set(COMMON_C_FLAGS
    -fshort-enums
    -fshort-wchar
    -funsigned-char
    -msoft-float
    -mcmse
    --specs=nano.specs
)

set(ARCH_C_FLAGS
    #-mcpu=cortex-m33
    -march=armv8-m.main
    -mthumb
)
    
