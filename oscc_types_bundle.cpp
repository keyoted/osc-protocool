#pragma once

#include "oscc_types.hpp"

namespace oscc::types {
        bundle::bundle(oscc::types::time time)
        : time_(time)
        {}

        types::time bundle::time() const {
                return time_;
        }

        types::packets bundle::contents() const {
                return contents_;
        }
}
