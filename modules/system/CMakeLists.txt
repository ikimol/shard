# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: system :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME system)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/system/include)
set(MODULE_SRC_DIR ${PROJECT_SOURCE_DIR}/modules/system/src)

set(MODULE_SOURCES
    ${MODULE_SRC_DIR}/env.cpp
    ${MODULE_SRC_DIR}/platform.cpp
    )

shard_add_static_library(${MODULE_NAME}
                         SOURCES ${MODULE_SOURCES}
                         INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include
                         )
