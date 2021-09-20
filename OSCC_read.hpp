#pragma once

#include "OSCC_types.hpp"
#include "arrayConsumer.hpp"

namespace OSCC::read {
        types::int32 int32(arrayConsumer<char> &data);

        types::float32 float32(arrayConsumer<char> &data);

        types::time time(arrayConsumer<char> &data);

        types::string string(arrayConsumer<char> &data);

        types::blob blob(arrayConsumer<char> &data);
}