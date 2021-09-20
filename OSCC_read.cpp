#include "OSCC_read.hpp"

namespace OSCC::read {
        types::int32 int32(arrayConsumer<char> &data) {
                types::int32 ret;
                std::memcpy(&ret, data.consume(sizeof(types::int32)), sizeof(types::int32));
                return ret;
        }

        types::float32 float32(arrayConsumer<char> &data) {
                types::float32 ret;
                std::memcpy(&ret, data.consume(sizeof(types::float32)), sizeof(types::float32));
                return ret;
        }

        types::time time(arrayConsumer<char> &data) {
                types::time ret;
                std::memcpy(&ret, data.consume(sizeof(types::time)), sizeof(types::time));
                return ret;
        }

        types::string string(arrayConsumer<char> &data) {
                types::string ret {data.consumeUntilOrThrow('\0')};
                data.consume(ret.length() % 4);
                return ret;
        }

        types::blob blob(arrayConsumer<char> &data) {
                types::int32 size = int32(data);
                auto dr = data.reserve(size);
                types::blob ret(dr.consume(0), dr.consume(0) + size);
                return ret;
        }
}