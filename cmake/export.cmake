# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: cmake :: export.cmake
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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