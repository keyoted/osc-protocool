#pragma once

#include "oscc_types.hpp"

namespace oscc::core::write {
        void int32(const type::int32 &data, std::vector<char> &bytes);
        void float32(const type::float32 &data, std::vector<char> &bytes);
        void time(const type::time &UNIX, std::vector<char> &bytes);
        void string(const type::string &data, std::vector<char> &bytes);
        void blob(const type::blob &data, std::vector<char> &bytes);
}