# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: utility :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(function_ref
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/function_ref.cpp
                  MODULES shard::utility
                  )

shard_add_example(timer
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp
                  MODULES shard::utility
                  )
