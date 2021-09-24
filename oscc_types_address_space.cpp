#include "oscc_types.hpp"

namespace oscc::types {

        void address_space::registerFunction(const address a, const call_back c) {
                // TODO: ensure address contains only legal characters
                // TODO: ensure address is only address and not address pattern
                functions_.insert_or_assign(a.string(), c);
        }

}