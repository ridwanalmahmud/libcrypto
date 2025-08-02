cmake_minimum_required(VERSION 3.15)
project(crypto
    VERSION 1.0.0
    DESCRIPTION "NCrypto - A high performance crypto library"
    LANGUAGES C
)

enable_testing()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(build_crypto)
include(build_tests)
include(install_crypto)

option(BUILD_TESTS "Build test executables" ON)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_custom_target(uninstall 
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_MODULE_PATH}/uninstall_crypto.cmake
)
