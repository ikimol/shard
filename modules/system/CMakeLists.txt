# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: system :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME system)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/system/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR})
