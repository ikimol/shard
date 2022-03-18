# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: property :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME property)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/property/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR})
