# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# include CMake helper macros

include(${PROJECT_SOURCE_DIR}/cmake/macros.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/os.cmake)

add_subdirectory(any)
add_subdirectory(math)
add_subdirectory(meta)
add_subdirectory(optional)
add_subdirectory(random)
add_subdirectory(string)
