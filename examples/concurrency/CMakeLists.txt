# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: concurrency :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

shard_add_example(concurrency.channel
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/channel.cpp
                  MODULES shard::concurrency
                  )

shard_add_example(concurrency.mpsc-queue
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/mpsc_queue.cpp
                  MODULES shard::concurrency
                  )

shard_add_example(concurrency.semaphore
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/semaphore.cpp
                  MODULES shard::concurrency shard::random
                  )

shard_add_example(concurrency.tbuf-value
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tbuf_value.cpp
                  MODULES shard::concurrency
                  )

shard_add_example(concurrency.this-thread
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/this_thread.cpp
                  MODULES shard::concurrency
                  )

shard_add_example(concurrency.thread-pool
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool.cpp
                  MODULES shard::concurrency
                  )

shard_add_example(concurrency.thread-safe
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/thread_safe.cpp
                  MODULES shard::concurrency
                  )
