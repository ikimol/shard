// Copyright (c) 2025 Miklos Molnar. All rights reserved.

#include <shard/runtime_id.hpp>

#include <cassert>
#include <functional>

namespace gui {

SHARD_RUNTIME_ID(widget_id, std::uint16_t)

} // namespace gui

SHARD_RUNTIME_ID_HASH_SUPPORT(gui, widget_id)

int main(int /* argc */, char* /* argv */[]) {
    auto w1 = gui::widget_id::next();
    auto w2 = gui::widget_id::next();
    assert(w1 != w2);
    assert(w1 < w2);

    return 0;
}
