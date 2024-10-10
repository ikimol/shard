# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: benchmarks :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# include CMake helper macros

include(${PROJECT_SOURCE_DIR}/cmake/add_target.cmake)

shard_add_benchmark(random
                    SOURCES random.cpp main.cpp
                    INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
                    MODULES shard::random
                    )

shard_add_benchmark(signal
                    SOURCES signal.cpp main.cpp
                    INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
                    MODULES shard::signal
                    )

shard_add_benchmark(string.fmt
                    SOURCES fmt.cpp main.cpp
                    INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
                    MODULES shard::string
                    )
