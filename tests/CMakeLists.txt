# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: tests :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(DOCTEST_ROOT_DIR ${PROJECT_SOURCE_DIR}/third_party/doctest)
set(DOCTEST_INCLUDE_DIR ${DOCTEST_ROOT_DIR}/include)
set(DOCTEST_CMAKE_DIR ${DOCTEST_ROOT_DIR}/cmake)

set(SHARD_TEST_TARGET "tests")

add_executable(${SHARD_TEST_TARGET}
               # main test runner
               ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
               # modules
               ${CMAKE_CURRENT_SOURCE_DIR}/any_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/core_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/enums_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/math_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/memory_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/meta_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/signal_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/string_test.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/optional_test.cpp
               )

target_link_libraries(${SHARD_TEST_TARGET} PRIVATE
                      shard::any
                      shard::core
                      shard::enums
                      shard::math
                      shard::memory
                      shard::meta
                      shard::optional
                      shard::signal
                      shard::string
                      )

target_include_directories(${SHARD_TEST_TARGET} PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${DOCTEST_INCLUDE_DIR}
                           )

# add CTest compatibility
include(${DOCTEST_CMAKE_DIR}/doctest.cmake)
doctest_discover_tests(${SHARD_TEST_TARGET})
