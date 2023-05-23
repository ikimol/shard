# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: cmake :: shard_config.cmake
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if (NOT shard_FIND_COMPONENTS)
    # request all components if none were provided
    set(shard_FIND_COMPONENTS
        algorithm any bit concurrency containers core enums math memory meta
        optional random signal string system
        )
endif ()

set(FIND_SHARD_ALGORITHM_DEPENDENCIES utility)
set(FIND_SHARD_BIT_DEPENDENCIES "")
set(FIND_SHARD_CONCURRENCY_DEPENDENCIES meta utility system)
set(FIND_SHARD_CONTAINERS_DEPENDENCIES memory)
set(FIND_SHARD_ENUMS_DEPENDENCIES meta)
set(FIND_SHARD_MATH_DEPENDENCIES "")
set(FIND_SHARD_MEMORY_DEPENDENCIES utility)
set(FIND_SHARD_META_DEPENDENCIES "")
set(FIND_SHARD_NET_DEPENDENCIES concurrency enums memory string)
set(FIND_SHARD_PROPERTY_DEPENDENCIES "")
set(FIND_SHARD_RANDOM_DEPENDENCIES meta)
set(FIND_SHARD_SIGNAL_DEPENDENCIES meta)
set(FIND_SHARD_SQLITE_DEPENDENCIES enums meta memory utility)
set(FIND_SHARD_STRING_DEPENDENCIES meta)
set(FIND_SHARD_SYSTEM_DEPENDENCIES "")
set(FIND_SHARD_UTILITY_DEPENDENCIES "")
set(FIND_SHARD_UUID_DEPENDENCIES algorithm utility)

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

    # create an umbrella target that links all the components that were found
    add_library(shard::shard INTERFACE IMPORTED)

    foreach (COMPONENT ${shard_FIND_COMPONENTS})
        string(TOUPPER "${COMPONENT}" UPPER_COMPONENT)
        if (TARGET shard::${COMPONENT})
            set(SHARD_${UPPER_COMPONENT}_FOUND TRUE)
            # link the component to the umbrella target
            target_link_libraries(shard::shard INTERFACE shard::${COMPONENT})
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
