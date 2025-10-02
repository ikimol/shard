// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#include "shard/crypto/base64.hpp"

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>

namespace shard::crypto {

dynamic_data base64_encode(const data& data) {
    auto b64 = BIO_new(BIO_f_base64());
    auto mem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, mem);

    // disable newlines
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    BIO_write(b64, data.bytes(), static_cast<int>(data.size()));
    BIO_flush(b64);

    BUF_MEM* bptr;
    BIO_get_mem_ptr(b64, &bptr);

    dynamic_data result(bptr->length);
    std::memcpy(result.bytes(), bptr->data, bptr->length);

    BIO_free_all(b64);

    return result;
}

dynamic_data base64_decode(const data& data) {
    auto b64 = BIO_new(BIO_f_base64());
    auto mem = BIO_new_mem_buf(data.bytes(), static_cast<int>(data.size()));
    mem = BIO_push(b64, mem);

    // sisable newlines
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    // decoded size is at most 3/4 of encoded size
    dynamic_data result(data.size() * 3 / 4 + 3);

    auto decoded_length = BIO_read(mem, result.bytes(), static_cast<int>(data.size()));
    if (decoded_length < 0) {
        BIO_free_all(mem);
        return {};
    }

    BIO_free_all(mem);

    // reallocate to actual decoded length
    result.reallocate(decoded_length);

    return result;
}

} // namespace shard::crypto
