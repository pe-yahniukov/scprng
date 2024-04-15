#
# MIT License
#
# Copyright 2024 PE Stanislav Yahniukov <pe@yahniukov.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this
# software and associated documentation files (the “Software”), to deal in the Software
# without restriction, including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
# to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

cmake_minimum_required(VERSION 3.10)

set(scprng_version 1.2.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(scprng
    VERSION ${scprng_version}
    LANGUAGES C)

if(NOT DEFINED SCPRNG_AS_SUBPROJECT)
    set(SCPRNG_AS_SUBPROJECT ON)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(SCPRNG_AS_SUBPROJECT OFF)
    endif()
endif()

include(CheckCCompilerFlag)

set(default_compiler_flags
    -Wall
    -Werror
    -Wpedantic
    -Wno-unused-variable
    -fPIC)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND default_compiler_flags -g)
endif()

foreach(compiler_flag ${default_compiler_flags})
    string(REGEX REPLACE "[^a-zA-Z0-9]" "" current_variable ${compiler_flag})
    CHECK_C_COMPILER_FLAG(${compiler_flag} "FLAG_SUPPORTED_${current_variable}")
    if (FLAG_SUPPORTED_${current_variable})
        list(APPEND supported_compiler_flags)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${compiler_flag}")
    endif()
endforeach()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${supported_compiler_flags}")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    link_libraries("-s -O3")
endif()

find_package(MbedCrypto REQUIRED)

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/scprng.h)
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/scprng.c)

set(SCPRNG_LIB scprng)
set(SCPRNG_GEN scprng_gen)

add_library("${SCPRNG_LIB}" SHARED "${HEADERS}" "${SOURCES}")
add_library("${SCPRNG_LIB}-static" STATIC "${HEADERS}" "${SOURCES}")

target_include_directories("${SCPRNG_LIB}" PRIVATE ${MBEDCRYPTO_INCLUDE_DIR})
target_include_directories("${SCPRNG_LIB}-static" PRIVATE ${MBEDCRYPTO_INCLUDE_DIR})

target_link_libraries("${SCPRNG_LIB}" "${MBEDCRYPTO_LIBRARY}")
target_link_libraries("${SCPRNG_LIB}-static" "${MBEDCRYPTO_LIBRARY}")

set_target_properties("${SCPRNG_LIB}" PROPERTIES OUTPUT_NAME "${SCPRNG_LIB}-${scprng_version}")
set_target_properties("${SCPRNG_LIB}" PROPERTIES PREFIX "lib")
set_target_properties("${SCPRNG_LIB}-static" PROPERTIES OUTPUT_NAME "${SCPRNG_LIB}-${scprng_version}")
set_target_properties("${SCPRNG_LIB}-static" PROPERTIES PREFIX "lib")

add_custom_command(TARGET "${SCPRNG_LIB}" POST_BUILD
    COMMAND ln -s $<TARGET_FILE_NAME:${SCPRNG_LIB}> libscprng${CMAKE_SHARED_LIBRARY_SUFFIX}
)
add_custom_command(TARGET "${SCPRNG_LIB}-static" POST_BUILD
    COMMAND ln -s $<TARGET_FILE_NAME:${SCPRNG_LIB}-static> libscprng${CMAKE_STATIC_LIBRARY_SUFFIX}
)

if (NOT ${SCPRNG_AS_SUBPROJECT})
    add_executable("${SCPRNG_GEN}" ${CMAKE_CURRENT_SOURCE_DIR}/scprng_gen.c)
    target_include_directories("${SCPRNG_GEN}" PRIVATE ${MBEDCRYPTO_INCLUDE_DIR})
    target_link_libraries("${SCPRNG_GEN}" "${SCPRNG_LIB}-static" "${MBEDCRYPTO_LIBRARY}")
endif()
