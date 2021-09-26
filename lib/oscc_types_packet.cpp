#include "oscc_types.hpp"
#include "oscc_util.hpp"

namespace oscc::type {
        packet::packet(std::string OSCstring) :variant(oscc::core::util::strToOSC(std::move(OSCstring))) {}

        std::string packet::string() const {
                const auto &var = *this;
                if(std::holds_alternative<bundle>(var)) {
                        return std::get<bundle>(var).string();
                } else if(std::holds_alternative<message>(var)) {
                        return std::get<message>(var).string();
                } else return "";
        }
}