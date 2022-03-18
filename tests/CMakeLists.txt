# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: tests :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(CATCH_ROOT_DIR ${PROJECT_SOURCE_DIR}/third_party/catch)
set(CATCH_INCLUDE_DIR ${CATCH_ROOT_DIR}/include)
set(CATCH_CMAKE_DIR ${CATCH_ROOT_DIR}/cmake)

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
               ${CMAKE_CURRENT_SOURCE_DIR}/any_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/array_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/bit_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/concurrency_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/enums_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/math_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/memory_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/meta_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/optional_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/property_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/signal_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/string_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/utility_test.cpp
               )

target_link_libraries(${SHARD_TEST_TARGET} PRIVATE
                      shard::algorithm
                      shard::any
                      shard::bit
                      shard::concurrency
                      shard::containers
                      shard::enums
                      shard::math
                      shard::memory
                      shard::meta
                      shard::optional
                      shard::property
                      shard::signal
                      shard::string
                      shard::utility
                      )

include(${PROJECT_SOURCE_DIR}/cmake/os.cmake)

# link pthreads if building on Linux
if (SHARD_OS_LINUX)
    find_package(Threads REQUIRED)
    target_link_libraries(${SHARD_TEST_TARGET} PRIVATE Threads::Threads)
endif ()

target_include_directories(${SHARD_TEST_TARGET} PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${CATCH_INCLUDE_DIR}
                           )

# add CTest compatibility
include(${CATCH_CMAKE_DIR}/ParseAndAddCatchTests.cmake)
parseandaddcatchtests(${SHARD_TEST_TARGET})
