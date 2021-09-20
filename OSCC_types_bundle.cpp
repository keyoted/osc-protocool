#pragma once

#include "OSCC_types.hpp"

namespace OSCC::types {
        bundle::bundle(OSCC::types::time time)
        : time_(time)
        {}

        types::time bundle::time() const {
                return time_;
        }

        types::packets bundle::contents() const {
                return contents_;
        }
}
