#pragma once

#include "OSCC_types.hpp"
#include <iostream>

namespace OSCC::write {
        void int32(const types::int32 &data, std::vector<char> &bytes) {
                bytes.insert(bytes.end(), sizeof(types::int32), '\0');
                memcpy(&*(bytes.end() - sizeof(types::int32)), &data, sizeof(types::int32));
        }

        void float32(const types::float32 &data, std::vector<char> &bytes) {
                bytes.insert(bytes.end(), sizeof(types::float32), '\0');
                memcpy(&*(bytes.end() - sizeof(types::int32)), &data, sizeof(types::float32));
        }

        void time(const types::time &data, std::vector<char> &bytes) {
                bytes.insert(bytes.end(), sizeof(types::time), '\0');
                memcpy(&*(bytes.end() - sizeof(types::time)), &data, sizeof(types::time));
        }

        void string(const types::string &data, std::vector<char> &bytes) {
                bytes.insert(bytes.end(), data.begin(), data.end());
                bytes.insert(bytes.end(), 4 - (data.size() % 4), '\0');
        }

        void blob(const types::blob &data, std::vector<char> &bytes) {
                int32(data.size(), bytes);
                bytes.insert(bytes.end(), data.begin(), data.end());
                bytes.insert(bytes.end(), 4 - (data.size() % 4), '\0');
        }
}