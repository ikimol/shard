# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: cmake :: utility.cmake
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# enable warnings for the given target
function(shard_target_enable_warnings TARGET)
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
endfunction()
