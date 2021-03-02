# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: cmake :: macros.cmake
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(SHARD_CONFIG_EXPORT_NAME "shard_config_export")

# use the paths from the GNU cmake module as defaults
include(GNUInstallDirs)

# include cmake utilities
include(${CMAKE_SOURCE_DIR}/cmake/utility.cmake)

# add a new static library target
#
# usage: shard_add_static_library(<name>
#                                 SOURCES <src>...
#                                 INCLUDE_DIR <dir>
#                                 )
macro (shard_add_static_library MODULE_NAME)
    cmake_parse_arguments(LOCAL "" "INCLUDE_DIR" "SOURCES" ${ARGN})

    add_library(${MODULE_NAME} STATIC)
    add_library(shard::${MODULE_NAME} ALIAS ${MODULE_NAME})

    # enable warnings
    shard_target_enable_warnings(${MODULE_NAME})

    target_sources(${MODULE_NAME} PRIVATE ${LOCAL_SOURCES})

    # define the export symbol of the module
    string(REPLACE "-" "_" NAME_UPPER "${MODULE_NAME}")
    string(TOUPPER "${NAME_UPPER}" NAME_UPPER)
    set_target_properties(${MODULE_NAME} PROPERTIES
                          DEFINE_SYMBOL ${NAME_UPPER}_EXPORTS
                          PREFIX "shard_"
                          DEBUG_POSTFIX "_debug"
                          )

    # add the main include directory
    target_include_directories(${MODULE_NAME} PUBLIC
                               $<BUILD_INTERFACE:${LOCAL_INCLUDE_DIR}>
                               $<INSTALL_INTERFACE:include>
                               )

    # define SHARD_STATIC if the build type is not set to 'shared'
    if (NOT BUILD_SHARED_LIBS)
        target_compile_definitions(${MODULE_NAME} PUBLIC "SHARD_STATIC")
    endif ()

    # setup the install rules

    install(DIRECTORY ${LOCAL_INCLUDE_DIR}/
            DESTINATION include
            FILES_MATCHING PATTERN "*.hpp"
            )

    install(TARGETS ${MODULE_NAME} EXPORT ${SHARD_CONFIG_EXPORT_NAME}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # dynamic library
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # static library
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
endmacro ()

# add a new header-only library target
#
# usage: shard_add_header_only_library(<name> <include_dir>)
macro (shard_add_header_only_library MODULE_NAME INCLUDE_DIR)
    add_library(${MODULE_NAME} INTERFACE)
    add_library(shard::${MODULE_NAME} ALIAS ${MODULE_NAME})

    # add the main include directory
    target_include_directories(${MODULE_NAME} INTERFACE
                               $<BUILD_INTERFACE:${INCLUDE_DIR}>
                               $<INSTALL_INTERFACE:include>
                               )

    # setup the install rules

    install(DIRECTORY ${INCLUDE_DIR}/
            DESTINATION include
            FILES_MATCHING PATTERN "*.hpp"
            )

    install(TARGETS ${MODULE_NAME} EXPORT ${SHARD_CONFIG_EXPORT_NAME}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
endmacro ()

# add a new example target
#
# usage: shard_add_example(<name>
#                          SOURCES <src>...
#                          [MODULES <module>...]
#                          [RESOURCE_DIR <dir>]
#                          )
macro (shard_add_example EXAMPLE_NAME)
    cmake_parse_arguments(LOCAL "" "RESOURCE_DIR" "SOURCES;MODULES" ${ARGN})

    set(TARGET_NAME "example.${EXAMPLE_NAME}")

    add_executable(${TARGET_NAME} ${LOCAL_SOURCES})

    # enable warnings
    shard_target_enable_warnings(${TARGET_NAME})

    target_link_libraries(${TARGET_NAME} ${LOCAL_MODULES})

    if (LOCAL_RESOURCE_DIR)
        file(GLOB RESOURCE_FILES ${LOCAL_RESOURCE_DIR}/*)
        file(COPY ${RESOURCE_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
    endif ()
endmacro ()

# add a new benchmark target
#
# usage: shard_add_benchmark(<name>
#                            SOURCES <src>...
#                            [INCLUDE_DIR <dir>]
#                            [MODULES <module>...]
#                            )
macro (shard_add_benchmark BENCHMARK_NAME)
    cmake_parse_arguments(LOCAL "" "INCLUDE_DIR" "SOURCES;MODULES" ${ARGN})
    set(TARGET_NAME "benchmark.${BENCHMARK_NAME}")
    add_executable(${TARGET_NAME} ${LOCAL_SOURCES})
    target_link_libraries(${TARGET_NAME} ${LOCAL_MODULES})
    target_include_directories(${TARGET_NAME} PRIVATE ${LOCAL_INCLUDE_DIR})
endmacro ()

# export all the library targets from the project
function (shard_export_targets)
    # CMAKE_CURRENT_LIST_DIR is not usable here
    set(CURRENT_DIR "${PROJECT_SOURCE_DIR}/cmake")

    # use the package helpers when installing
    include(CMakePackageConfigHelpers)

    set(TARGETS_CONFIG_FILENAME "shard-targets.cmake")
    set(CONFIG_PACKAGE_LOCATION "${CMAKE_INSTALL_LIBDIR}/cmake/shard")

    export(EXPORT ${SHARD_CONFIG_EXPORT_NAME}
           FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGETS_CONFIG_FILENAME}"
           )

    configure_package_config_file("${CURRENT_DIR}/shard-config.cmake"
                                  "${CMAKE_CURRENT_BINARY_DIR}/shard-config.cmake"
                                  INSTALL_DESTINATION ${CONFIG_PACKAGE_LOCATION}
                                  )

    install(EXPORT ${SHARD_CONFIG_EXPORT_NAME}
            FILE ${TARGETS_CONFIG_FILENAME}
            NAMESPACE "shard::"
            DESTINATION ${CONFIG_PACKAGE_LOCATION}
            )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/shard-config.cmake"
            DESTINATION ${CONFIG_PACKAGE_LOCATION}
            )
endfunction ()
