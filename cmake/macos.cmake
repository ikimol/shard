# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: cmake :: macos.cmake
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# collect Apple frameworks into the variable
#
# usage: shard_find_frameworks(<variable> <framework> [...])
macro (shard_find_frameworks OUT_VAR)
    foreach (FRAMEWORK_NAME ${ARGN})
        set(FRAMEWORK_LIB ${FRAMEWORK_NAME}_LIB)
        find_library(${FRAMEWORK_LIB} ${FRAMEWORK_NAME})
        if (${FRAMEWORK_LIB})
            list(APPEND ${OUT_VAR} ${${FRAMEWORK_LIB}})
        endif ()
    endforeach ()
endmacro ()

# link the frameworks to the target
#
# usage: shard_link_frameworks(<target> <framework> [...])
macro (shard_link_frameworks TARGET)
    shard_find_frameworks(FRAMEWORKS ${ARGN})
    target_link_libraries(${TARGET} PUBLIC ${FRAMEWORKS})
endmacro ()
