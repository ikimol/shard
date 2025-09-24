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

shard_add_example(net.spotify
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/spotify.cpp
                  COMPILE_OPTIONS "-I/opt/homebrew/include"
                  MODULES shard::net
                  )

shard_add_example(net.world-time
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/world_time.cpp
                  COMPILE_OPTIONS "-I/opt/homebrew/include"
                  MODULES shard::net
                  )
