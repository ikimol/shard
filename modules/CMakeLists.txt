# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# include CMake helper macros

include(${PROJECT_SOURCE_DIR}/cmake/add_target.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/export.cmake)

shard_list_subdirs(MODULE_DIRS ${CMAKE_CURRENT_LIST_DIR})

foreach (MODULE_DIR ${MODULE_DIRS})
    string(TOUPPER "${MODULE_DIR}" MODULE_DIR_UPPER)
    if (NOT ${SHARD_BUILD_${MODULE_DIR_UPPER}_MODULE})
        message(STATUS "Skipping '${MODULE_DIR}' directory")
        continue()
    endif ()
    add_subdirectory(${MODULE_DIR})
endforeach ()

# export all the targets

shard_export_targets()
