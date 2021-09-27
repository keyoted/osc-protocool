#include "oscc_util.hpp"

namespace oscc::util {
        // TODO: clean-up
        // NOTE: maybe start thinking about something like bison
        /* format:
               bundle: #:123[...;...;...]
                       123 -- is milliseconds offset from now
                       ... -- is messages / other bundles
               message: /path 1 2.0 1i 2f 3l 4d "5" '6' {7} x0194 :123 #01020304 .01030507 [1[2]3]
                       /path -- is pattern
                       1 and 1i   -- is type::int32
                       2.0 and 2f -- is type::float32
                       3l         -- is type::int64
                       4d         -- is type::float64/double
                       "5"        -- is string
                       '6'        -- is asci character
                       {7}        -- is type::symbol
                       x0194      -- is type::blob
                       :123       -- is milliseconds offset from now
                       #01020304  -- is type::rgba color
                       .01030507  -- is type::midi data
                       [1[2]3]    -- is array with int, an array, and an int
        */

        type::packet strToOSC(const std::string& OSC_str) {
                auto pass_blob = [](const char*& c, unsigned max) -> type::blob {
                        type::blob    blb{};
                        unsigned char uc = 0;
                        while (true) {
                                switch (*c) {
                                        case '0': uc = 0x00; break;
                                        case '1': uc = 0x10; break;
                                        case '2': uc = 0x20; break;
                                        case '3': uc = 0x30; break;
                                        case '4': uc = 0x40; break;
                                        case '5': uc = 0x50; break;
                                        case '6': uc = 0x60; break;
                                        case '7': uc = 0x70; break;
                                        case '8': uc = 0x80; break;
                                        case '9': uc = 0x90; break;
                                        case 'A':
                                        case 'a': uc = 0xA0; break;
                                        case 'B':
                                        case 'b': uc = 0xB0; break;
                                        case 'C':
                                        case 'c': uc = 0xC0; break;
                                        case 'D':
                                        case 'd': uc = 0xD0; break;
                                        case 'E':
                                        case 'e': uc = 0xE0; break;
                                        case 'F':
                                        case 'f': uc = 0xF0; break;
                                        default: goto BRK2;
                                }
                                c++;
                                switch (*c) {
                                        case '0': uc |= 0x00; break;
                                        case '1': uc |= 0x01; break;
                                        case '2': uc |= 0x02; break;
                                        case '3': uc |= 0x03; break;
                                        case '4': uc |= 0x04; break;
                                        case '5': uc |= 0x05; break;
                                        case '6': uc |= 0x06; break;
                                        case '7': uc |= 0x07; break;
                                        case '8': uc |= 0x08; break;
                                        case '9': uc |= 0x09; break;
                                        case 'A':
                                        case 'a': uc |= 0x0A; break;
                                        case 'B':
                                        case 'b': uc |= 0x0B; break;
                                        case 'C':
                                        case 'c': uc |= 0x0C; break;
                                        case 'D':
                                        case 'd': uc |= 0x0D; break;
                                        case 'E':
                                        case 'e': uc |= 0x0E; break;
                                        case 'F':
                                        case 'f': uc |= 0x0F; break;
                                        default: {
                                                blb.push_back(*((char*)&uc));
                                                goto BRK2;
                                        }
                                }
                                c++;
                                blb.push_back(*((char*)&uc));
                                if (blb.size() == max) goto BRK2;
                        }
                BRK2:
                        return blb;
                };

                auto pass_number = [](const char*& c, const bool negative) -> type::argument {
                        enum mode {
                                i,
                                f,
#ifdef OSCC_TYPE_d
                                d,
#endif
#ifdef OSCC_TYPE_h
                                l,
#endif
                        };
                        mode        md    = i;
                        const char* start = c;
                        const char* end   = nullptr;
                        while (*c != '\0') switch (*c) {
                                        case '.': {
                                                if (md != i) throw std::domain_error("Multiple '.' in number.");
                                                else
                                                        md = f;
                                                c++;
                                        } break;

                                        case 'i':
                                        case 'I': {
                                                if (md == f)
                                                        throw std::domain_error("Float treated as int in OSC string");
                                                end = c;
                                                c++;
                                        }
                                                goto BRK;

                                        case 'f':
                                        case 'F': {
                                                md  = f;
                                                end = c;
                                                c++;
                                        }
                                                goto BRK;

#ifdef OSCC_TYPE_d
                                        case 'd':
                                        case 'D': {
                                                md  = d;
                                                end = c;
                                                c++;
                                        }
                                                goto BRK;
#endif

#ifdef OSCC_TYPE_h
                                        case 'l':
                                        case 'L': {
                                                if (md == f)
                                                        throw std::domain_error("Float treated as long in OSC string");
                                                md  = l;
                                                end = c;
                                                c++;
                                        }
                                                goto BRK;
#endif

                                        case '0':
                                        case '1':
                                        case '2':
                                        case '3':
                                        case '4':
                                        case '5':
                                        case '6':
                                        case '7':
                                        case '8':
                                        case '9': {
                                                c++;
                                        } break;

                                        default: end = c; goto BRK;
                                }

                BRK:
                        if (end == nullptr) end = c;
                        if (end == start) return 0;
                        const auto&& str = std::string(start, end - start);
                        switch (md) {
                                case i: {
                                        type::int32 v = std::stoi(str);
                                        return (negative) ? -v : v;
                                }

                                case f: {
                                        type::float32 v = std::stof(str);
                                        return (negative) ? -v : v;
                                }

#ifdef OSCC_TYPE_d
                                case d: {
                                        type::float64 v = std::stold(str);
                                        return (negative) ? -v : v;
                                }
#endif

#ifdef OSCC_TYPE_h
                                case l: {
                                        type::int64 v = std::stoll(str);
                                        return (negative) ? -v : v;
                                }
#endif
                                // NOTE: Not used but VSC complains about it...
                                default: return 0;
                        }
                };

                auto pass_argument = [&](const char*& c) -> type::argument {
                        switch (*c) {
                                case '"': {
                                        std::string ret;
                                        c++;
                                        while (*c != '\0') {
                                                if (*c == '"' && *(c - 1) != '\\') break;
                                                ret += *c;
                                                c++;
                                        }
                                        if (*c == '\0') throw std::domain_error("Missing '\"' in OSC string");
                                        c++;
                                        while (*c == ' ') c++;
                                        return ret;
                                }

                                case '\'': {
                                        c++;
                                        char r = *c;
                                        if (*c == '\\') {
                                                c++;
                                                r = *c;
                                        }
                                        if (*(c + 1) != '\'') throw std::domain_error("Missing \"'\" in OSC string");
                                        c += 2;
                                        while (*c == ' ') c++;
                                        return r;
                                }

                                case '[': throw std::domain_error("Illegal '['");
                                case '}':
                                case ']': throw std::domain_error("Illegal ']' or '}'");

                                case '-': {
                                        c++;
                                        return pass_number(c, true);
                                }

                                case 'x':
                                case 'X': {
                                        c++;
                                        return pass_blob(c, -1);
                                }

                                case '0':
                                case '1':
                                case '2':
                                case '3':
                                case '4':
                                case '5':
                                case '6':
                                case '7':
                                case '8':
                                case '9': return pass_number(c, false);

#ifdef OSCC_TYPE_m
                                case '.': {
                                        c++;
                                        type::blob blb = pass_blob(c, 4);
                                        if (blb.size() != 4)
                                                throw std::domain_error("MIDI data bust have 8 hex letters");
                                        return type::midi{(unsigned char)blb[0], (unsigned char)blb[1],
                                                          (unsigned char)blb[2], (unsigned char)blb[3]};
                                }
#endif

#ifdef OSCC_TYPE_r
                                case '#': {
                                        c++;
                                        type::blob blb = pass_blob(c, 4);
                                        if (blb.size() != 4)
                                                throw std::domain_error("RGBA data bust have 8 hex letters");
                                        return type::rgba{(unsigned char)blb[0], (unsigned char)blb[1],
                                                          (unsigned char)blb[2], (unsigned char)blb[3]};
                                }
#endif

#ifdef OSCC_TYPE_TF
                                case 't':
                                case 'T': return type::T;
                                case 'f':
                                case 'F': return type::F;
#endif

#ifdef OSCC_TYPE_I
                                case 'i':
                                case 'I': return type::I;
#endif

#ifdef OSCC_TYPE_N
                                case 'n':
                                case 'N': return type::N;
#endif

#ifdef OSCC_TYPE_t
                                case ':': {
                                        c++;
                                        type::int64 offset;
                                        const auto& num = pass_number(c, false);

                                        if (const type::int32* v = std::get_if<type::int32>(&num)) {
                                                offset = *v;
                                        } else if (const type::int64* v = std::get_if<type::int64>(&num)) {
                                                offset = *v;
                                        } else if (const type::float32* v = std::get_if<type::float32>(&num)) {
                                                offset = *v;
                                        } else if (const type::float64* v = std::get_if<type::float64>(&num)) {
                                                offset = *v;
                                        } else {
                                                throw std::domain_error("Unknown error.");
                                        }

                                        const type::time milliseconds_since_epoch = util::getCurrentTime();

                                        return type::time{milliseconds_since_epoch.unix + offset};
                                }
#endif

#ifdef OSCC_TYPE_S
                                case '{': {
                                        type::symbol ret{""};
                                        c++;
                                        while (*c != '\0') {
                                                if (*c == '}' && *(c - 1) != '\\') break;
                                                ret.identifier += *c;
                                                c++;
                                        }
                                        if (*c == '\0') throw std::domain_error("Missing '}' in OSC string");
                                        c++;
                                        while (*c == ' ') c++;
                                        return type::symbol{ret};
                                }
#endif
                                default:
                                        throw std::domain_error(std::string("Error on OSC string at char [") + (*c) +
                                                                "]");
                        }
                };

                const char* c = &OSC_str[0];
                while (*c == ' ') c++;
                if (*c == '#') {
                        // We are on a bundle
                        c++;
                        type::int64 offset;
                        const auto& num = pass_number(c, false);

                        if (const type::int32* v = std::get_if<type::int32>(&num)) {
                                offset = *v;
#ifdef OSCC_TYPE_h
                        } else if (const type::int64* v = std::get_if<type::int64>(&num)) {
                                offset = *v;
#endif
                        } else if (const type::float32* v = std::get_if<type::float32>(&num)) {
                                offset = *v;
#ifdef OSCC_TYPE_d
                        } else if (const type::float64* v = std::get_if<type::float64>(&num)) {
                                offset = *v;
#endif
                        } else {
                                throw std::domain_error("Unknown error.");
                        }
                        const type::int64 milliseconds_since_epoch =
                                std::chrono::duration_cast<std::chrono::milliseconds>(
                                        std::chrono::system_clock::now().time_since_epoch())
                                        .count();

                        type::bundle bdl{type::time{milliseconds_since_epoch + offset}};
                        while (*c == ' ') c++;
                        if (*c == '\0') {
                                return bdl;
                        } else if (*c == '[') {
                                c++;
                                int         indent = 1;
                                std::string str;
                                while (*c != '\0' && indent != 0) {
                                        if (*c == '[') {
                                                indent++;
                                                str += *c;
                                        } else if (*c == ']') {
                                                indent--;
                                                if (indent > 0) str += *c;
                                        } else if (*c == ';' && indent == 1) {
                                                bdl.push(strToOSC(str));
                                                str = "";
                                        } else {
                                                str += *c;
                                        }
                                        c++;
                                }
                                if (indent != 0) throw std::domain_error("Missing ']' for bundle in OSC string");
                                if (!str.empty()) {
                                        bdl.push(strToOSC(str));
                                        str = "";
                                }
                                return bdl;
                        } else {
                                throw std::domain_error("Unknown error in bundle OSC string");
                        }
                } else if (*c == '/') {
                        std::string pat;
                        while (*c != '\0' && *c != ' ') {
                                pat += *c;
                                c++;
                        }
                        if (!isValidPattern(pat)) throw std::domain_error("Invalid pattern: " + pat);
                        type::message message{pat};
                        while (*c == ' ') c++;
#ifndef OSCC_TYPE_ARR_
                        while (*c != '\0') {
                                message.push(pass_argument(c));
                                while (*c == ' ') c++;
                        }
#else
                        std::vector<type::arguments*> stack;
                        stack.push_back(&message.arguments());
                        while (*c != '\0') {
                                if (*c == '[') {
                                        stack.back()->push_back(type::arguments{});
                                        stack.push_back(&std::get<type::arguments>(stack.back()->back()));
                                        c++;
                                } else if (*c == ']') {
                                        stack.pop_back();
                                        if (stack.empty()) throw std::domain_error("Unexpected ']' in OSC string");
                                        c++;
                                } else {
                                        stack.back()->push_back(pass_argument(c));
                                }
                                while (*c == ' ') c++;
                        }
                        if (stack.size() != 1) throw std::domain_error("Missing ] in OSC string");
#endif
                        return message;
                } else {
                        throw std::domain_error("OSC message should start with '/' or '#'");
                }
        }

        std::string string(const type::bundle& bdl) {
                std::string str{"#"};

                const auto  cur_time = util::getCurrentTime();
                if (bdl.time().unix <= cur_time.unix) str += '0';
                else
                        str += std::to_string(bdl.time().unix - cur_time.unix);

                str += '[';
                std::for_each(bdl.contents().begin(), bdl.contents().end(),
                              [&str](const auto& packet) { str += packet.string() + "; "; });
                if (bdl.contents().empty()) str += "] ";
                else
                        *(str.end() - 2) = ']';

                return str;
        }

        /**
         * Fill the first two character of an array with the hexadecimal
         * representation of a byte.
         * @param c The byte to convert to hexadecimal.
         * @param arr The array where to write the conversion to.
         * @return The array passed as an imput.
         */
        char* to_hex(const char c, char* arr) {
                constexpr char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
                arr[0]               = hex[(c & 0xF0) >> 4];
                arr[1]               = hex[c & 0x0F];
                return arr;
        }

        std::string string(const type::arguments& args) {
                std::string str;
                std::for_each(args.begin(), args.end(), [&str](const auto& argument) {
                        std::visit(
                                [&str](const auto& arg) {
                                        using T = std::decay_t<decltype(arg)>;

                                        if constexpr (std::is_same_v<T, type::int32>) {
                                                str += std::to_string(arg) + "i ";
                                        } else if constexpr (std::is_same_v<T, type::float32>) {
                                                str += std::to_string(arg) + "f ";
                                        } else if constexpr (std::is_same_v<T, type::string>) {
                                                str += "\"";
                                                for (const char& c : arg)
                                                        if (c != '"') str += c;
                                                        else
                                                                str += "\\\"";
                                                str += "\" ";
                                        } else if constexpr (std::is_same_v<T, type::blob>) {
                                                str += 'X';
                                                char b[] = "__";
                                                for (const char& c : arg) { str += to_hex(c, b); }
                                                str += ' ';
#ifdef OSCC_TYPES_VAL
                                        } else if constexpr (std::is_same_v<T, type::value_argument>) {
                                                switch (arg) {
        #ifdef OSCC_TYPE_TF
                                                        case type::value_argument::T: str += "T "; break;
                                                        case type::value_argument::F: str += "F "; break;
        #endif
        #ifdef OSCC_TYPE_N
                                                        case type::value_argument::N: str += "N "; break;
        #endif
        #ifdef OSCC_TYPE_I
                                                        case type::value_argument::I: str += "I "; break;
        #endif
                                                }
#endif
#ifdef OSCC_TYPE_t
                                        } else if constexpr (std::is_same_v<T, type::time>) {
                                                const auto cur_time = util::getCurrentTime().unix;
                                                str += ':';
                                                if (arg.unix <= cur_time) str += "0 ";
                                                else
                                                        str += std::to_string(arg.unix - cur_time) + ' ';
#endif
#ifdef OSCC_TYPE_h
                                        } else if constexpr (std::is_same_v<T, type::int64>) {
                                                str += std::to_string(arg) + "l ";
#endif
#ifdef OSCC_TYPE_d
                                        } else if constexpr (std::is_same_v<T, type::float64>) {
                                                str += std::to_string(arg) + "d ";
#endif
#ifdef OSCC_TYPE_c
                                        } else if constexpr (std::is_same_v<T, char>) {
                                                if (arg == '\'') str += "'\\''";
                                                else
                                                        str = str + '\'' + arg + "' ";
#endif
#ifdef OSCC_TYPE_m
                                        } else if constexpr (std::is_same_v<T, type::midi>) {
                                                char b[] = "__";
                                                str += str + '.';
                                                str += to_hex(arg.port_ID, b);
                                                str += to_hex(arg.status, b);
                                                str += to_hex(arg.data_1, b);
                                                str += to_hex(arg.data_2, b);
                                                str += ' ';
#endif
#ifdef OSCC_TYPE_r
                                        } else if constexpr (std::is_same_v<T, type::rgba>) {
                                                char b[] = "__";
                                                str += '#';
                                                str += to_hex(arg.red, b);
                                                str += to_hex(arg.green, b);
                                                str += to_hex(arg.blue, b);
                                                str += to_hex(arg.alpha, b);
                                                str += ' ';
#endif
#ifdef OSCC_TYPE_S
                                        } else if constexpr (std::is_same_v<T, type::symbol>) {
                                                str += "{";
                                                for (const char c : arg.identifier)
                                                        if (c != '}') str += c;
                                                        else
                                                                str += "\\}";
                                                str += "} ";
#endif
#ifdef OSCC_TYPE_ARR_
                                        } else if constexpr (std::is_same_v<T, type::arguments>) {
                                                str += "[ " + string(arg) + "] ";
#endif
                                        }
                                },
                                argument);
                });
                return str;
        }

        std::string string(const type::message& msg) { return msg.pattern().string() + " " + string(msg.arguments()); }

        std::string string(const type::packet& pkt) {
                if (std::holds_alternative<type::bundle>(pkt)) {
                        return std::get<type::bundle>(pkt).string();
                } else if (std::holds_alternative<type::message>(pkt)) {
                        return std::get<type::message>(pkt).string();
                } else
                        return "";
        }

}  // namespace oscc::util
