// Copyright (c) 2021 Miklos Molnar. All rights reserved.

#include <shard/meta/type_traits.hpp>
#include <shard/utility/preprocessor.hpp>

int main(int /* argc */, char* /* argv */[]) {
    auto lambda = [](int i, const char* ptr) -> bool {
        return i > 0 && ptr != nullptr;
    };
    SHARD_UNUSED(lambda);

    using traits = shard::functor_traits<decltype(lambda)>;
    static_assert(traits::arity == 2);
    static_assert(std::is_same_v<traits::result_type, bool>);
    static_assert(std::is_same_v<traits::arg_type<0>, int>);
    static_assert(std::is_same_v<traits::arg_type<1>, const char*>);

    return 0;
}
