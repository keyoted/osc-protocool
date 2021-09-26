#pragma once

#include <cstddef>
#include <stdexcept>
#include "oscc_types.hpp"

namespace oscc::core::util {
        template <typename T>
        class arrayConsumer {
                private:
                        T*          data;
                        std::size_t size;

                public:
                        arrayConsumer(T* data, std::size_t size);
                        T*               consume(std::size_t elements);
                        arrayConsumer<T> reserve(std::size_t limit);
                        T                peek() const;
                        bool             isEmpty();
                        T*               consumeUntilOrThrow(const T& element);
        };

        type::time   NTPtoUNIX(const oscc::type::time& NTP);
        type::time   UNIXtoNTP(const oscc::type::time& UNIX);
        bool         isMatch(type::string address, type::string pattern);
        bool         isValidAddress(std::string address);
        bool         isValidPattern(std::string pattern);
        type::packet strToOSC(const std::string& OSC_str);
        std::string  string(const type::arguments& args);
        std::string  string(const type::bundle& bdl);
        std::string  string(const type::message& msg);
        std::string  string(const type::packet& pkt);
        type::time   getCurrentTime();
}  // namespace oscc::core::util

#include "oscc_util_arrayConsumer.hpp"