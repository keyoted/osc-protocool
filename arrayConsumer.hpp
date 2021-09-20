#pragma once

#include <cstddef>

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

                        T peek();

                        bool isEmpty();

                        T *consumeUntilOrThrow(const T &element);
        };
}