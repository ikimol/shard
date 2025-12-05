# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: net :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

find_package(CURL REQUIRED)

set(MODULE_NAME net)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/net/include)

shard_add_header_only_library(${MODULE_NAME} ${MODULE_INCLUDE_DIR}
                              LIBRARIES
                              shard::concurrency
                              shard::enums
                              shard::expected
                              shard::memory
                              shard::string
                              # third party
                              CURL::libcurl
                              )
