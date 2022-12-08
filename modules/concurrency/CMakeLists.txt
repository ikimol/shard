# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: concurrency :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME concurrency)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/concurrency/include)
set(MODULE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/modules/concurrency/src)

set(PLATFORM_SPECIFIC_SOURCES "")

include(${PROJECT_SOURCE_DIR}/cmake/os.cmake)

if (SHARD_OS_LINUX)       # Linux
    list(APPEND PLATFORM_SPECIFIC_SOURCES ${MODULE_SOURCE_DIR}/linux/semaphore.cpp)
elseif (SHARD_OS_DARWIN)  # Apple
    list(APPEND PLATFORM_SPECIFIC_SOURCES ${MODULE_SOURCE_DIR}/darwin/semaphore.cpp)
elseif (SHARD_OS_WINDOWS) # Windows
    list(APPEND PLATFORM_SPECIFIC_SOURCES ${MODULE_SOURCE_DIR}/windows/semaphore.cpp)
endif ()

shard_add_static_library(${MODULE_NAME}
                         SOURCES ${PLATFORM_SPECIFIC_SOURCES}
                         INCLUDE_DIR ${MODULE_INCLUDE_DIR})

target_link_libraries(${MODULE_NAME} PUBLIC
                      shard::meta
                      shard::utility
                      shard::system
                      )
