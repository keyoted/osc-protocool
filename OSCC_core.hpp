#pragma once

#include "OSCC_types.hpp"
#include "OSCC_read.hpp"
#include "OSCC_util.hpp"

namespace OSCC::core {
        types::packet extractOSCPacket(util::arrayConsumer<char> &raw);
}
