# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: io :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME io)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/io/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR}
                              LIBRARIES shard::enums shard::memory
                              )