#include "oscc_util.hpp"

namespace oscc::core::util {

#define EPOCH_TIME_DIFFERENCE_S (2208988800LL)
#define EPOCH_TIME_DIFFERENCE_MS (EPOCH_TIME_DIFFERENCE_S * 1000LL)

        /**
         * Converts NTP time to UNIX time.
         * @param NTP The absolute time in NTP format.
         * @return The time in UNIX format (milliseconds since 1970/1/1 00:00 +0).
         */
        oscc::type::time NTPtoUNIX(const oscc::type::time &NTP) {
                if ((NTP.ntp & 1U) == 1) return type::time{0}; // Least significant bit means now
                else {
                        const auto seconds = ((oscc::type::uint32*)&NTP.ntp) [0];
                        const auto fraction = ((oscc::type::uint32*)&NTP.ntp) [1];
                        const auto millis = (seconds * 1000) + ((fraction * 1000) / 0x100000000L);
                        // There is a constant offset of seconds between 1900/1/1 UTC and 1970/1/1 UTC
                        return type::time{millis + EPOCH_TIME_DIFFERENCE_MS};
                }
        }

        /**
         * Converts UNIX time to NTP time.
         * @param UNIX The time in UNIX format (milliseconds since 1970/1/1 00:00 +0).
         * @return The time in NTP format.
         */
        oscc::type::time UNIXtoNTP(const oscc::type::time &UNIX) {
                // There is a constant offset of seconds between 1900/1/1 UTC and 1970/1/1 UTC
                const auto millis = UNIX.unix - EPOCH_TIME_DIFFERENCE_MS;
                const auto secconds = (type::uint32) (millis / 1000);
                const auto fraction = (type::uint32) (((millis % 1000) * 0x100000000L) / 1000);
                const oscc::type::uint32 ret[2] {secconds, fraction};
                return *((oscc::type::time*)ret);
        }


        /**
         * Tries to match at least one character from addr to the pattern (patrn).
         *
         * May match multiple characters from addr if patrn contains a list of words.
         * May match multiple characters from pattrn if pattrn contains a list of characters.
         *
         * If the return mills is false addr and ptrn will remain unchanged.
         * If the return mills is true addr and ptrn will point to the next character in the sequence that was not used
         * for matching.
         *
         * @param addr A pointer to the char array containing the address string to match.
         * @param ptrn A pointer to the char array containing the OSC address to match.
         * @return True iff at least one character from addr could be matched.
         */
        bool char_match(const char* &addr, const char* &ptrn) {
                if((*addr == '\0') ^ (*ptrn == '\0')) { // addr and ptrn are not empty or are bath empty
                        return false;
                } else if (*addr == *ptrn || (*ptrn == '?' && *addr != '/')) { // addr == ptrn or ptrn matches any character (besides '/')
                        if(*addr != '\0') { // increment pointers only if both string are not empty
                                addr++;
                                ptrn++;
                        }
                        return true;
                } else if(*ptrn == '[') { // handle list of characters
                        ptrn++; // point to first char in list
                        if(*ptrn == '\0') { // list at end of string
                                ptrn--;
                                return false;
                        } else if(*ptrn == ']') { // empty list "[]" UB, decided to ignore
                                ptrn++;
                                const bool ret = char_match(addr, ptrn);
                                if(!ret) ptrn-= 2;
                                return ret;
                        } else {
                                bool match = true; // the mills that will be returned when a match happens
                                const char* const pr = ptrn;
                                if(*ptrn == '!') { // if first char is '!' then we return true iff the char in addr is NOT in the list
                                        match = false;
                                        ptrn++;
                                }
                                while (*ptrn != ']' && *ptrn != '\0') { // iterate over the character list
                                        if(                                             // handle '-' sign (eg. "[a-zA-Z]")
                                                *ptrn == '-' &&                            // current char is '-'
                                                *(ptrn + 1) != ']' &&                        // '-' at the end of list has no special meaning
                                                *(ptrn - 1) != '[' &&                        // '-' at beginning of list is UB, decided no special meaning
                                                !(*(ptrn - 1) == '!' && *(ptrn - 2) == '[')       // '-' after [! is UB, decided no special meaning
                                        ) {
                                                if(*addr >= *(ptrn - 1) && *addr <= *(ptrn + 1)) {      // if the number is in range
                                                        if(match) {
                                                                addr++;                    // if matched increment addr and return
                                                                while (*(ptrn - 1) != ']')
                                                                        ptrn++;
                                                        } else ptrn = pr;
                                                        return match;
                                                } else {                                // no match, increment pattern
                                                        ptrn+=2;
                                                }
                                        } else if(*addr == *ptrn) {                           // addr is a char in the pattern
                                                while (*ptrn != ']' && *ptrn != '\0') ptrn++;    // increment ptrn until ']'
                                                if(*ptrn == '\0') {                        // UB, no closing ']'
                                                        ptrn = pr;
                                                        return false;
                                                }
                                                ptrn++;                                    // skip ']'
                                                if(match) addr++;                          // if matched increment addr and return
                                                else ptrn = pr;
                                                return match;
                                        } else {
                                                ptrn++;                                    // no match, move to next char
                                        }
                                }
                                if(*ptrn == '\0') { // UB, no closing ']'
                                        ptrn = pr;
                                        return false;
                                }
                                if(!match) {
                                        addr++;                                            // if matched increment addr and return
                                        ptrn++;
                                } else ptrn = pr;
                                return !match;
                        }
                } else if (*ptrn == '{') { // handle list of words (eg. {a,word,another,one})
                        const char* beginOfMatch = addr; // save original position of addr
                        const char* const pr = ptrn;
                        ptrn++; // go to first char in list
                        if(*ptrn == '\0') { // UB no closing '}'
                                ptrn = pr;
                                return false;
                        } else if(*ptrn == '}') { // UB empty list "{}", decided to ignore
                                ptrn++;
                                const bool ret = char_match(addr, ptrn);
                                if(!ret) ptrn = pr;
                                return ret;
                        } else {
                                while (*ptrn != ',' && *ptrn != '}' && *ptrn != '\0') { // iterate until end of a word
                                        if(*ptrn != *addr) { // if characters don't match
                                                addr = beginOfMatch; // reset addr
                                                while (true) { // increment +
                                                        ptrn++;
                                                        if(*ptrn == '\0') { // until we hit UB -- no closing '}'
                                                                ptrn = pr;
                                                                return false;
                                                        } else if(*ptrn == '}') { // until we finish this word and there are no other words
                                                                ptrn = pr;
                                                                return false;
                                                        } else if(*ptrn == ',') { // until we finish this word and move on to the next one
                                                                ptrn++;
                                                                break;
                                                        }
                                                }
                                        } else { // there was a match, move on
                                                addr++;
                                                ptrn++;
                                        }
                                }
                                if(
                                        *ptrn == '\0' ||           // UB -- no closing '}'
                                        *(ptrn - 1) == ',' ||        // UB -- empty last word ...",}" or empty word "{"...",,"..."}"
                                        *(ptrn - 1) == '{'           // UB -- empty list "{}"
                                ) {
                                        addr = beginOfMatch;       // reset addr and return
                                        ptrn = pr;
                                        return false;
                                }
                                // return
                                while (*ptrn != '\0' && *(ptrn - 1) != '}')
                                        ptrn++;
                                if(*ptrn == '\0') { // UB -- no closing '}'
                                        addr = beginOfMatch;       // reset addr and return
                                        ptrn = pr;
                                        return false;
                                }
                                return true;
                        }
                } else { // no match
                        return false;
                }
        }

        /**
         * Determines if an address matches a certain pattern.
         * @param address The address to match.
         * @param pattern The pattern to use for the matching.
         * @return True iff the address matches the pattern.
         */
        bool isMatch(const type::string address, const type::string pattern) {
                // TODO: find a way to do this without recursion.
                std::function<bool(const char*, const char*)> handle_wildcard;
                handle_wildcard = [&handle_wildcard](const char* a, const char* p) -> bool {
                        if((*a == '\0') ^ (*p == '\0')) return false;
                        while (char_match(a, p) && *a != '\0' && *p != '\0') continue;
                        if(*a == '\0' || *p == '\0') { // reached end of address/pattern
                                return *a == *p;
                        } else if(*p == '*') {
                                while (*(p+1) == '*') p++;      // Ignore multiple '*' in a row
                                if(*(p+1) == '/') {             // '*' is last in its segment
                                        p+=2;
                                        while (*a != '/')       // read until segment is over
                                                a++;
                                        a++;
                                        return handle_wildcard(a, p);
                                } else if(*(p+1) == '\0') {     // '*' is last character
                                        p+=1;
                                        while (*a != '\0')      // read until address is over
                                                a++;
                                        a++;
                                        return true;
                                }
                                // Two options;
                                // 1) ignore
                                p++;
                                if(handle_wildcard(a, p)) return true;                             // always try to ignore first
                                // 2) match a character
                                else {                                                                  // then try to match
                                        p--;
                                        a++;
                                        return handle_wildcard(a, p);
                                }
                        } else return false;
                };
                const char* addr = address.c_str();
                const char* patt = pattern.c_str();
                return handle_wildcard(addr, patt);
        }

        bool isValidAddress(std::string address) {
                if(address[0] != '/') return false;
                return std::all_of(address.begin(), address.end(), [](char c) {
                        return isprint(c) && c != ' ' && c != '#' && c != '*' &&
                                c != ',' && c != '?' && c != '[' && c != ']' &&
                                c != '{' && c != '}';

                });
        }

        bool isValidPattern(std::string pattern) {
                enum validate_mode_{
                        normal,
                        in_brackets,
                        in_list
                };

                if(pattern[0] != '/') return false;
                validate_mode_ mode = normal;
                for(const auto& c : pattern) {
                        switch (mode) {
                                case normal: {
                                        if(!std::isprint(c) || c==' ' || c=='#' || c==',' || c==']' || c=='}') return false;
                                        else if(c=='[') mode = in_brackets;
                                        else if(c=='{') mode = in_list;
                                } break;

                                case in_brackets: {
                                        if(!std::isprint(c) || c==' ' || c=='#' || c=='*' || c==',' || c=='/' || c=='?' || c=='[' || c=='{' || c=='}') return false;
                                        else if (c==']') mode = normal;
                                } break;

                                case in_list: {
                                        if(!std::isprint(c) || c==' ' || c=='#' || c=='*' || c=='/' || c=='?' || c=='{' || c=='[' || c==']') return false;
                                        else if (c=='}') mode = normal;
                                } break;
                        }
                }
                return true;
        }

         // TODO: clean-up
         /* format:
                bundle: #:123[...;...;...]
                        123 -- is milliseconds offset from now
                        ... -- is messages / other bundles
                message: /path 1 2.0 1i 2f 3l 4d "5" '6' {7} x0194 :123 #01020304 .01030507 [1[2]3]
                        /path -- is pattern
                        1 and 1i   -- is int32
                        2.0 and 2f -- is float32
                        3l         -- is int64
                        4d         -- is float64/double
                        "5"        -- is string
                        '6'        -- is asci character
                        {7}        -- is symbol
                        x0194      -- is blob
                        :123       -- is milliseconds offset from now
                        #01020304  -- is rgba color
                        .01030507  -- is midi data
                        [1[2]3]    -- is array with int, an array, and an int
        */

        type::packet strToOSC(std::string OSCstr) {
                auto pass_blob = [](char* &c, unsigned max) -> type::blob {
                        type::blob blob {};
                        unsigned char uc = 0;
                        while (true) {
                                switch (*c) {
                                        case '0':           uc = 0x00; break;
                                        case '1':           uc = 0x10; break;
                                        case '2':           uc = 0x20; break;
                                        case '3':           uc = 0x30; break;
                                        case '4':           uc = 0x40; break;
                                        case '5':           uc = 0x50; break;
                                        case '6':           uc = 0x60; break;
                                        case '7':           uc = 0x70; break;
                                        case '8':           uc = 0x80; break;
                                        case '9':           uc = 0x90; break;
                                        case 'A': case 'a': uc = 0xA0; break;
                                        case 'B': case 'b': uc = 0xB0; break;
                                        case 'C': case 'c': uc = 0xC0; break;
                                        case 'D': case 'd': uc = 0xD0; break;
                                        case 'E': case 'e': uc = 0xE0; break;
                                        case 'F': case 'f': uc = 0xF0; break;
                                        default:  goto BRK2;
                                }
                                c++;
                                switch (*c) {
                                        case '0':           uc |= 0x00; break;
                                        case '1':           uc |= 0x01; break;
                                        case '2':           uc |= 0x02; break;
                                        case '3':           uc |= 0x03; break;
                                        case '4':           uc |= 0x04; break;
                                        case '5':           uc |= 0x05; break;
                                        case '6':           uc |= 0x06; break;
                                        case '7':           uc |= 0x07; break;
                                        case '8':           uc |= 0x08; break;
                                        case '9':           uc |= 0x09; break;
                                        case 'A': case 'a': uc |= 0x0A; break;
                                        case 'B': case 'b': uc |= 0x0B; break;
                                        case 'C': case 'c': uc |= 0x0C; break;
                                        case 'D': case 'd': uc |= 0x0D; break;
                                        case 'E': case 'e': uc |= 0x0E; break;
                                        case 'F': case 'f': uc |= 0x0F; break;
                                        default: {
                                                blob.push_back(uc);
                                                goto BRK2;
                                        }
                                }
                                c++;
                                blob.push_back(uc);
                                if(blob.size() == max) goto BRK2;
                        }
                        BRK2:
                        return blob;
                };

                auto pass_number = [](char* &c, const bool negative) -> type::argument {
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
                        mode md = i;
                        char* start = c;
                        char* end = nullptr;
                        while (*c != '\0') switch (*c) {
                                case '.': {
                                        if(md != i) throw std::domain_error("Multiple '.' in number.");
                                        else md = f;
                                        c++;
                                } break;

                                case 'i': case 'I': {
                                        if (md == f)
                                                throw std::domain_error("Float treated as int in OSC string");
                                        end = c;
                                        c++;
                                } goto BRK;

                                case 'f': case 'F': {
                                        md = f;
                                        end = c;
                                        c++;
                                } goto BRK;

                                #ifdef OSCC_TYPE_d
                                        case 'd': case 'D': {
                                                md = d;
                                                end = c;
                                                c++;
                                        } goto BRK;
                                #endif

                                #ifdef OSCC_TYPE_h
                                        case 'l': case 'L': {
                                                if (md == f)
                                                        throw std::domain_error("Float treated as long in OSC string");
                                                md = l;
                                                end = c;
                                                c++;
                                        } goto BRK;
                                #endif

                                case '0': case '1': case '2':
                                case '3': case '4': case '5':
                                case '6': case '7': case '8':
                                case '9': {
                                        c++;
                                } break;

                                default:
                                        end = c;
                                        goto BRK;
                        }

                        BRK:
                        if(end == nullptr) end = c;
                        if(end == start) return 0;
                        char tmp;
                        switch (md) {
                                case i: {
                                        tmp = *end;
                                        *end = '\0';
                                        type::int32 v = std::stoi(start);
                                        *end = tmp;
                                        return (negative) ? -v : v;
                                }

                                case f: {
                                        tmp = *end;
                                        *end = '\0';
                                        type::float32 v = std::stof(start);
                                        *end = tmp;
                                        return (negative) ? -v : v;
                                }

                                #ifdef OSCC_TYPE_d
                                        case d: {
                                                tmp = *end;
                                                *end = '\0';
                                                type::float64 v = std::stold(start);
                                                *end = tmp;
                                                return (negative) ? -v : v;
                                        }
                                #endif

                                #ifdef OSCC_TYPE_h
                                        case l: {
                                                tmp = *end;
                                                *end = '\0';
                                                type::int64 v = std::stoll(start);
                                                *end = tmp;
                                                return (negative) ? -v : v;
                                        }
                                #endif
                        }
                };

                auto pass_argument = [&](char* &c) -> type::argument {
                        switch (*c) {
                                case '"': {
                                        std::string ret;
                                        c++;
                                        while (*c != '\0') {
                                                if(*c == '"' && *(c-1) != '\\') break;
                                                ret += *c;
                                                c++;
                                        }
                                        if(*c == '\0') throw std::domain_error("Missing '\"' in OSC string");
                                        c++;
                                        while(*c == ' ') c++;
                                        return ret;
                                }

                                case '\'' : {
                                        c++;
                                        char r = *c;
                                        if(*c == '\\') {
                                                c++;
                                                r = *c;
                                        }
                                        if(*(c+1) != '\'')  throw std::domain_error("Missing \"'\" in OSC string");
                                        c+=2;
                                        while(*c == ' ') c++;
                                        return r;
                                }

                                case '[': throw std::domain_error("Illegal '['");
                                case '}': case ']': throw std::domain_error("Illegal ']' or '}'");

                                case '-': {
                                        c++;
                                        return pass_number(c, true);
                                }

                                case 'x': case 'X': {
                                        c++;
                                        return pass_blob(c, -1);
                                }

                                case '0': case '1': case '2':
                                case '3': case '4': case '5':
                                case '6': case '7': case '8':
                                case '9': return pass_number(c, false);

                                #ifdef OSCC_TYPE_m
                                        case '.': {
                                                c++;
                                                type::blob blob = pass_blob(c, 4);
                                                if(blob.size() != 4)
                                                        throw std::domain_error("MIDI data bust have 8 hex letters");
                                                return type::midi {
                                                        (unsigned char) blob[0],
                                                        (unsigned char) blob[1],
                                                        (unsigned char) blob[2],
                                                        (unsigned char) blob[3]
                                                };
                                        }
                                #endif

                                #ifdef OSCC_TYPE_r
                                        case '#': {
                                                c++;
                                                type::blob blob = pass_blob(c, 4);
                                                if(blob.size() != 4)
                                                        throw std::domain_error("RGBA data bust have 8 hex letters");
                                                return type::rgba {
                                                        (unsigned char) blob[0],
                                                        (unsigned char) blob[1],
                                                        (unsigned char) blob[2],
                                                        (unsigned char) blob[3]
                                                };
                                        }
                                #endif

                                #ifdef OSCC_TYPE_TF
                                        case 't': case 'T': return type::T;
                                        case 'f': case 'F': return type::F;
                                #endif

                                #ifdef OSCC_TYPE_I
                                        case 'i': case 'I': return type::I;
                                #endif

                                #ifdef OSCC_TYPE_N
                                        case 'n': case 'N': return type::N;
                                #endif

                                #ifdef OSCC_TYPE_t
                                        case ':': {
                                                c++;
                                                type::int64 offset;
                                                const auto& num = pass_number(c, false);

                                                if(const type::int32* v = std::get_if<type::int32>(&num)) {
                                                        offset = *v;
                                                } else if(const type::int64* v = std::get_if<type::int64>(&num)) {
                                                        offset = *v;
                                                } else if(const type::float32* v = std::get_if<type::float32>(&num)) {
                                                        offset = *v;
                                                } else if(const type::float64* v = std::get_if<type::float64>(&num)) {
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
                                                        if(*c == '}' && *(c-1) != '\\') break;
                                                        ret.identifier += *c;
                                                        c++;
                                                }
                                                if(*c == '\0') throw std::domain_error("Missing '}' in OSC string");
                                                c++;
                                                while(*c == ' ') c++;
                                                return type::symbol{ret};
                                        }
                                #endif
                                default:
                                        throw std::domain_error(std::string("Error on OSC string at char [") + (*c) + "]");
                        }
                };

                char *c = &OSCstr[0];
                while (*c == ' ') c++;
                if(*c == '#') {
                        // We are on a bundle
                        c++;
                        type::int64 offset;
                        const auto& num = pass_number(c, false);

                        if(const type::int32* v = std::get_if<type::int32>(&num)) {
                                offset = *v;
                        #ifdef OSCC_TYPE_h
                                } else if(const type::int64* v = std::get_if<type::int64>(&num)) {
                                        offset = *v;
                        #endif
                        } else if(const type::float32* v = std::get_if<type::float32>(&num)) {
                                offset = *v;
                        #ifdef OSCC_TYPE_d
                                } else if(const type::float64* v = std::get_if<type::float64>(&num)) {
                                        offset = *v;
                        #endif
                        } else {
                                throw std::domain_error("Unknown error.");
                        }
                        const type::int64 milliseconds_since_epoch =
                                std::chrono::duration_cast<std::chrono::milliseconds>(
                                        std::chrono::system_clock::now().time_since_epoch()
                                ).count();

                        type::bundle bdl{type::time{milliseconds_since_epoch + offset}};
                        while (*c == ' ') c++;
                        if(*c == '\0') {
                                return bdl;
                        } else if(*c == '[') {
                                c++;
                                int indent = 1;
                                std::string str {};
                                while(*c != '\0' && indent != 0) {
                                        if(*c == '[') {
                                                indent++;
                                                str += *c;
                                        } else if(*c == ']') {
                                                indent--;
                                                if(indent > 0)
                                                        str += *c;
                                        } else if(*c == ';' && indent == 1) {
                                                bdl.push(strToOSC(str));
                                                str = "";
                                        } else {
                                                str += *c;
                                        }
                                        c++;
                                }
                                if(indent != 0) throw std::domain_error("Missing ']' for bundle in OSC string");
                                if(str.size() != 0) {
                                        bdl.push(strToOSC(str));
                                        str = "";
                                }
                                return bdl;
                        } else {
                                throw std::domain_error("Unknown error in bundle OSC string");
                        }
                } else if(*c == '/') {
                        std::string pat;
                        while (*c != '\0' && *c != ' ') {
                                pat += *c;
                                c++;
                        }
                        if(!isValidPattern(pat))
                                throw std::domain_error("Invalid pattern: " + pat);
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
                                        if(*c == '[') {
                                                stack.back()->push_back(type::arguments{});
                                                stack.push_back(&std::get<type::arguments>(stack.back()->back()));
                                                c++;
                                        } else if (*c == ']') {
                                                stack.pop_back();
                                                if(stack.empty())
                                                        throw std::domain_error("Unexpected ']' in OSC string");
                                                c++;
                                        } else {
                                                stack.back()->push_back(pass_argument(c));
                                        }
                                        while (*c == ' ') c++;
                                }
                                if(stack.size() != 1)
                                        throw std::domain_error("Missing ] in OSC string");
                        #endif
                        return message;
                } else {
                        throw std::domain_error("OSC message should start with '/' or '#'");
                }
        }

        type::time getCurrentTime() {
                return type::time{std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()};
        }
}
