# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: enums :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(enums.enum-set
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/enum_set.cpp
                  MODULES shard::enums
                  )

shard_add_example(enums.flags
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/flags.cpp
                  MODULES shard::enums
                  )
