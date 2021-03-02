# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: any :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME any)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/any/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR})

target_link_libraries(${MODULE_NAME} INTERFACE shard::meta)
