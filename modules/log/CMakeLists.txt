# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: log :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME log)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/log/include)
set(MODULE_SRC_DIR ${PROJECT_SOURCE_DIR}/modules/log/src)

set(MODULE_SOURCES
    ${MODULE_SRC_DIR}/logger.cpp
    ${MODULE_SRC_DIR}/message.cpp
    ${MODULE_SRC_DIR}/sink.cpp
    )

shard_add_static_library(${MODULE_NAME}
                         SOURCES ${MODULE_SOURCES}
                         INCLUDE_DIR ${MODULE_INCLUDE_DIR}
                         LIBRARIES shard::common shard::enums
                         )
