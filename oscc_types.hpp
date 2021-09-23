#pragma once

#include <cstdint>
#include <cmath>
#include <vector>
#include <string>
#include <filesystem>
#include <variant>

namespace oscc::types {

        class message;

        class bundle;

        typedef std::int32_t int32;

        typedef std::int64_t int64;

        typedef std::float_t float32;

        typedef std::string string;

        typedef std::vector<char> blob;

        typedef std::filesystem::path address;

        typedef int64 time;

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
                        types::address address_pattern_;
                        types::arguments arguments_;

                public:
                        explicit message(types::address path);
                        template<typename T> void push(T && val) { arguments_.push_back(std::forward<T>(val)); }
                        [[nodiscard]] types::address address() const;
                        [[nodiscard]] types::arguments arguments() const;
        };

        class bundle {
                private:
                        types::time time_;
                        types::packets contents_;

                public:
                        explicit bundle(types::time time);
                        template<typename T> void push(T && val) { contents_.push_back(std::forward<T>(val)); }
                        [[nodiscard]] types::time time() const;
                        [[nodiscard]] types::packets contents() const;
        };
}