# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: sqlite :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

find_package(SQLite3 REQUIRED)

set(MODULE_NAME sqlite)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/sqlite/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR}
                              LIBRARIES
                              shard::common
                              shard::enums
                              shard::meta
                              shard::memory
                              # third party
                              SQLite::SQLite3
                              )
