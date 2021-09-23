#pragma once

#include "OSCC_util.hpp"

namespace oscc::core::util {
        template<typename T>
        arrayConsumer<T>::arrayConsumer(T *data, std::size_t size)
                :data(data), size(size) {}

        template<typename T>
        T *arrayConsumer<T>::consume(std::size_t elements) {
                if (elements > size) throw std::out_of_range("Invalid access");
                T *n = data;
                data += elements;
                size -= elements;
                return n;
        }

        template<typename T>
        arrayConsumer<T> arrayConsumer<T>::reserve(std::size_t limit) {
                if (limit > size) throw std::out_of_range("Cannot extend range");
                T *n = data;
                data += limit;
                size -= limit;
                return arrayConsumer(n, limit);
        }

        template<typename T>
        T arrayConsumer<T>::peek() const {
                if (size == 0) throw std::out_of_range("Invalid access");
                return *data;
        }

        template<typename T>
        bool arrayConsumer<T>::isEmpty() {
                return size == 0;
        }

        template<typename T>
        T *arrayConsumer<T>::consumeUntilOrThrow(const T &element) {
                for (std::size_t consumed = 0; size - consumed != 0; consumed++) {
                        if (data[consumed] == element) {
                                return consume(consumed + 1);
                        }
                }
                throw std::out_of_range("Element not found in range");
        }
}