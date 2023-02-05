// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include "shard/uuid.hpp"

#include <system_error>

#include <objbase.h>

namespace shard {

uuid uuid::make_system_uuid() {
    GUID id;
    HRESULT hr = ::CoCreateGuid(&id);

    if (FAILED(hr)) {
        throw std::system_error(hr, std::system_category(), "CoCreateGuid failed");
    }

    std::array<std::uint8_t, 16> bytes = {
    {(std::uint8_t)((id.Data1 >> 24) & 0xff), (std::uint8_t)((id.Data1 >> 16) & 0xff),
    (std::uint8_t)((id.Data1 >> 8) & 0xff), (std::uint8_t)((id.Data1) & 0xff), (std::uint8_t)((id.Data2 >> 8) & 0xff),
    (std::uint8_t)((id.Data2) & 0xff), (std::uint8_t)((id.Data3 >> 8) & 0xff), (std::uint8_t)((id.Data3) & 0xff),
    id.Data4[0], id.Data4[1], id.Data4[2], id.Data4[3], id.Data4[4], id.Data4[5], id.Data4[6], id.Data4[7]}};

    return uuid {std::begin(bytes), std::end(bytes)};
}

} // namespace shard
