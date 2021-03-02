# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: meta :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME meta)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/meta/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR})
