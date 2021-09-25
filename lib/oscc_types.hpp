#pragma once

#include <cstdint>
#include <cmath>
#include <vector>
#include <string>
#include <filesystem>
#include <variant>
#include <functional>
#include <map>

#ifdef OSCC_TYPE_NON_STD
        #define OSCC_TYPE1_1
        #define OSCC_TYPE_h
        #define OSCC_TYPE_d
        #define OSCC_TYPE_c
        #define OSCC_TYPE_m
        #define OSCC_TYPE_r
        #define OSCC_TYPE_S
        #define OSCC_TYPE_ARR_
#endif

#ifdef OSCC_TYPE1_1
        #define OSCC_TYPE_TF
        #define OSCC_TYPE_N
        #define OSCC_TYPE_I
        #define OSCC_TYPE_t
#endif

namespace oscc::type {

        class message;

        class bundle;

        typedef std::int32_t int32;

        typedef std::uint32_t uint32;

        typedef std::int64_t int64;

        typedef std::float_t float32;

#ifdef OSCC_TYPE_d
        typedef std::double_t float64;
#endif

#ifdef OSCC_TYPE_m
        typedef struct {
                unsigned char port_ID;
                unsigned char status;
                unsigned char data_1;
                unsigned char data_2;
        } midi;
#endif

#ifdef OSCC_TYPE_r
        typedef struct {
                unsigned char red;
                unsigned char green;
                unsigned char blue;
                unsigned char alpha;
        } rgba;
#endif

#ifdef OSCC_TYPE_S
        class symbol {
                public:
                        std::string value;
                        symbol(const std::string && str) : value(str) {};
        };
#endif

        typedef std::string string;

        typedef std::vector<char> blob;

        typedef std::filesystem::path address;

        typedef union {
                int64 unix;
                int64 ntp;
        } time;

        #if defined(OSCC_TYPE_TF) || defined(OSCC_TYPE_N) || defined(OSCC_TYPE_I)
                #define OSCC_TYPES_VAL
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

        typedef std::variant<
                int32
                ,float32
                ,string
                ,blob
                #ifdef OSCC_TYPES_VAL
                        ,value_argument
                #endif
                #ifdef OSCC_TYPE_t
                        ,time
                #endif
                #ifdef OSCC_TYPE_h
                        ,int64
                #endif
                #ifdef OSCC_TYPE_d
                        ,float64
                #endif
                #ifdef OSCC_TYPE_c
                        ,char
                #endif
                #ifdef OSCC_TYPE_m
                        ,midi
                #endif
                #ifdef OSCC_TYPE_r
                        ,rgba
                #endif
                #ifdef OSCC_TYPE_S
                        ,symbol
                #endif
                #ifdef OSCC_TYPE_ARR_
                        ,arguments
                #endif
                > argument;


        class arguments : public std::vector<argument> {};

        class packet;

        typedef std::vector<packet> packets;

        typedef std::function<void(packet)> call_back;

        class message {
                private:
                        type::address address_pattern_;
                        type::arguments arguments_;

                public:
                        explicit message(type::address path);
                        template<typename T> void push(T && val) { arguments_.push_back(std::forward<T>(val)); }
                        void push(type::argument &arg);
                        [[nodiscard]] type::address& pattern();
                        [[nodiscard]] type::arguments& arguments();
        };

        class bundle {
                private:
                        type::time time_;
                        type::packets contents_;

                public:
                        explicit bundle(type::time time);
                        template<typename T> void push(T && val) { contents_.push_back(std::forward<T>(val)); }
                        [[nodiscard]] type::time time() const;
                        [[nodiscard]] type::packets contents() const;
        };

        class packet : public std::variant<bundle, message> {
                public:
                        packet(std::string OSCstring);
                        template<typename T> packet(T t) :variant(t) {};
        };
}