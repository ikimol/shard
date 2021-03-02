# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: math :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(math.degrees
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/degrees.cpp
                  MODULES shard::math
                  )

shard_add_example(math.percentage
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/percentage.cpp
                  MODULES shard::math
                  )

shard_add_example(math.utils
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
                  MODULES shard::math
                  )
