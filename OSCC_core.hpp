#pragma once

#include <stdexcept>
#include "OSCC_types.hpp"
#include "OSCC_read.hpp"
#include "OSCC_util.hpp"
#include "OSCC_write.hpp"

namespace oscc::core {
        types::packet bytesToOSC(char* bytes, size_t size);
        types::packet bytesToOSC(types::blob bytes);
        types::packet bytesToOSC(util::arrayConsumer<char> &bytes);
        std::vector<char> OSCToBytes(const types::packet& data);
}
