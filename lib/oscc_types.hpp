#pragma once

#include <cstdint>
#include <cmath>
#include <vector>
#include <string>
#include <filesystem>
#include <variant>
#include <functional>
#include <map>

namespace oscc::type {

        class message;

        class bundle;

        typedef std::int32_t int32;

        typedef std::uint32_t uint32;

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

        typedef std::function<void(packet)> call_back;

        class message {
                private:
                        type::address address_pattern_;
                        type::arguments arguments_;

                public:
                        explicit message(type::address path);
                        template<typename T> void push(T && val) { arguments_.push_back(std::forward<T>(val)); }
                        [[nodiscard]] type::address pattern() const;
                        [[nodiscard]] type::arguments arguments() const;
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
}