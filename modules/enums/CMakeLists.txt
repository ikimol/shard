# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: enums :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME enums)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/enums/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR})
