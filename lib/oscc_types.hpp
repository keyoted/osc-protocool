#pragma once

#include <cstdint>
#include <cmath>
#include <vector>
#include <string>
#include <filesystem>
#include <variant>
#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include "oscc_util.hpp"

namespace oscc::types {

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
                        types::address address_pattern_;
                        types::arguments arguments_;

                public:
                        explicit message(types::address path);
                        template<typename T> void push(T && val) { arguments_.push_back(std::forward<T>(val)); }
                        [[nodiscard]] types::address pattern() const;
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

        class address_space {
                private:
                        std::unordered_map<std::string, call_back> functions_;
                        std::mutex mutex_;
                        const call_back& find(std::string pattern, const call_back& not_found);
                        void delayed_dispatch(bundle bdl, const call_back& not_found, int64 sleep);
                public:
                        void registerFunction(address adr, call_back cal);
                        void dispatch(packet pack, const call_back& not_found);
        };
}