// Copyright (c) 2023 Miklos Molnar. All rights reserved.

#include <shard/algorithm/variant_switch.hpp>

#include <iostream>
#include <string>
#include <type_traits>

namespace input_device {

struct pencil {};

struct keyboard_and_pointer {
    enum pointer_type {
        mouse,
        trackpad,
    } type;
};

} // namespace input_device

using input_device_t = std::variant<input_device::keyboard_and_pointer, input_device::pencil>;

int main(int /* argc */, char* /* argv */[]) {
    input_device_t id = input_device::pencil {};
    shard::variant_switch(
        id,
        [](input_device::keyboard_and_pointer input_device) {
            std::cout << "keyboard and " << input_device.type << '\n';
        },
        [](input_device::pencil) { std::cout << "pencil\n"; });

    return 0;
}
