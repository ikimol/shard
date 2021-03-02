# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: math :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME math)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/math/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR})
