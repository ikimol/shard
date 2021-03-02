# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: cmake :: macos.cmake
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# link the macOS frameworks to the target
#
# usage: shard_link_mac_frameworks(<target> <framework> [...])
macro(shard_link_mac_frameworks TARGET)
    foreach (FRAMEWORK_NAME ${ARGN})
        set(FRAMEWORK_LIB ${FRAMEWORK_NAME}_LIB)
        find_library(${FRAMEWORK_LIB} ${FRAMEWORK_NAME})
        if (${FRAMEWORK_LIB})
            list(APPEND FRAMEWORKS ${${FRAMEWORK_LIB}})
        endif ()
    endforeach ()
    target_link_libraries(${TARGET} PUBLIC ${FRAMEWORKS})
endmacro()
