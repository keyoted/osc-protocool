/*
        OSC OSCpacketList:
                1) Byte count (multiple of 4)
                2) Contents:
                        * OSC OSCbundle:
                                1) OSC string "#OSCbundle  "
                                2) OSC Time Tag
                                3) OSC Bundle Elements (0 or more)
                                        Each OSCbundle element:
                                                1) Byte count
                                                2) Contents
                                                        Either an OSC OSCmessage or an OSC OSCbundle
                        * OSC OSCmessage:
                                1) OSC Address pattern:
                                        1) OSC string beginning with '/'
                                2) OSC Type Tag String (maybe omitted):
                                        1) OSC-string beginning with the character ','
                                        2) Sequence of characters corresponding exactly to the sequence of OSC Arguments
                                                i - int32
                                                f - float32
                                                s - OSC string
                                                b - OSC blob
                                                Non-standard arguments:
                                                        h - 64 bit big-endian two's complement integer
                                                        t - OSC-timetag
                                                        d - 64 bit ("double") IEEE 754 floating point number
                                                        S - Alternate type represented as an OSC-string (for example, for systems that differentiate "symbols" from "strings")
                                                        c - An ascii character, sent as 32 bits
                                                        r - 32 bit RGBA color
                                                        m - 4 byte MIDI OSCmessage. Bytes from MSB to LSB are: port id, status byte, data1, data2
                                                        T - True. No bytes are allocated in the OSCargument data.
                                                        F - False. No bytes are allocated in the OSCargument data.
                                                        N - Nil. No bytes are allocated in the OSCargument data.
                                                        I - Infinitum. No bytes are allocated in the OSCargument data.
                                                        [ - Indicates the beginning of an array. The tags following are for data in the Array until a close brace tag is reached.
                                                        ] - Indicates the end of an array.

                                                        OSC applications are not required to recognize these types;
                                                        An OSC application should discard any OSCmessage whose OSC Type Tag String contains any unrecognized OSC Type Tags
                                3) OSC Arguments (0 or more)
                                        A sequence of OSC Arguments is represented by a contiguous sequence of the binary representations of each OSCargument.
*/

#include <variant>
#include <utility>
#include <filesystem>
#include <vector>
#include <cstdint>
#include <array>
#include <stdexcept>

namespace OSCC {

        typedef std::int32_t OSCint32;
        typedef std::float_t OSCfloat32;
        typedef std::string OSCstring;
        typedef std::vector<char> OSCblob;
        typedef std::filesystem::path OSCaddress;
        typedef std::int64_t OSCtime;

        typedef std::variant<
                OSCint32,
                OSCfloat32,
                OSCstring,
                OSCblob> OSCargument;

        typedef std::vector<OSCargument> OSCargumentList;

        class OSCbundle;
        class OSCmessage;

        typedef std::variant<OSCbundle, OSCmessage> OSCpacket;
        typedef std::vector<OSCpacket> OSCpacketList;

        class OSCmessage {
                public:
                        OSCaddress addressPattern;
                        OSCargumentList arguments;
        };

        class OSCbundle {
                public:
                        OSCtime timeTag;
                        OSCpacketList contents;
        };

        template<typename T>
        class arrayConsumer {
                private:
                        T *data;
                        std::size_t size;
                public:
                        arrayConsumer(T* data, std::size_t size)
                                :data(data)
                                ,size(size)
                        {}

                        T* consume(std::size_t elements) {
                                if(size > elements) throw std::out_of_range("Invalid access");
                                T * n = data;
                                data += elements;
                                size -= elements;
                                return n;
                        }

                        arrayConsumer<T> reserve(std::size_t limit) {
                                if(limit > size) throw std::out_of_range("Cannot extend range");
                                T * n = data;
                                data += limit;
                                size -= limit;
                                return arrayConsumer(n, limit);
                        }

                        T peek() {
                                if(size == 0) throw std::out_of_range("Invalid access");
                                return *data;
                        }

                        bool isEmpty() {
                                return size == 0;
                        }

                        T* consumeUntilOrThrow(const T &element) {
                                for(std::size_t consumed = 0; size - consumed != 0; consumed++) {
                                        if(data[consumed] == element) {
                                                return consume(consumed+1);
                                        }
                                }
                                throw std::out_of_range("Element not found in range");
                        }
        };

        OSCint32 read_int32(arrayConsumer<char> &data) {
                OSCint32 ret;
                std::memcpy(&ret, data.consume(sizeof(OSCint32)), sizeof(OSCint32));
                return ret;
        }

        OSCfloat32 read_float32(arrayConsumer<char> &data) {
                OSCfloat32 ret;
                std::memcpy(&ret, data.consume(sizeof(OSCfloat32)), sizeof(OSCfloat32));
                return ret;
        }

        OSCtime read_time(arrayConsumer<char> &data) {
                OSCtime ret;
                std::memcpy(&ret, data.consume(sizeof(OSCtime)), sizeof(OSCtime));
                return ret;
        }

        OSCstring read_OSCstring(arrayConsumer<char> &data) {
                OSCstring ret {data.consumeUntilOrThrow('\0')};
                data.consume(ret.length() % 4);
                return ret;
        }

        OSCblob read_OSCblob(arrayConsumer<char> &data) {
                OSCint32 size = read_int32(data);
                auto dr = data.reserve(size);
                OSCblob ret(dr.consume(0), dr.consume(0) + size);
                return ret;
        }



        OSCpacket extractOSCPacket(arrayConsumer<char> &raw) {

                const auto cast = [](auto * const var, arrayConsumer<char> &src) {
                        std::memcpy(var, src.consume(sizeof(*var)), sizeof(*var));
                };

                OSCint32 size = read_int32(raw);
                arrayConsumer<char> data = raw.reserve(size);

                if(data.peek() == '#') {
                        // Is OSC bundle
                        if(read_OSCstring(data) != "#OSCbundle\0")
                                throw std::domain_error("When a OSC packet address starts with '#' it must be \"#OSCbundle\" representing a bundle");
                        OSCbundle bundle {};
                        bundle.timeTag = read_time(data);
                        while (!data.isEmpty()) bundle.contents.push_back(extractOSCPacket(data));
                        return bundle;
                } else if(data.peek() == '/') {
                        // Is OSC message
                        OSCmessage message;
                        message.addressPattern = OSCaddress { read_OSCstring(data) };

                        if(data.isEmpty()) {
                                // Type tag omitted, packet ended
                                return message;
                        } else if(data.peek() == ',') {
                                // Type tag not omitted
                                const OSCstring typeTag {read_OSCstring(data)};
                                for(std::size_t i = 1; i < typeTag.length(); i++) {
                                        switch (typeTag[i]) {
                                                case 'i': message.arguments.push_back(read_int32(data)); break;
                                                case 'f': message.arguments.push_back(read_float32(data)); break;
                                                case 's': message.arguments.push_back(read_OSCstring(data)); break;
                                                case 'b': message.arguments.push_back(read_OSCblob(data)); break;
                                        }
                                }
                        } else {
                                // Type tag omitted, packet has additional data
                                throw std::domain_error("OSC message has no type tag, but contains arguments");
                        }

                } else {
                        throw std::domain_error("A OSC packet address must be \"#OSCbundle\" or must start with '/'");
                }
        }
}
