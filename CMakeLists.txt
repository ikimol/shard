# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: root :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# ------------------------------------------------------------------------------
# project options
# ------------------------------------------------------------------------------

set(SHARD_VERSION 1.13.0)

project(shard VERSION ${SHARD_VERSION})

message(STATUS "${PROJECT_NAME} version ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 17)

# ------------------------------------------------------------------------------
# system information
# ------------------------------------------------------------------------------

math(EXPR SHARD_SYSTEM_ARCH "${CMAKE_SIZEOF_VOID_P} * 8")
message(STATUS "Building on ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} (${SHARD_SYSTEM_ARCH}-bit)")

# ------------------------------------------------------------------------------
# build options
# ------------------------------------------------------------------------------

option(SHARD_BUILD_EXAMPLES "Enable to build the example targets" OFF)
option(SHARD_BUILD_TESTS "Enable to build the tests" OFF)
option(SHARD_BUILD_BENCHMARKS "Enable to build the benchmarks" OFF)
option(SHARD_BUILD_DOCS "Enable to build the documentation" OFF)

# ------------------------------------------------------------------------------
# optional module building
# ------------------------------------------------------------------------------

option(SHARD_BUILD_NET_MODULE "Enable to build the 'net' module" ON)
option(SHARD_BUILD_SQLITE_MODULE "Enable to build the 'sqlite' module" ON)

# ------------------------------------------------------------------------------
# create targets
# ------------------------------------------------------------------------------

# create modules

add_subdirectory(modules)

# add examples

if (SHARD_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

# add tests

if (SHARD_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()

# add benchmarks

if (SHARD_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif ()

# add documentation

if (SHARD_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
endif ()
