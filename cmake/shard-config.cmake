# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: cmake :: shard_config.cmake
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if (NOT shard_FIND_COMPONENTS)
    message(FATAL_ERROR "find_package(shard) called with no components")
endif ()

set(FIND_SHARD_MATH_DEPENDENCIES "")

set(FIND_SHARD_ADDITIONAL_COMPONENTS "")
foreach (COMPONENT ${shard_FIND_COMPONENTS})
    string(TOUPPER "${COMPONENT}" UPPER_COMPONENT)
    list(APPEND FIND_SHARD_ADDITIONAL_COMPONENTS ${FIND_SHARD_${UPPER_COMPONENT}_DEPENDENCIES})
endforeach ()

list(APPEND shard_FIND_COMPONENTS ${FIND_SHARD_ADDITIONAL_COMPONENTS})
list(REMOVE_DUPLICATES shard_FIND_COMPONENTS)

set(TARGETS_CONFIG_FILENAME "${CMAKE_CURRENT_LIST_DIR}/shard-targets.cmake")

# generate imported targets for shard and its dependencies
if (EXISTS "${TARGETS_CONFIG_FILENAME}")
    include("${TARGETS_CONFIG_FILENAME}")

    set(SHARD_FOUND TRUE)

    foreach (COMPONENT ${shard_FIND_COMPONENTS})
        string(TOUPPER "${COMPONENT}" UPPER_COMPONENT)
        if (TARGET shard::${COMPONENT})
            set(SHARD_${UPPER_COMPONENT}_FOUND TRUE)
        else ()
            set(SHARD_FIND_ERROR "Requested shard component '${COMPONENT}' is missing")
            set(SHARD_${UPPER_COMPONENT}_FOUND FALSE)
            set(SHARD_FOUND FALSE)
        endif ()
    endforeach ()
else ()
    set(SHARD_FIND_ERROR "Requested shard configuration was not found.")
    set(SHARD_FOUND FALSE)
endif ()

if (NOT SHARD_FOUND)
    if (shard_FIND_REQUIRED)
        # print error and stop
        message(FATAL_ERROR "${SHARD_FIND_ERROR}")
    elseif (NOT shard_FIND_QUIETLY)
        # print error and continue
        message(STATUS "${SHARD_FIND_ERROR}")
    endif ()
endif ()

if (SHARD_FOUND AND NOT shard_FIND_QUIETLY)
    message(STATUS "Found shard @SHARD_VERSION@ in ${CMAKE_CURRENT_LIST_DIR}")
endif ()
