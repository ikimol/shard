# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: tests :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(DOCTEST_ROOT_DIR ${PROJECT_SOURCE_DIR}/third_party/doctest)
set(DOCTEST_INCLUDE_DIR ${DOCTEST_ROOT_DIR}/include)
set(DOCTEST_CMAKE_DIR ${DOCTEST_ROOT_DIR}/cmake)

set(SHARD_TEST_TARGET "tests")

if (POLICY CMP0110)
    # https://cmake.org/cmake/help/latest/policy/CMP0110.html
    cmake_policy(SET CMP0110 NEW)
endif ()

add_executable(${SHARD_TEST_TARGET}
               # main test runner
               ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
               # helpers
               ${CMAKE_CURRENT_SOURCE_DIR}/helpers/counter.cpp
               # modules
               ${CMAKE_CURRENT_SOURCE_DIR}/algorithm_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/alloc/containers/array_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/alloc/allocators_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/bit_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/concurrency_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/containers/dynamic_bitset_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/enums_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/expected_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/math_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/memory_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/meta_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/property_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/signal_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/sparse_set_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/string_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/utility_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/uuid_test.cpp
               )

target_link_libraries(${SHARD_TEST_TARGET} PRIVATE
                      shard::algorithm
                      shard::alloc
                      shard::bit
                      shard::concurrency
                      shard::containers
                      shard::enums
                      shard::expected
                      shard::math
                      shard::memory
                      shard::meta
                      shard::property
                      shard::signal
                      shard::string
                      shard::utility
                      shard::uuid
                      )

include(${PROJECT_SOURCE_DIR}/cmake/os.cmake)

# link pthreads if building on Linux
if (SHARD_OS_LINUX)
    find_package(Threads REQUIRED)
    target_link_libraries(${SHARD_TEST_TARGET} PRIVATE Threads::Threads)
endif ()

target_include_directories(${SHARD_TEST_TARGET} PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${DOCTEST_INCLUDE_DIR}
                           )

# add CTest compatibility
include(${DOCTEST_CMAKE_DIR}/doctest.cmake)
doctest_discover_tests(${SHARD_TEST_TARGET})
