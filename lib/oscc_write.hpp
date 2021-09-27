#pragma once

#include "oscc_types.hpp"

namespace oscc::util::write {
        /**
         * Write a 32-bit integer to the binary data array.
         * @param data The 32-bit integer to write to the array.
         * @param bytes The data array where to write the int to.
         */
        void int32(const type::int32& data, std::vector<char>& bytes);

        /**
         * Write a 64-bit integer to the binary data array.
         * @param data The 64-bit integer to write to the array.
         * @param bytes The data array where to write the int to.
         */
        void int64(const type::int64& data, std::vector<char>& bytes);

        /**
         * Write a 32-bit float to the binary data array.
         * @param data The 32-bit float to write to the array.
         * @param bytes The data array where to write the float to.
         */
        void float32(const type::float32& data, std::vector<char>& bytes);

        /**
         * Write a 64-bit NTP timestamp to the binary data array.
         * @param data The timestamp to write to the array, converted to
         * milliseconds since the unix epoch (aka: a unix timestamp).
         * @param bytes The data array where to write the timestamp to.
         */
        void time(const type::time& UNIX, std::vector<char>& bytes);

        /**
         * Write a c-style string to the binary data array.
         * @param data The string to write to the binary data array.
         * @param bytes The data array where to write the string to.
         */
        void string(const type::string& data, std::vector<char>& bytes);

        /**
         * Write an OSC blob to the binary data array.
         * @param data The blob to write to the binary data array.
         * @param bytes The data array where to write the blob to.
         */
        void blob(const type::blob& data, std::vector<char>& bytes);

#ifdef OSCC_TYPE_d
        /**
         * Write a 64-bit float/double to the binary data array.
         * @param data The 64-bit float to write to the array.
         * @param bytes The data array where to write the float/double to.
         */
        void float64(const type::float64& data, std::vector<char>& bytes);
#endif
#ifdef OSCC_TYPE_m
        /**
         * Write 32-bit midi information to the binary data array.
         * @param data The 32-bit midi information to write to the array.
         * @param bytes The data array where to write the midi information to.
         */
        void midi(const type::midi& data, std::vector<char>& bytes);
#endif
#ifdef OSCC_TYPE_r
        /**
         * Write 32-bit color information to the binary data array.
         * @param data The 32-bit color information to write to the array.
         * @param bytes The data array where to write the color information to.
         */
        void rgba(const type::rgba& data, std::vector<char>& bytes);
#endif
}  // namespace oscc::util::write