# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# include CMake helper macros

include(${PROJECT_SOURCE_DIR}/cmake/add_target.cmake)

add_subdirectory(algorithm)
add_subdirectory(concurrency)
add_subdirectory(containers)
add_subdirectory(enums)
add_subdirectory(math)
add_subdirectory(memory)
add_subdirectory(property)
add_subdirectory(net)
add_subdirectory(random)
add_subdirectory(signal)
add_subdirectory(string)
add_subdirectory(system)
add_subdirectory(utility)
