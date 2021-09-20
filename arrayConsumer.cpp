#include "arrayConsumer.hpp"

namespace OSCC::util {
        template<typename T>
        arrayConsumer::arrayConsumer(T *data, std::size_t size)
                :data(data), size(size) {}

        template<typename T>
        T *arrayConsumer::consume(std::size_t elements) {
                if (size > elements) throw std::out_of_range("Invalid access");
                T *n = data;
                data += elements;
                size -= elements;
                return n;
        }

        template<typename T>
        arrayConsumer<T> arrayConsumer::reserve(std::size_t limit) {
                if (limit > size) throw std::out_of_range("Cannot extend range");
                T *n = data;
                data += limit;
                size -= limit;
                return arrayConsumer(n, limit);
        }

        template<typename T>
        T arrayConsumer::peek() {
                if (size == 0) throw std::out_of_range("Invalid access");
                return *data;
        }

        template<typename T>
        bool arrayConsumer::isEmpty() {
                return size == 0;
        }

        template<typename T>
        T *arrayConsumer::consumeUntilOrThrow(const T &element) {
                for (std::size_t consumed = 0; size - consumed != 0; consumed++) {
                        if (data[consumed] == element) {
                                return consume(consumed + 1);
                        }
                }
                throw std::out_of_range("Element not found in range");
        }
}