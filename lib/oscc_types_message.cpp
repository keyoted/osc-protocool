#include "oscc_types.hpp"

namespace oscc::type {
        message::message(oscc::type::address path)
        : address_pattern_(std::move(path))
        {}

        type::address& message::pattern() {
                return address_pattern_;
        }

        type::arguments& message::arguments() {
                return arguments_;
        }

        void message::push(type::argument &arg) {
                arguments_.insert(arguments_.end(), arg);
        }
}
