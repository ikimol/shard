# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: expected :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME expected)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/expected/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR})

target_link_libraries(${MODULE_NAME} INTERFACE
                      shard::memory
                      shard::meta
                      )
