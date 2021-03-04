# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: core :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME core)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/core/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR})
