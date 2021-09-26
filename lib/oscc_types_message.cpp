#include "oscc_types.hpp"
#include "oscc_util.hpp"

namespace oscc::type {
        message::message(oscc::type::address path)
        : address_pattern_(std::move(path))
        {}

        type::address& message::pattern() {
                return address_pattern_;
        }

        type::arguments& message::arguments() {
                return arguments_;
        }

        void message::push(type::argument &arg) {
                arguments_.insert(arguments_.end(), arg);
        }

        char* to_hex(const char c, char* arr) {
                constexpr char hex[] = {'0', '1', '2', '3',
                                        '4', '5', '6', '7',
                                        '8', '9', 'A', 'B',
                                        'C', 'D', 'E', 'F'};
                arr[0] = hex[(c & 0xF0) >> 4];
                arr[1] = hex[c & 0x0F];
                return arr;
        }

        std::string arguments_string(const arguments &args) {
                std::string str;
                std::for_each(args.begin(),  args.end(), [&str](const auto &argument) {
                        std::visit([&str](const auto &arg){
                                using T = std::decay_t<decltype(arg)>;

                                if constexpr(std::is_same_v<T, int32>) {
                                        str += std::to_string(arg) + "i ";
                                } else if constexpr(std::is_same_v<T, float32>) {
                                        str += std::to_string(arg) + "f ";
                                } else if constexpr(std::is_same_v<T, string>) {
                                        str += "\"";
                                        for(const char &c : arg)
                                                if(c != '"') str += c;
                                                else str += "\\\"";
                                        str += "\" ";
                                } else if constexpr(std::is_same_v<T, blob>) {
                                        str += 'X';
                                        char b[] = "__";
                                        for(const char &c : arg) {
                                                str += to_hex(c, b);
                                        }
                                        str += ' ';
                        #ifdef OSCC_TYPES_VAL
                                } else if constexpr(std::is_same_v<T, value_argument>) {
                                        switch (arg) {
                                        #ifdef OSCC_TYPE_TF
                                                case value_argument::T: str += "T "; break;
                                                case value_argument::F: str += "F "; break;
                                        #endif
                                        #ifdef OSCC_TYPE_N
                                                case value_argument::N: str += "N "; break;
                                        #endif
                                        #ifdef OSCC_TYPE_I
                                                case value_argument::I: str += "I "; break;
                                        #endif
                                        }
                        #endif
                        #ifdef OSCC_TYPE_t
                                } else if constexpr(std::is_same_v<T, time>) {
                                        const auto cur_time = core::util::getCurrentTime().unix;
                                        str += ':';
                                        if(arg.unix <= cur_time) str += "0 ";
                                        else str += std::to_string(arg.unix - cur_time) + ' ';
                        #endif
                        #ifdef OSCC_TYPE_h
                                } else if constexpr(std::is_same_v<T, int64>) {
                                        str += std::to_string(arg) + "l ";
                        #endif
                        #ifdef OSCC_TYPE_d
                                } else if constexpr(std::is_same_v<T, float64>) {
                                        str += std::to_string(arg) + "d ";
                        #endif
                        #ifdef OSCC_TYPE_c
                                } else if constexpr(std::is_same_v<T, char>) {
                                        if(arg == '\'') str += "'\\''";
                                        else str = str + '\'' + arg + "' ";
                        #endif
                        #ifdef OSCC_TYPE_m
                                } else if constexpr(std::is_same_v<T, midi>) {
                                        char b[] = "__";
                                        str += str + '.';
                                        str += to_hex(arg.port_ID, b);
                                        str += to_hex(arg.status, b);
                                        str += to_hex(arg.data_1, b);
                                        str += to_hex(arg.data_2, b);
                                        str += ' ';
                        #endif
                        #ifdef OSCC_TYPE_r
                                } else if constexpr(std::is_same_v<T, rgba>) {
                                        char b[] = "__";
                                        str += '#';
                                        str += to_hex(arg.red, b);
                                        str += to_hex(arg.green, b);
                                        str += to_hex(arg.blue, b);
                                        str += to_hex(arg.alpha, b);
                                        str += ' ';
                        #endif
                        #ifdef OSCC_TYPE_S
                                } else if constexpr(std::is_same_v<T, symbol>) {
                                        str += "{";
                                        for(const char &c : arg.identifier)
                                                if(c != '}') str += c;
                                                else str += "\\}";
                                        str += "} ";
                        #endif
                        #ifdef OSCC_TYPE_ARR_
                                } else if constexpr(std::is_same_v<T, arguments>) {
                                        str += "[ " + arguments_string(arg) + "] ";
                        #endif
                                }
                        },argument);
                });
                return str;
        }

        std::string message::string() const {
                return address_pattern_.string() + " " + arguments_string(arguments_);
        }
}
