# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: alloc :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(alloc.allocators
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/allocators.cpp
                  MODULES shard::alloc
                  )

shard_add_example(alloc.containers.array
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/array.cpp
                  MODULES shard::alloc
                  )

shard_add_example(alloc.containers.stack
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/stack.cpp
                  MODULES shard::alloc
                  )
