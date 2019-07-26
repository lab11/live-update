# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(hello_world)

target_sources(app PRIVATE src/main.c)

## https://stackoverflow.com/questions/32183975/how-to-print-all-the-properties-of-a-target-in-cmake
#
## Get all properties that cmake supports
#execute_process(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)
## Convert command output into a CMake list
#STRING(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
#STRING(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
#
#list(REMOVE_DUPLICATES CMAKE_PROPERTY_LIST)
#
#function(print_target_properties tgt)
#    foreach (prop ${CMAKE_PROPERTY_LIST})
#        string(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" prop ${prop})
#        if(prop STREQUAL "LOCATION" OR prop MATCHES "^LOCATION_" OR prop MATCHES "_LOCATION$")
#            continue()
#        endif()
#        get_target_property(propval ${tgt} ${prop})
#        if (propval)
#            get_target_property(propval ${tgt} ${prop})
#            message ("${tgt} ${prop} = ${propval}")
#        endif()
#    endforeach(prop)
#endfunction(print_target_properties)
#
#print_target_properties(app)


