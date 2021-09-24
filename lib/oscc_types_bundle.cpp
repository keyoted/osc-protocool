#pragma once

#include "oscc_types.hpp"

namespace oscc::type {
        bundle::bundle(oscc::type::time time)
        : time_(time)
        {}

        type::time bundle::time() const {
                return time_;
        }

        type::packets bundle::contents() const {
                return contents_;
        }
}
