# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: uuid :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME uuid)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/uuid/include)
set(MODULE_SRC_DIR ${PROJECT_SOURCE_DIR}/modules/uuid/src)

set(PLATFORM_SPECIFIC_SOURCES "")
set(PLATFORM_SPECIFIC_LIBS "")

include(${PROJECT_SOURCE_DIR}/cmake/os.cmake)

if (SHARD_OS_LINUX)       # Linux
    list(APPEND PLATFORM_SPECIFIC_SOURCES ${MODULE_SRC_DIR}/linux/generator.cpp)
    list(APPEND PLATFORM_SPECIFIC_LIBS -luuid)
elseif (SHARD_OS_DARWIN)  # Apple
    list(APPEND PLATFORM_SPECIFIC_SOURCES ${MODULE_SRC_DIR}/darwin/generator.cpp)
    include(${PROJECT_SOURCE_DIR}/cmake/macos.cmake)
    shard_find_frameworks(PLATFORM_SPECIFIC_LIBS CoreFoundation)
elseif (SHARD_OS_WINDOWS) # Windows
    list(APPEND PLATFORM_SPECIFIC_SOURCES ${MODULE_SRC_DIR}/win/generator.cpp)
endif ()

set(MODULE_SOURCES
    ${MODULE_SRC_DIR}/uuid.cpp
    )

shard_add_static_library(${MODULE_NAME}
                         SOURCES ${MODULE_SOURCES} ${PLATFORM_SPECIFIC_SOURCES}
                         INCLUDE_DIR ${MODULE_INCLUDE_DIR}
                         LIBRARIES shard::algorithm shard::utility ${PLATFORM_SPECIFIC_LIBS}
                         )
