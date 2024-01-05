# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: cmake :: utility.cmake
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# enable warnings for the given target
function (shard_target_enable_warnings TARGET)
    if (MSVC)
        target_compile_options(${TARGET} PRIVATE /W4)
    else ()
        target_compile_options(${TARGET} PRIVATE
                               -Wall
                               -Wextra
                               -Wpedantic
                               # disable some warning
                               -Wno-gnu-zero-variadic-macro-arguments
                               )
    endif ()
endfunction ()

# get the subdirectories of the given directory
function (shard_list_subdirs OUT_DIRECTORIES DIRECTORY)
    file(GLOB CHILDREN RELATIVE ${DIRECTORY} ${DIRECTORY}/*)
    foreach (CHILD ${CHILDREN})
        if (IS_DIRECTORY ${DIRECTORY}/${CHILD})
            list(APPEND DIRECTORY_LIST ${CHILD})
        endif ()
    endforeach ()
    set(${OUT_DIRECTORIES} ${DIRECTORY_LIST} PARENT_SCOPE)
endfunction ()
