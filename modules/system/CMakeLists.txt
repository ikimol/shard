# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: system :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME system)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/system/include)

shard_add_static_library(${MODULE_NAME}
                         SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/env.cpp
                         INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include
                         )
