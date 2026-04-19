// Copyright (c) 2026 Miklos Molnar. All rights reserved.

#pragma once

#define SHARD_INTERNAL_LAZY_STREAM(stream, condition)                                                                  \
    switch (0)                                                                                                         \
    case 0:                                                                                                            \
    default:                                                                                                           \
        if (!(condition))                                                                                              \
            ;                                                                                                          \
        else                                                                                                           \
            (stream)

#define SHARD_LOG(logger, level)                                                                                       \
    SHARD_INTERNAL_LAZY_STREAM(                                                                                        \
        ::shard::log::message((logger), (level), std::time(nullptr), SHARD_CURRENT_SOURCE_LOCATION),                   \
        (level) >= (logger).min_level())

#if defined(NDEBUG)
#define SHARD_LOG_TRACE(logger) SHARD_INTERNAL_LAZY_STREAM(::shard::log::detail::null_message {}, false)
#define SHARD_LOG_DEBUG(logger) SHARD_INTERNAL_LAZY_STREAM(::shard::log::detail::null_message {}, false)
#else
#define SHARD_LOG_TRACE(logger) SHARD_LOG((logger), ::shard::log::level::type::trace)
#define SHARD_LOG_DEBUG(logger) SHARD_LOG((logger), ::shard::log::level::type::debug)
#endif

#define SHARD_LOG_INFO(logger) SHARD_LOG((logger), ::shard::log::level::type::info)
#define SHARD_LOG_WARN(logger) SHARD_LOG((logger), ::shard::log::level::type::warn)
#define SHARD_LOG_ERROR(logger) SHARD_LOG((logger), ::shard::log::level::type::error)
#define SHARD_LOG_FATAL(logger) SHARD_LOG((logger), ::shard::log::level::type::fatal)

// short macros

#define SHARD_TLOG(logger) SHARD_LOG_TRACE(logger)
#define SHARD_DLOG(logger) SHARD_LOG_DEBUG(logger)
#define SHARD_ILOG(logger) SHARD_LOG_INFO(logger)
#define SHARD_WLOG(logger) SHARD_LOG_WARN(logger)
#define SHARD_ELOG(logger) SHARD_LOG_ERROR(logger)
#define SHARD_FLOG(logger) SHARD_LOG_FATAL(logger)
