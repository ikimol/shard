# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: common :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME common)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/common/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR}
                              LIBRARIES
                              shard::string
                              )
