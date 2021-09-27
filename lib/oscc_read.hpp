#pragma once

#include "oscc_types.hpp"
#include "oscc_util.hpp"

/**
 * The read namespace contains functions that are useful for reading OSC types
 * from binary data arrays.
 */
namespace oscc::util::read {
        /**
         * Read a 32-bit integer from the binary data array.
         * @param data The data array where to read the int from.
         * @return The 32-bit integer read from the first 4 bytes of the array.
         */
        type::int32 int32(util::arrayConsumer<char>& data);

        /**
         * Read a 64-bit integer from the binary data array.
         * @param data The data array where to read the int from.
         * @return The 64-bit integer read from the first 8 bytes of the array.
         */
        type::int64 int64(util::arrayConsumer<char>& data);

        /**
         * Read a 32-bit float from the binary data array.
         * @param data The data array where to read the float from.
         * @return The 32-bit float read from the first 4 bytes of the array.
         */
        type::float32 float32(util::arrayConsumer<char>& data);

        /**
         * Read a 64-bit NTP timestamp from the binary data array.
         * @param data The data array where to read the timestamp from.
         * @return The timestamp read from the first 8 bytes of the array,
         * converted to milliseconds since the unix epoch (aka: a unix
         * timestamp).
         */
        type::time time(util::arrayConsumer<char>& data);

        /**
         * Read a c-style string from the binary data array.
         * @param data The data array where to read the string from.
         * @return The string read from the binary data array, at least 1 byte.
         */
        type::string string(util::arrayConsumer<char>& data);

        /**
         * Read an OSC blob from the binary data array.
         * @param data The data array where to read the blob from.
         * @return The blob read from the binary data array, at least 4 bytes.
         */
        type::blob blob(util::arrayConsumer<char>& data);

#ifdef OSCC_TYPE_d
        /**
         * Read a 64-bit float/double from the binary data array.
         * @param data The data array where to read the float/double from.
         * @return The 64-bit float read from the first 8 bytes of the array.
         */
        type::float64 float64(util::arrayConsumer<char>& data);
#endif
#ifdef OSCC_TYPE_m
        /**
         * Read 32-bit midi information from the binary data array.
         * @param data The data array where to read the midi information from.
         * @return The 32-bit midi information read from the first 4 bytes of
         * the array.
         */
        type::midi midi(util::arrayConsumer<char>& data);
#endif
#ifdef OSCC_TYPE_r
        /**
         * Read 32-bit color information from the binary data array.
         * @param data The data array where to read the color information from.
         * @return The 32-bit color information read from the first 4 bytes of
         * the array.
         */
        type::rgba rgba(util::arrayConsumer<char>& data);
#endif
}  // namespace oscc::util::read