# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: concurrency :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME concurrency)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/concurrency/include)
set(MODULE_SRC_DIR ${PROJECT_SOURCE_DIR}/modules/concurrency/src)

set(PLATFORM_SPECIFIC_SOURCES "")

include(${PROJECT_SOURCE_DIR}/cmake/os.cmake)

if (SHARD_OS_LINUX)       # Linux
    list(APPEND PLATFORM_SPECIFIC_SOURCES
         ${MODULE_SRC_DIR}/linux/semaphore.cpp
         ${MODULE_SRC_DIR}/linux/this_thread.cpp
         )
elseif (SHARD_OS_DARWIN)  # Apple
    list(APPEND PLATFORM_SPECIFIC_SOURCES
         ${MODULE_SRC_DIR}/darwin/semaphore.cpp
         ${MODULE_SRC_DIR}/darwin/this_thread.cpp
         )
elseif (SHARD_OS_WINDOWS) # Windows
    list(APPEND PLATFORM_SPECIFIC_SOURCES
         ${MODULE_SRC_DIR}/win/semaphore.cpp
         ${MODULE_SRC_DIR}/win/this_thread.cpp
         )
endif ()

shard_add_static_library(${MODULE_NAME}
                         SOURCES ${PLATFORM_SPECIFIC_SOURCES}
                         INCLUDE_DIR ${MODULE_INCLUDE_DIR}
                         LIBRARIES shard::meta shard::utility shard::system
                         )
