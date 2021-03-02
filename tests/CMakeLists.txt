# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: tests :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(DOCTEST_ROOT_DIR ${PROJECT_SOURCE_DIR}/third_party/doctest)
set(DOCTEST_INCLUDE_DIR ${DOCTEST_ROOT_DIR}/include)
set(DOCTEST_CMAKE_DIR ${DOCTEST_ROOT_DIR}/cmake)

set(SHARD_TEST_TARGET "tests")

add_executable(${SHARD_TEST_TARGET})

target_sources(${SHARD_TEST_TARGET} PRIVATE
               # main test runner
               ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
               )

# TODO: Link required project libraries

target_include_directories(${SHARD_TEST_TARGET} PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${DOCTEST_INCLUDE_DIR}
                           )

# add CTest compatibility
include(${DOCTEST_CMAKE_DIR}/doctest.cmake)
doctest_discover_tests(${SHARD_TEST_TARGET})
