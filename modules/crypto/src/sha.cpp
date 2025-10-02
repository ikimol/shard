// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#include "shard/crypto/sha.hpp"

#include <openssl/evp.h>

namespace shard::crypto {

dynamic_data sha256(const data& data) {
    auto ctx = EVP_MD_CTX_new();
    EVP_DigestInit(ctx, EVP_sha256());
    EVP_DigestUpdate(ctx, data.bytes(), data.size());

    dynamic_data result(EVP_MD_size(EVP_sha256()));

    unsigned int length;
    EVP_DigestFinal(ctx, reinterpret_cast<unsigned char*>(result.bytes()), &length);

    EVP_MD_CTX_free(ctx);

    return result;
}

std::string sha256_string(const data& data) {
    auto digest = sha256(data);
    auto bytes = reinterpret_cast<unsigned char*>(digest.bytes());
    std::string buffer(64, '\0');
    for (auto i = 0; i < EVP_MD_size(EVP_sha256()); i++) {
        std::snprintf(buffer.data() + i * 2, 3, "%02x", bytes[i]);
    }
    return buffer;
}

} // namespace shard::crypto
