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
                                        *ptrn++;
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
}