// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include "shard/uuid.hpp"

#include <uuid/uuid.h>

namespace shard {

uuid uuid::make_system_uuid() {
    uuid_t id;
    uuid_generate(id);

    std::array<std::uint8_t, 16> bytes = {{id[0],
                                           id[1],
                                           id[2],
                                           id[3],
                                           id[4],
                                           id[5],
                                           id[6],
                                           id[7],
                                           id[8],
                                           id[9],
                                           id[10],
                                           id[11],
                                           id[12],
                                           id[13],
                                           id[14],
                                           id[15]}};

    return uuid {std::begin(bytes), std::end(bytes)};
}

} // namespace shard
