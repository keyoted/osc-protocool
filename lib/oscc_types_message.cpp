#include "oscc_types.hpp"

namespace oscc::type {
        message::message(oscc::type::address path)
        : address_pattern_(std::move(path))
        {}

        type::address message::pattern() const {
                return address_pattern_;
        }

        type::arguments message::arguments() const {
                return arguments_;
        }
}
