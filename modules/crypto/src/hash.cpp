// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#include "shard/crypto/hash.hpp"

#include <shard/enums/type.hpp>

#include <array>

#include <openssl/evp.h>

namespace shard::crypto {
namespace {

std::array g_algorithms = {EVP_md5, EVP_sha256};

} // namespace

hash::hash(algorithm algorithm, const data& input)
: m_digest(EVP_MD_size(g_algorithms[to_underlying(algorithm)]())) {
    auto ctx = EVP_MD_CTX_new();
    EVP_DigestInit(ctx, g_algorithms[to_underlying(algorithm)]());
    EVP_DigestUpdate(ctx, input.bytes(), input.size());
    EVP_DigestFinal(ctx, reinterpret_cast<unsigned char*>(m_digest.bytes()), nullptr);
    EVP_MD_CTX_free(ctx);
}

std::string hash::to_string() const {
    auto bytes = reinterpret_cast<unsigned char*>(m_digest.bytes());
    std::string result(m_digest.size() * 2, '\0');
    for (std::size_t i = 0; i < m_digest.size(); i++) {
        std::snprintf(result.data() + i * 2, 3, "%02x", bytes[i]);
    }
    return result;
}

} // namespace shard::crypto
