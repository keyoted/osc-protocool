#include "oscc_read.hpp"
// TODO: Handle systems with different endianness

namespace oscc::util::read {

        /**
         * A helper templated function to read most types.
         * @tparam T The type of data to read.
         * @param data The byte array where to read the data from.
         * @return The data read from the byte array as a type T.
         */
        template <typename T>
        T trivial_read(util::arrayConsumer<char>& data) {
                T ret{};
                std::memcpy(&ret, data.consume(sizeof(T)), sizeof(T));
                return ret;
        }

        type::int32   int32(util::arrayConsumer<char>& data) { return trivial_read<type::int32>(data); }

        type::float32 float32(util::arrayConsumer<char>& data) { return trivial_read<type::float32>(data); }

        type::time    time(util::arrayConsumer<char>& data) { return trivial_read<type::time>(data); }

        type::string  string(util::arrayConsumer<char>& data) {
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

        type::int64 int64(util::arrayConsumer<char>& data) { return trivial_read<type::int64>(data); }

#ifdef OSCC_TYPE_d
        type::float64 float64(util::arrayConsumer<char>& data) { return trivial_read<type::float64>(data); }
#endif

#ifdef OSCC_TYPE_m
        type::midi midi(util::arrayConsumer<char>& data) { return trivial_read<type::midi>(data); }
#endif

#ifdef OSCC_TYPE_r
        type::rgba rgba(util::arrayConsumer<char>& data) { return trivial_read<type::rgba>(data); }
#endif
}  // namespace oscc::util::read