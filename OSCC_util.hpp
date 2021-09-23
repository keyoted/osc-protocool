#pragma once

#include <cstddef>
#include <stdexcept>
#include "OSCC_types.hpp"

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

        oscc::types::time NTPtoUNIX(const oscc::types::time &NTP);
        oscc::types::time UNIXtoNTP(const oscc::types::time &UNIX);
}

#include "OSCC_util_arrayConsumer.hpp"