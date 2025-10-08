# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: examples :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# include CMake helper macros

include(${PROJECT_SOURCE_DIR}/cmake/add_target.cmake)

shard_list_subdirs(EXAMPLE_DIRS ${CMAKE_CURRENT_LIST_DIR})

foreach (EXAMPLE_DIR ${EXAMPLE_DIRS})
    string(TOUPPER "${EXAMPLE_DIR}" EXAMPLE_DIR_UPPER)
    if (NOT ${SHARD_BUILD_${EXAMPLE_DIR_UPPER}_MODULE})
        continue()
    endif ()
    add_subdirectory(${EXAMPLE_DIR})
endforeach ()
