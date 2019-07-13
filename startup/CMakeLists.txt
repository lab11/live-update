
cmake_minimum_required(VERSION 3.1...3.14)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(STARTUP_COMPILE_OPTIONS ${ARCH_C_FLAGS})
set(STARTUP_COMPILE_DEFS __STARTUP_CLEAR_BSS_MULTIPLE __STARTUP_COPY_MULTIPLE)

# Startup scripts 
add_library(startup_bootloader OBJECT
    "startup_cmsdk_musca_bl2.S"
)
target_compile_options(startup_bootloader PUBLIC ${STARTUP_COMPILE_OPTIONS})
target_compile_definitions(startup_bootloader PRIVATE ${STARTUP_COMPILE_DEFS})

add_library(startup_s OBJECT
    "startup_cmsdk_musca_s.S"
)
target_compile_options(startup_s PUBLIC ${STARTUP_COMPILE_OPTIONS})
target_compile_definitions(startup_s PRIVATE ${STARTUP_COMPILE_DEFS})

add_library(startup_ns OBJECT
    "startup_cmsdk_musca_ns.S"
)
target_compile_options(startup_ns PUBLIC ${STARTUP_COMPILE_OPTIONS})
target_compile_definitions(startup_ns PRIVATE ${STARTUP_COMPILE_DEFS})

