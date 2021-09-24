#pragma once

#include "oscc_types.hpp"
#include "oscc_util.hpp"
#include <iostream>

namespace oscc::core::write {
        void int32(const type::int32 &data, std::vector<char> &bytes) {
                bytes.insert(bytes.end(), sizeof(type::int32), '\0');
                memcpy(&*(bytes.end() - sizeof(type::int32)), &data, sizeof(type::int32));
        }

        void float32(const type::float32 &data, std::vector<char> &bytes) {
                bytes.insert(bytes.end(), sizeof(type::float32), '\0');
                memcpy(&*(bytes.end() - sizeof(type::int32)), &data, sizeof(type::float32));
        }

        void time(const type::time &UNIX, std::vector<char> &bytes) {
                bytes.insert(bytes.end(), sizeof(type::time), '\0');
                const auto NTP = oscc::core::util::UNIXtoNTP(UNIX);
                memcpy(&*(bytes.end() - sizeof(type::time)), &NTP, sizeof(type::time));
        }

        void string(const type::string &data, std::vector<char> &bytes) {
                bytes.insert(bytes.end(), data.begin(), data.end());
                bytes.insert(bytes.end(), 4 - (data.size() % 4), '\0');
        }

        void blob(const type::blob &data, std::vector<char> &bytes) {
                int32(data.size(), bytes);
                bytes.insert(bytes.end(), data.begin(), data.end());
                bytes.insert(bytes.end(), 4 - (data.size() % 4), '\0');
        }
}