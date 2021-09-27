#include "oscc_types.hpp"
#include "oscc_util.hpp"

namespace oscc::type {
        message::message(oscc::type::address pattern) : address_pattern_(std::move(pattern)) {}

        type::address&         message::pattern() { return address_pattern_; }

        type::arguments&       message::arguments() { return arguments_; }

        const type::address&   message::pattern() const { return address_pattern_; }

        const type::arguments& message::arguments() const { return arguments_; }

        void                   message::push(type::argument& arg) { arguments_.insert(arguments_.end(), arg); }

        std::string            message::string() const { return util::string(*this); }
}  // namespace oscc::type
