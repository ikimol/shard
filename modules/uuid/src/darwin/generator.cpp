// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include "shard/uuid.hpp"

#include <CoreFoundation/CoreFoundation.h>

namespace shard {

uuid uuid::make_system_uuid() {
    auto id = CFUUIDCreate(nullptr);
    auto id_bytes = CFUUIDGetUUIDBytes(id);
    CFRelease(id);

    std::array<std::uint8_t, 16> bytes = {{
        id_bytes.byte0,
        id_bytes.byte1,
        id_bytes.byte2,
        id_bytes.byte3,
        id_bytes.byte4,
        id_bytes.byte5,
        id_bytes.byte6,
        id_bytes.byte7,
        id_bytes.byte8,
        id_bytes.byte9,
        id_bytes.byte10,
        id_bytes.byte11,
        id_bytes.byte12,
        id_bytes.byte13,
        id_bytes.byte14,
        id_bytes.byte15,
    }};

    return uuid {std::begin(bytes), std::end(bytes)};
}

} // namespace shard
