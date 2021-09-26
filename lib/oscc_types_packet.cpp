#include "oscc_types.hpp"
#include "oscc_util.hpp"

namespace oscc::type {
        packet::packet(std::string OSCstring) : variant(oscc::core::util::strToOSC(std::move(OSCstring))) {}

        std::string packet::string() const { return core::util::string(*this); }
}  // namespace oscc::type