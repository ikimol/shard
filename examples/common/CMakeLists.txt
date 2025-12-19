# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: common :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(hash-value
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/hash_value.cpp
                  MODULES shard::common
                  )

shard_add_example(error
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/error.cpp
                  MODULES shard::common
                  )

shard_add_example(runtime-id
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/runtime_id.cpp
                  MODULES shard::common
                  )
