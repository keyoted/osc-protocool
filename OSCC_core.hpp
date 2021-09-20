#pragma once

#include "OSCC_types.hpp"
#include "OSCC_read.hpp"
#include "arrayConsumer.hpp"

namespace OSCC::core {
        types::packet extractOSCPacket(arrayConsumer<char> &raw);
}
