#pragma once

#include "oscc_types.hpp"
#include "oscc_util.hpp"

namespace oscc::core::read {
        types::int32 int32(util::arrayConsumer<char> &data);

        types::float32 float32(util::arrayConsumer<char> &data);

        types::time time(util::arrayConsumer<char> &data);

        types::string string(util::arrayConsumer<char> &data);

        types::blob blob(util::arrayConsumer<char> &data);
}