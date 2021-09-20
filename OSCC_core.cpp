/*
        OSC packets:
                1) Byte count (multiple of 4)
                2) Contents:
                        * OSC bundle:
                                1) OSC string "#bundle  "
                                2) OSC Time Tag
                                3) OSC Bundle Elements (0 or more)
                                        Each bundle element:
                                                1) Byte count
                                                2) Contents
                                                        Either an OSC message or an OSC bundle
                        * OSC message:
                                1) OSC Address pattern:
                                        1) OSC string beginning with '/'
                                2) OSC Type Tag String (maybe omitted):
                                        1) OSC-string beginning with the character ','
                                        2) Sequence of characters corresponding exactly to the sequence of OSC Arguments
                                                i - int32
                                                f - float32
                                                s - OSC string
                                                b - OSC blob
                                                Non-standard arguments_:
                                                        h - 64 bit big-endian two's complement integer
                                                        t - OSC-timetag
                                                        d - 64 bit ("double") IEEE 754 floating point number
                                                        S - Alternate type represented as an OSC-string (for example, for systems that differentiate "symbols" from "strings")
                                                        c - An ascii character, sent as 32 bits
                                                        r - 32 bit RGBA color
                                                        m - 4 byte MIDI message. Bytes from MSB to LSB are: port id, status byte, data1, data2
                                                        T - True. No bytes are allocated in the argument data.
                                                        F - False. No bytes are allocated in the argument data.
                                                        N - Nil. No bytes are allocated in the argument data.
                                                        I - Infinitum. No bytes are allocated in the argument data.
                                                        [ - Indicates the beginning of an array. The tags following are for data in the Array until a close brace tag is reached.
                                                        ] - Indicates the end of an array.

                                                        OSC applications are not required to recognize these types;
                                                        An OSC application should discard any message whose OSC Type Tag String contains any unrecognized OSC Type Tags
                                3) OSC Arguments (0 or more)
                                        A sequence of OSC Arguments is represented by a contiguous sequence of the binary representations of each argument.
*/

#include "OSCC_core.hpp"


namespace OSCC::core {
        types::packet extractOSCPacket(util::arrayConsumer<char> &raw) {
                types::int32 size = read::int32(raw);
                util::arrayConsumer<char> data = raw.reserve(size);

                if(data.peek() == '#') {
                        // Is OSC bundle
                        if(read::string(data) != "#bundle")
                                throw std::domain_error("When a OSC packet address_ starts with '#' it must be \"#bundle\" representing a bundle");
                        types::bundle bundle {read::time(data)};
                        while (!data.isEmpty())
                                bundle.push(extractOSCPacket(data));
                        return types::packet{bundle};
                } else if(data.peek() == '/') {
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

                } else {
                        throw std::domain_error("A OSC packet address_ must be \"#bundle\" or must start with '/'");
                }
        }
}
