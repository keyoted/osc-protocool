#include "oscc_types.hpp"

namespace oscc::types {
        message::message(oscc::types::address path)
        : address_pattern_(std::move(path))
        {}

        types::address message::address() const {
                return address_pattern_;
        }

        types::arguments message::arguments() const {
                return arguments_;
        }
}
