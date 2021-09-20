#pragma once

#include <cstdint>
#include <cmath>
#include <vector>
#include <string>
#include <filesystem>
#include <variant>

namespace OSCC::types {

        class message;

        class bundle;

        typedef std::int32_t int32;

        typedef std::float_t float32;

        typedef std::string string;

        typedef std::vector<char> blob;

        typedef std::filesystem::path address;

        typedef std::int64_t time;

        typedef std::variant<
                int32,
                float32,
                string,
                blob> argument;

        typedef std::vector<argument> arguments;

        typedef std::variant<bundle, message> packet;

        typedef std::vector<packet> packets;

        class message {
                private:
                        types::address address;
                        types::arguments arguments;

                public:
                        message(types::address path);

                        template<class T>
                        void push(T && val);
        };

        class bundle {
                private:
                        types::time time;
                        types::packets contents;
                public:
                        bundle(types::time time);

                        template<class T>
                        void push(T && val);
        };
}
