# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: concurrency :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(concurrency.channel
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/channel.cpp
                  MODULES shard::concurrency
                  )

shard_add_example(concurrency.thread-safe
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/thread_safe.cpp
                  MODULES shard::concurrency
                  )
