#include "oscc_read.hpp"

namespace oscc::core::read {
        // TODO: Handle systems with different endianness
        type::int32 int32(util::arrayConsumer<char>& data) {
                type::int32 ret;
                std::memcpy(&ret, data.consume(sizeof(type::int32)), sizeof(type::int32));
                return ret;
        }

        type::float32 float32(util::arrayConsumer<char>& data) {
                type::float32 ret;
                std::memcpy(&ret, data.consume(sizeof(type::float32)), sizeof(type::float32));
                return ret;
        }

        type::time time(util::arrayConsumer<char>& data) {
                type::time NTP;
                std::memcpy(&NTP, data.consume(sizeof(type::time)), sizeof(type::time));
                return util::NTPtoUNIX(NTP);
        }

        type::string string(util::arrayConsumer<char>& data) {
                type::string ret{data.consumeUntilOrThrow('\0')};
                data.consume(3 - ((ret.length() + (1 + 3)) % 4));
                return ret;
        }

        type::blob blob(util::arrayConsumer<char>& data) {
                type::int32 size = int32(data);
                auto        dr   = data.reserve(size + (3 - ((size + 3) % 4)));
                type::blob  ret(dr.consume(0), dr.consume(0) + size);
                return ret;
        }

        type::int64 int64(util::arrayConsumer<char>& data) {
                type::int64 ret;
                std::memcpy(&ret, data.consume(sizeof(type::int64)), sizeof(type::int64));
                return ret;
        }

#ifdef OSCC_TYPE_d
        type::float64 float64(util::arrayConsumer<char>& data) {
                type::float64 ret;
                std::memcpy(&ret, data.consume(sizeof(type::float64)), sizeof(type::float64));
                return ret;
        }
#endif

#ifdef OSCC_TYPE_m
        type::midi midi(util::arrayConsumer<char>& data) {
                type::midi ret;
                std::memcpy(&ret, data.consume(sizeof(type::midi)), sizeof(type::midi));
                return ret;
        }
#endif

#ifdef OSCC_TYPE_r
        type::rgba rgba(util::arrayConsumer<char>& data) {
                type::rgba ret;
                std::memcpy(&ret, data.consume(sizeof(type::rgba)), sizeof(type::rgba));
                return ret;
        }
#endif
}  // namespace oscc::core::read