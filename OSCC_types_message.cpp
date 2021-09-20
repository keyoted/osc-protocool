#include "OSCC_types.hpp"

namespace OSCC::types {
        message::message(OSCC::types::address path)
        : address_(std::move(path))
        {}

        types::address message::address() const{
                return address_;
        }

        types::arguments message::arguments() const {
                return arguments_;
        }
}
