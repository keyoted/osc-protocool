#pragma once

#include "oscc_types.hpp"

namespace oscc::core::write {
        void int32(const types::int32 &data, std::vector<char> &bytes);
        void float32(const types::float32 &data, std::vector<char> &bytes);
        void time(const types::time &UNIX, std::vector<char> &bytes);
        void string(const types::string &data, std::vector<char> &bytes);
        void blob(const types::blob &data, std::vector<char> &bytes);
}