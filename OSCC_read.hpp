#pragma once

#include "OSCC_types.hpp"
#include "OSCC_util.hpp"

namespace OSCC::read {
        types::int32 int32(util::arrayConsumer<char> &data);

        types::float32 float32(util::arrayConsumer<char> &data);

        types::time time(util::arrayConsumer<char> &data);

        types::string string(util::arrayConsumer<char> &data);

        types::blob blob(util::arrayConsumer<char> &data);
}