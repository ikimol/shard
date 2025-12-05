# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: stlxt :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME stlxt)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/stlxt/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR})
