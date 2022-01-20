# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# include CMake helper macros

include(${PROJECT_SOURCE_DIR}/cmake/add_target.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/export.cmake)

add_subdirectory(algorithm)
add_subdirectory(any)
add_subdirectory(bit)
add_subdirectory(concurrency)
add_subdirectory(containers)
add_subdirectory(core)
add_subdirectory(enums)
add_subdirectory(math)
add_subdirectory(memory)
add_subdirectory(meta)
add_subdirectory(optional)
add_subdirectory(random)
add_subdirectory(signal)
add_subdirectory(string)
add_subdirectory(system)
add_subdirectory(utility)

# export all the targets

shard_export_targets()
