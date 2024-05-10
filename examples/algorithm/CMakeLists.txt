# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: algorithm :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(algorithm.enumerate
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/enumerate.cpp
                  MODULES shard::algorithm
                  )

shard_add_example(algorithm.functional
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/functional.cpp
                  MODULES shard::algorithm
                  )

shard_add_example(algorithm.variant-switch
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/variant_switch.cpp
                  MODULES shard::algorithm
                  )
