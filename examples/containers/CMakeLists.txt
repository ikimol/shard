# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: containers :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(containers.dynamic_bitset
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_bitset.cpp
                  MODULES shard::containers
                  )

shard_add_example(containers.sparse_set
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sparse_set.cpp
                  MODULES shard::containers shard::meta
                  )
