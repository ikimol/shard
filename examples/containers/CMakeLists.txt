# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: containers :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(containers.array
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/array.cpp
                  MODULES shard::containers
                  )

shard_add_example(containers.hash-map
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/hash_map.cpp
                  MODULES shard::containers
                  )

shard_add_example(containers.stack
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/stack.cpp
                  MODULES shard::containers
                  )
