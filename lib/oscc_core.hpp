#pragma once

#include <stdexcept>
#include "oscc_types.hpp"
#include "oscc_read.hpp"
#include "oscc_util.hpp"
#include "oscc_write.hpp"

namespace oscc::core {
        types::packet bytesToOSC(char* bytes, size_t size);
        types::packet bytesToOSC(types::blob bytes);
        types::packet bytesToOSC(util::arrayConsumer<char> &bytes);
        std::vector<char> OSCToBytes(const types::packet& data);
}
