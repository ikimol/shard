# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: crypto :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(crypto.base64
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/base64.cpp
                  MODULES shard::crypto
                  )

shard_add_example(crypto.sha
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sha.cpp
                  MODULES shard::crypto
                  )
