# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: containers :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(containers.array
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/array.cpp
                  MODULES shard::containers
                  )

shard_add_example(containers.stack
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/stack.cpp
                  MODULES shard::containers
                  )

shard_add_example(containers.static-vector
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/static_vector.cpp
                  MODULES shard::containers
                  )
