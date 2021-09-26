#pragma once

#include <iostream>
#include "oscc_types.hpp"
#include "oscc_util.hpp"

namespace oscc::core::write {
        void int32(const type::int32& data, std::vector<char>& bytes) {
                bytes.insert(bytes.end(), sizeof(type::int32), '\0');
                memcpy(&*(bytes.end() - sizeof(type::int32)), &data, sizeof(type::int32));
        }

        void float32(const type::float32& data, std::vector<char>& bytes) {
                bytes.insert(bytes.end(), sizeof(type::float32), '\0');
                memcpy(&*(bytes.end() - sizeof(type::int32)), &data, sizeof(type::float32));
        }

        void time(const type::time& UNIX, std::vector<char>& bytes) {
                bytes.insert(bytes.end(), sizeof(type::time), '\0');
                const auto NTP = oscc::core::util::UNIXtoNTP(UNIX);
                memcpy(&*(bytes.end() - sizeof(type::time)), &NTP, sizeof(type::time));
        }

        void string(const type::string& data, std::vector<char>& bytes) {
                bytes.insert(bytes.end(), data.begin(), data.end());
                bytes.insert(bytes.end(), 4 - (data.size() % 4), '\0');
        }

        void blob(const type::blob& data, std::vector<char>& bytes) {
                int32(data.size(), bytes);
                bytes.insert(bytes.end(), data.begin(), data.end());
                bytes.insert(bytes.end(), 4 - (data.size() % 4), '\0');
        }

        void int64(const type::int64& data, std::vector<char>& bytes) {
                bytes.insert(bytes.end(), sizeof(type::int64), '\0');
                memcpy(&*(bytes.end() - sizeof(type::int64)), &data, sizeof(type::int64));
        }

#ifdef OSCC_TYPE_d
        void float64(const type::float64& data, std::vector<char>& bytes) {
                bytes.insert(bytes.end(), sizeof(type::float64), '\0');
                memcpy(&*(bytes.end() - sizeof(type::float64)), &data, sizeof(type::float64));
        }
#endif

#ifdef OSCC_TYPE_m
        void midi(const type::midi& data, std::vector<char>& bytes) {
                bytes.insert(bytes.end(), sizeof(type::midi), '\0');
                memcpy(&*(bytes.end() - sizeof(type::midi)), &data, sizeof(type::midi));
        }
#endif

#ifdef OSCC_TYPE_r
        void rgba(const type::rgba& data, std::vector<char>& bytes) {
                bytes.insert(bytes.end(), sizeof(type::rgba), '\0');
                memcpy(&*(bytes.end() - sizeof(type::rgba)), &data, sizeof(type::rgba));
        }
#endif
}  // namespace oscc::core::write