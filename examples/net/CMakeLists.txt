# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: net :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(net.curl
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/curl.cpp
                  MODULES shard::net
                  )

shard_add_example(net.http
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/http.cpp
                  MODULES shard::net
                  )
