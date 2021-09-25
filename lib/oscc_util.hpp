#pragma once

#include <cstddef>
#include <stdexcept>
#include "oscc_types.hpp"

namespace oscc::core::util {
        template<typename T>
        class arrayConsumer {
                private:
                        T *data;
                        std::size_t size;
                public:
                        arrayConsumer(T *data, std::size_t size);
                        T *consume(std::size_t elements);
                        arrayConsumer<T> reserve(std::size_t limit);
                        T peek() const;
                        bool isEmpty();
                        T *consumeUntilOrThrow(const T &element);
        };

        oscc::type::time NTPtoUNIX(const oscc::type::time &NTP);
        oscc::type::time UNIXtoNTP(const oscc::type::time &UNIX);
        bool isMatch(type::string address, type::string pattern);
        bool isValidAddress(std::string address);
        bool isValidPattern(std::string pattern);
        std::variant<type::bundle, type::message> strToOSC(std::string OSCstr);
}

#include "oscc_util_arrayConsumer.hpp"