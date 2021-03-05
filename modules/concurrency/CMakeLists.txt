# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: concurrency :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME concurrency)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/concurrency/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR})

target_link_libraries(${MODULE_NAME} INTERFACE
                      shard::core
                      shard::meta
                      shard::optional
                      )
