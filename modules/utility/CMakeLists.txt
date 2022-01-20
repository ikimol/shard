# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: utility :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME utility)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/utility/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR})
