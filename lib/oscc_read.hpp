#pragma once

#include "oscc_types.hpp"
#include "oscc_util.hpp"

namespace oscc::core::read {
        type::int32   int32(util::arrayConsumer<char>& data);
        type::int64   int64(util::arrayConsumer<char>& data);
        type::float32 float32(util::arrayConsumer<char>& data);
        type::time    time(util::arrayConsumer<char>& data);
        type::string  string(util::arrayConsumer<char>& data);
        type::blob    blob(util::arrayConsumer<char>& data);
#ifdef OSCC_TYPE_d
        type::float64 float64(util::arrayConsumer<char>& data);
#endif
#ifdef OSCC_TYPE_m
        type::midi midi(util::arrayConsumer<char>& data);
#endif
#ifdef OSCC_TYPE_r
        type::rgba rgba(util::arrayConsumer<char>& data);
#endif
}  // namespace oscc::core::read