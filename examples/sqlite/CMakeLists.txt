# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: sqlite :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(sqlite
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sqlite.cpp
                  MODULES shard::sqlite
                  RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/databases
                  )
