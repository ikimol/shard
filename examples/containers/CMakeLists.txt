# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: containers :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(containers.sparse_set
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sparse_set.cpp
                  MODULES shard::containers shard::meta
                  )
