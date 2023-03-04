# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: memory :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(memory.allocators
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/allocators.cpp
                  MODULES shard::memory
                  )

shard_add_example(memory.buffer-ptr
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/buffer_ptr.cpp
                  MODULES shard::memory
                  )

shard_add_example(memory.object
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/object.cpp
                  MODULES shard::memory
                  )