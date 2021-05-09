# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: bit :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME bit)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/bit/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR})
