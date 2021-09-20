#pragma once

#include <cstddef>
#include <stdexcept>

namespace OSCC::util {
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
}

#include "OSCC_util_arrayConsumer.hpp"