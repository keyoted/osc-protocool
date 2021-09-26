#include "oscc_core.hpp"


namespace oscc::core {

        constexpr char BUNDLE_IDENTIFIER[] = "#bundle";

        type::packet bytesToOSC(char* bytes, size_t size) {
                util::arrayConsumer<char> ac{bytes, size};
                return bytesToOSC(ac);
        }

        type::packet bytesToOSC(type::blob bytes) {
                return bytesToOSC(bytes.data(), bytes.size());
        }

        type::packet bytesToOSC(util::arrayConsumer<char> &bytes) {
                const type::int32 size = read::int32(bytes);
                util::arrayConsumer<char> data = bytes.reserve(size);

                if(data.peek() == BUNDLE_IDENTIFIER[0]) {
                        // Is OSC bundle
                        if(read::string(data) != BUNDLE_IDENTIFIER)
                                throw std::domain_error(type::string{"When a OSC packet address starts with '#' it must be \""} + BUNDLE_IDENTIFIER + "\" representing a bundle");
                        type::bundle bundle {read::time(data)};
                        while (!data.isEmpty())
                                bundle.push(bytesToOSC(data));
                        return type::packet{bundle};
                } else {
                        // Is OSC message
                        type::message message {type::address{read::string(data) }};
                        #ifndef OSCC_TYPE_ARR_
                                #define push message.push
                        #else
                                std::vector<type::arguments*> stack{};
                                stack.emplace_back(&message.arguments());
                                #define push(E) stack.back()->emplace_back(E)
                        #endif
                        if(data.isEmpty()) {
                                // Type tag omitted, packet ended
                                return type::packet{message};
                        } else if(data.peek() == ',') {
                                // Type tag not omitted
                                const type::string typeTag {read::string(data)};
                                for(std::size_t i = 1; i < typeTag.length(); i++) {
                                        switch (typeTag[i]) {
                                                case 'i': push(read::int32(data)); break;
                                                case 'f': push(read::float32(data)); break;
                                                case 's': push(read::string(data)); break;
                                                case 'b': push(read::blob(data)); break;
                                                #ifdef OSCC_TYPE_TF
                                                        case 'T': push(type::T); break;
                                                        case 'F': push(type::F); break;
                                                #endif
                                                #ifdef OSCC_TYPE_I
                                                        case 'I': push(type::I); break;
                                                #endif
                                                #ifdef OSCC_TYPE_N
                                                        case 'N': push(type::N); break;
                                                #endif
                                                #ifdef OSCC_TYPE_t
                                                        case 't': push(type::argument(read::time(data))); break;
                                                #endif
                                                #ifdef OSCC_TYPE_h
                                                        case 'h': push(read::int64(data)); break;
                                                #endif
                                                #ifdef OSCC_TYPE_d
                                                        case 'd': push(read::float64(data)); break;
                                                #endif
                                                #ifdef OSCC_TYPE_c
                                                        case 'c': push((char) read::int32(data)); break;
                                                #endif
                                                #ifdef OSCC_TYPE_m
                                                        case 'm': push(read::midi(data)); break;
                                                #endif
                                                #ifdef OSCC_TYPE_r
                                                        case 'r': push(read::rgba(data)); break;
                                                #endif
                                                #ifdef OSCC_TYPE_S
                                                        case 'S': push(type::symbol{read::string(data)}); break;
                                                #endif
                                                #ifdef OSCC_TYPE_ARR_
                                                        case '[': {
                                                                push(type::arguments{});
                                                                stack.emplace_back(&std::get<type::arguments>(stack.back()->back()));
                                                        } break;

                                                        case ']': {
                                                                stack.pop_back();
                                                        } break;
                                                #endif
                                                default:
                                                        throw std::domain_error(std::string("Unrecognized tag: [") + typeTag[i] + "]");
                                        }
                                }
                                return type::packet{message};
                        } else {
                                // Type tag omitted, packet has additional data
                                throw std::domain_error("OSC message has no type tag, but contains arguments_");
                        }
                }
        }

        void OSCToBytes(const type::packet& data, std::vector<char> &bytes);

        std::vector<char> OSCToBytes(const type::packet& data) {
                std::vector<char> bytes{};
                bytes.reserve(100);
                OSCToBytes(data, bytes);
                return bytes;
        }

        void OSCToBytes(const type::packet& data, std::vector<char> &bytes) {
                using namespace std;
                // Reserve data for size
                const auto initialSize = bytes.size();
                bytes.insert(bytes.end(), sizeof(type::int32), '\0');

                // Write data
                if(holds_alternative<type::bundle>(data)) {
                        // Is bundle
                        const auto &bundle = get<type::bundle>(data);
                        write::string(BUNDLE_IDENTIFIER, bytes);
                        write::time(bundle.time(), bytes);
                        for(auto& content : bundle.contents()) {
                                OSCToBytes(content, bytes);
                        }
                } else {
                        // Is message
                        auto message = get<type::message>(data);
                        write::string(message.pattern().string(), bytes);
                        auto location = bytes.size()+1;
                        #ifndef OSCC_TYPE_ARR_
                                const auto count = message.arguments().size()+1;
                        #else
                                std::vector<const type::argument*> stack{};
                                auto count = 0;
                                for(const auto& a : message.arguments()) stack.push_back(&a);
                                while (!stack.empty()) {
                                        const type::argument * const b = stack.back();
                                        stack.pop_back();
                                        if(!holds_alternative<type::arguments>(*b)) {
                                                count++;
                                        } else {
                                                count+=2;
                                                for(const auto& a : get<type::arguments>(*b)) stack.push_back(&a);
                                        }
                                }
                        #endif
                        write::string(string(count, ','), bytes);
                        #ifndef OSCC_TYPE_ARR_
                                for(const auto &argument : message.arguments()) {
                        #else
                                stack = vector<const type::argument*>{};
                                for(std::size_t j = message.arguments().size()-1; j+1 != 0; j--) {
                                        stack.emplace_back(&message.arguments()[j]);
                                }
                                while (!stack.empty()) {
                                        if(stack.back() == nullptr) {
                                                stack.pop_back();
                                                bytes[location++] = ']';
                                                continue;
                                        }
                                        const auto &argument = *stack.back();
                                        stack.pop_back();
                        #endif
                                        visit([&](auto &&arg){
                                                using T = decay_t<decltype(arg)>;
                                                if constexpr(is_same_v<T, type::int32>) {
                                                        bytes[location++] = 'i';
                                                        write::int32(arg, bytes);
                                                } else if constexpr(is_same_v<T, type::float32>) {
                                                        bytes[location++] = 'f';
                                                        write::float32(arg, bytes);
                                                } else if constexpr(is_same_v<T, type::string>) {
                                                        bytes[location++] = 's';
                                                        write::string(arg, bytes);
                                                } else if constexpr(is_same_v<T, type::blob>) {
                                                        bytes[location++] = 'b';
                                                        write::blob(arg, bytes);
                                                #ifdef OSCC_TYPES_VAL
                                                        } else if constexpr(is_same_v<T, type::value_argument>) {
                                                                switch (arg) {
                                                                        #ifdef OSCC_TYPE_TF
                                                                                case type::T: bytes[location++] = 'T'; break;
                                                                                case type::F: bytes[location++] = 'F'; break;
                                                                        #endif
                                                                        #ifdef OSCC_TYPE_N
                                                                                case type::N: bytes[location++] = 'N'; break;
                                                                        #endif
                                                                        #ifdef OSCC_TYPE_I
                                                                                case type::I: bytes[location++] = 'I'; break;
                                                                        #endif
                                                                }
                                                #endif
                                                #ifdef OSCC_TYPE_t
                                                        } else if constexpr(is_same_v<T, type::time>) {
                                                                bytes[location++] = 't';
                                                                write::time(arg, bytes);
                                                #endif
                                                #ifdef OSCC_TYPE_h
                                                        } else if constexpr(is_same_v<T, type::int64>) {
                                                                bytes[location++] = 'h';
                                                                write::int64(arg, bytes);
                                                #endif
                                                #ifdef OSCC_TYPE_d
                                                        } else if constexpr(is_same_v<T, type::float64>) {
                                                                bytes[location++] = 'd';
                                                                write::float64(arg, bytes);
                                                #endif
                                                #ifdef OSCC_TYPE_c
                                                        } else if constexpr(is_same_v<T, char>) {
                                                                bytes[location++] = 'c';
                                                                write::int32((type::int32) arg, bytes);
                                                #endif
                                                #ifdef OSCC_TYPE_m
                                                        } else if constexpr(is_same_v<T, type::midi>) {
                                                                bytes[location++] = 'm';
                                                                write::midi(arg, bytes);
                                                #endif
                                                #ifdef OSCC_TYPE_m
                                                        } else if constexpr(is_same_v<T, type::rgba>) {
                                                                bytes[location++] = 'r';
                                                                write::rgba(arg, bytes);
                                                #endif
                                                #ifdef OSCC_TYPE_S
                                                        } else if constexpr(is_same_v<T, type::symbol>) {
                                                                bytes[location++] = 'S';
                                                                write::string(arg.identifier, bytes);
                                                #endif
                                                #ifdef OSCC_TYPE_ARR_
                                                        } else if constexpr(is_same_v<T, type::arguments>) {
                                                                bytes[location++] = '[';
                                                                stack.push_back(nullptr);
                                                                for(std::size_t j = arg.size()-1; j+1 != 0; j--) {
                                                                        stack.emplace_back(&arg[j]);
                                                                }
                                                #endif
                                                } else throw domain_error("Unknown type for argument");
                                        }, argument);
                                }
                }

                // Overwrite size data
                const type::int32 size = bytes.size() - initialSize - 4;
                memcpy(&*(bytes.begin()+initialSize), &size, sizeof(type::int32));
        }
}
