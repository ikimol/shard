# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: cmake :: os.cmake
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if (UNIX)
    set(SHARD_OS_UNIX 1)
endif ()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(SHARD_OS_LINUX 1)
elseif (ANDROID)
    set(SHARD_OS_ANDROID 1)
elseif (APPLE)
    set(SHARD_OS_DARWIN 1)
    if (IOS)
        set(SHARD_OS_IOS 1)
    else ()
        set(SHARD_OS_MAC 1)
    endif ()
elseif (WIN32)
    set(SHARD_OS_WINDOWS 1)
elseif (CYGWIN)
    set(SHARD_OS_CYGWIN 1)
else ()
    message(FATAL_ERROR "Unsupported operating system or environment")
    return()
endif ()
