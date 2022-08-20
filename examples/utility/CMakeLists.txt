# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: utility :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(utility.hash-value
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/hash_value.cpp
                  MODULES shard::utility
                  )

shard_add_example(utility.error
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/error.cpp
                  MODULES shard::utility
                  )

shard_add_example(utility.timer
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp
                  MODULES shard::utility
                  )
