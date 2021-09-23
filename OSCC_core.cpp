#include "OSCC_core.hpp"


namespace oscc::core {

        constexpr char BUNDLE_IDENTIFIER[] = "#bundle";

        types::packet bytesToOSC(char* bytes, size_t size) {
                util::arrayConsumer<char> ac{bytes, size};
                return bytesToOSC(ac);
        }

        types::packet bytesToOSC(types::blob bytes) {
                return bytesToOSC(bytes.data(), bytes.size());
        }

        types::packet bytesToOSC(util::arrayConsumer<char> &bytes) {
                const types::int32 size = read::int32(bytes);
                util::arrayConsumer<char> data = bytes.reserve(size);

                if(data.peek() == BUNDLE_IDENTIFIER[0]) {
                        // Is OSC bundle
                        if(read::string(data) != BUNDLE_IDENTIFIER)
                                throw std::domain_error(types::string{"When a OSC packet address starts with '#' it must be \""} + BUNDLE_IDENTIFIER + "\" representing a bundle");
                        types::bundle bundle {read::time(data)};
                        while (!data.isEmpty())
                                bundle.push(bytesToOSC(data));
                        return types::packet{bundle};
                } else {
                        // Is OSC message
                        types::message message {types::address{read::string(data) }};

                        if(data.isEmpty()) {
                                // Type tag omitted, packet ended
                                return types::packet{message};
                        } else if(data.peek() == ',') {
                                // Type tag not omitted
                                const types::string typeTag {read::string(data)};
                                for(std::size_t i = 1; i < typeTag.length(); i++) {
                                        switch (typeTag[i]) {
                                                case 'i': message.push(read::int32(data)); break;
                                                case 'f': message.push(read::float32(data)); break;
                                                case 's': message.push(read::string(data)); break;
                                                case 'b': message.push(read::blob(data)); break;
                                        }
                                }
                                return types::packet{message};
                        } else {
                                // Type tag omitted, packet has additional data
                                throw std::domain_error("OSC message has no type tag, but contains arguments_");
                        }
                }
        }

        void OSCToBytes(const types::packet& data, std::vector<char> &bytes);

        std::vector<char> OSCToBytes(const types::packet& data) {
                std::vector<char> bytes{};
                bytes.reserve(100);
                OSCToBytes(data, bytes);
                return bytes;
        }

        void OSCToBytes(const types::packet& data, std::vector<char> &bytes) {
                using namespace std;
                // Reserve data for size
                const auto initialSize = bytes.size();
                bytes.insert(bytes.end(), sizeof(types::int32), '\0');

                // Write data
                if(holds_alternative<types::bundle>(data)) {
                        // Is bundle
                        const auto &bundle = get<types::bundle>(data);
                        write::string(BUNDLE_IDENTIFIER, bytes);
                        write::time(bundle.time(), bytes);
                        for(auto& content : bundle.contents()) {
                                OSCToBytes(content, bytes);
                        }
                } else {
                        // Is message
                        const auto &message = get<types::message>(data);
                        write::string(message.address().string(), bytes);
                        auto location = bytes.size()+1;
                        write::string(string(message.arguments().size()+1, ','), bytes);
                        for(const auto &argument : message.arguments()) {
                                visit([&](auto &&arg){
                                        using T = decay_t<decltype(arg)>;
                                        if constexpr(is_same_v<T, types::int32>) {
                                                bytes[location++] = 'i';
                                                write::int32(arg, bytes);
                                        } else if constexpr(is_same_v<T, types::float32>) {
                                                bytes[location++] = 'f';
                                                write::float32(arg, bytes);
                                        } else if constexpr(is_same_v<T, types::string>) {
                                                bytes[location++] = 's';
                                                write::string(arg, bytes);
                                        } else if constexpr(is_same_v<T, types::blob>) {
                                                bytes[location++] = 'b';
                                                write::blob(arg, bytes);
                                        } else throw domain_error("Unknown type for argument");
                                }, argument);
                        }
                }

                // Overwrite size data
                const types::int32 size = bytes.size() - initialSize - 4;
                memcpy(&*(bytes.begin()+initialSize), &size, sizeof(types::int32));
        }
}
