#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>

/**
 * \def OSCC_TYPE1_1
 * If defined, this macro enables the types defined in the 1.1 specification
 * including: true/false, null, impulse and time.
 */

/**
 * \def OSCC_TYPE_NON_STD
 * If defined, this macro enables the types mentioned in the specifications as
 * being non-standard including: all the types in the 1.1 specification,
 * 64-bit integers, 64-bit double/float, symbols, ascii characters, rgba colors,
 * midi messages, and arrays.
 */

/** OSCC_TYPE_TF
 * If defined the true/false type is available.
 */

/** OSCC_TYPE_N
 * If defined the null type is available.
 */

/** OSCC_TYPE_I
 * If defined the impulse type is available.
 */

/** OSCC_TYPE_t
 * If defined the time type is available.
 */

/** OSCC_TYPE_h
 * If defined the 64-bit integer type is available.
 */

/** OSCC_TYPE_d
 * If defined the 64-bit float/double type is available.
 */

/** OSCC_TYPE_S
 * If defined the symbol type is available.
 */

/** OSCC_TYPE_c
 * If defined the character type is available.
 */

/** OSCC_TYPE_m
 * If defined the midi type is available.
 */

/** OSCC_TYPE_r
 * If defined the rgba type is available.
 */

/** OSCC_TYPE_ARR_
 * If defined the array type is available.
 */

/** OSCC_TYPES_VAL
 * If defined the types true/false and/or null and/or and/or impulse are
 * available, this types do not take any data and are defined on their own
 * structure.
 */

#ifdef OSCC_TYPE_NON_STD
        #define OSCC_TYPE1_1
        #define OSCC_TYPE_h
        #define OSCC_TYPE_d
        #define OSCC_TYPE_S
        #define OSCC_TYPE_c
        #define OSCC_TYPE_m
        #define OSCC_TYPE_r
        #define OSCC_TYPE_ARR_
#endif

#ifdef OSCC_TYPE1_1
        #define OSCC_TYPE_TF
        #define OSCC_TYPE_N
        #define OSCC_TYPE_I
        #define OSCC_TYPE_t
#endif

#if defined(OSCC_TYPE_TF) || defined(OSCC_TYPE_N) || defined(OSCC_TYPE_I)
        #define OSCC_TYPES_VAL
#endif

namespace oscc::type {

        /** Represents a 32-bit integer. */
        typedef std::int32_t int32;

        /** Represents a 32-bit unsigned integer. */
        typedef std::uint32_t uint32;

        /** Represents a 64-bit integer. */
        typedef std::int64_t int64;

        /** Represents a 32-bit float. */
        typedef std::float_t float32;

#ifdef OSCC_TYPE_d
        /** Represents a 64-bit float. */
        typedef long double float64;
#endif

#ifdef OSCC_TYPE_m
        /** Represents midi data. */
        typedef struct {
                        unsigned char port_ID;
                        unsigned char status;
                        unsigned char data_1;
                        unsigned char data_2;
        } midi;
#endif

#ifdef OSCC_TYPE_r
        /** Represents rgba data. */
        typedef struct {
                        unsigned char red;
                        unsigned char green;
                        unsigned char blue;
                        unsigned char alpha;
        } rgba;
#endif

#ifdef OSCC_TYPE_S
        /** Represents symbol data. */
        class symbol {
                public:
                        std::string identifier;
                        symbol(const std::string&& str) : identifier(str){};
        };
#endif

        /** Represents a string. */
        typedef std::string string;

        /** Represents a blob. */
        typedef std::vector<char> blob;

        /** Represents an OSC address or pattern. */
        typedef std::filesystem::path address;

        /** Represents time data. */
        typedef union {
                        int64 unix;
                        int64 ntp;
        } time;

#ifdef OSCC_TYPES_VAL
        /**
         * Represents an argume without a value, its value is the argument
         * itself.
         * */
        enum value_argument {
        #ifdef OSCC_TYPE_TF
                T,
                F,
        #endif
        #ifdef OSCC_TYPE_N
                N,
        #endif
        #ifdef OSCC_TYPE_I
                I,
        #endif
        };
#endif

#ifdef OSCC_TYPE_ARR_
        class arguments;
#endif

        /** Represents an argument that chan be passed to a OSC method. */
        typedef std::variant<int32,
                             float32,
                             string,
                             blob
#ifdef OSCC_TYPES_VAL
                             ,
                             value_argument
#endif
#ifdef OSCC_TYPE_t
                             ,
                             time
#endif
#ifdef OSCC_TYPE_h
                             ,
                             int64
#endif
#ifdef OSCC_TYPE_d
                             ,
                             float64
#endif
#ifdef OSCC_TYPE_c
                             ,
                             char
#endif
#ifdef OSCC_TYPE_m
                             ,
                             midi
#endif
#ifdef OSCC_TYPE_r
                             ,
                             rgba
#endif
#ifdef OSCC_TYPE_S
                             ,
                             symbol
#endif
#ifdef OSCC_TYPE_ARR_
                             ,
                             arguments
#endif
                             >
                argument;

        /** Represents a group of arguments. */
        class arguments : public std::vector<argument> {};

        /** Represents a packet that is either a message or a bundle. */
        class packet;

        /** Represents a group of packets */
        typedef std::vector<packet> packets;

        /** Represents a system method that can be called via OSC */
        typedef std::function<void(const packet&)> call_back;
}  // namespace oscc::type

#include "oscc_types_bundle.hpp"
#include "oscc_types_message.hpp"
#include "oscc_types_packet.hpp"