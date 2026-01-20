# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: crypto :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(crypto.base64
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/base64.cpp
                  MODULES shard::crypto
                  )

shard_add_example(crypto.md5
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/md5.cpp
                  MODULES shard::crypto
                  )

shard_add_example(crypto.sha1
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sha1.cpp
                  MODULES shard::crypto
                  )

shard_add_example(crypto.sha256
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sha256.cpp
                  MODULES shard::crypto
                  )
