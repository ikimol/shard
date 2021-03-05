# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: algorithm :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME algorithm)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/algorithm/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR})
