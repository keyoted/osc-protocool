#pragma once

#include <stdexcept>
#include "oscc_read.hpp"
#include "oscc_types.hpp"
#include "oscc_util.hpp"
#include "oscc_write.hpp"

namespace oscc::core {
        type::packet      bytesToOSC(char* bytes, size_t size);
        type::packet      bytesToOSC(type::blob bytes);
        type::packet      bytesToOSC(util::arrayConsumer<char>& bytes);
        std::vector<char> OSCToBytes(const type::packet& data);
}  // namespace oscc::core
