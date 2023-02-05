# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: uuid :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME uuid)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/uuid/include)
set(MODULE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/modules/uuid/src)

set(PLATFORM_SPECIFIC_SOURCES "")
set(PLATFORM_SPECIFIC_SOURCES "")

include(${PROJECT_SOURCE_DIR}/cmake/os.cmake)

if (SHARD_OS_LINUX)       # Linux
    list(APPEND PLATFORM_SPECIFIC_SOURCES ${MODULE_SOURCE_DIR}/linux/generator.cpp)
elseif (SHARD_OS_DARWIN)  # Apple
    list(APPEND PLATFORM_SPECIFIC_SOURCES ${MODULE_SOURCE_DIR}/darwin/generator.cpp)
elseif (SHARD_OS_WINDOWS) # Windows
    list(APPEND PLATFORM_SPECIFIC_SOURCES ${MODULE_SOURCE_DIR}/win/generator.cpp)
endif ()

shard_add_static_library(${MODULE_NAME}
                         SOURCES ${PLATFORM_SPECIFIC_SOURCES}
                         INCLUDE_DIR ${MODULE_INCLUDE_DIR}
                         )

target_link_libraries(${MODULE_NAME} PUBLIC
                      shard::algorithm
                      shard::utility
                      )

if (SHARD_OS_DARWIN)
    include(${PROJECT_SOURCE_DIR}/cmake/macos.cmake)
    shard_link_mac_frameworks(${MODULE_NAME} CoreFoundation)
endif ()
