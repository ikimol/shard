# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: string :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(string.fmt
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/fmt.cpp
                  MODULES shard::string
                  )

shard_add_example(string.join
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/join.cpp
                  MODULES shard::string
                  )

shard_add_example(string.replace
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/replace.cpp
                  MODULES shard::string
                  )

shard_add_example(string.split
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/split.cpp
                  MODULES shard::string
                  )

shard_add_example(string.to-number
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/to_number.cpp
                  MODULES shard::string
                  )

shard_add_example(string.to-string
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/to_string.cpp
                  MODULES shard::string
                  )
