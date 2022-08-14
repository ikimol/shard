# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# include CMake helper macros

include(${PROJECT_SOURCE_DIR}/cmake/add_target.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/export.cmake)

add_subdirectory(algorithm)
add_subdirectory(bit)
add_subdirectory(concurrency)
add_subdirectory(containers)
add_subdirectory(enums)
add_subdirectory(math)
add_subdirectory(memory)
add_subdirectory(meta)
add_subdirectory(property)
add_subdirectory(random)
add_subdirectory(signal)
add_subdirectory(string)
add_subdirectory(system)
add_subdirectory(utility)

# optional modules

if (SHARD_BUILD_NET_MODULE)
    add_subdirectory(net)
endif ()

if (SHARD_BUILD_SQLITE_MODULE)
    add_subdirectory(sqlite)
endif ()

# export all the targets

shard_export_targets()
