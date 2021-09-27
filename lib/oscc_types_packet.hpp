#pragma once
#include "oscc_types.hpp"

namespace oscc::type {
        //! An OSC packet
        /**
         * An OSC packet may contain either a bundle or a message.
         */
        class packet : public std::variant<bundle, message> {
                public:
                        /**
                         * Implicit constructor to turn OSC string into packets.
                         * @param OSCstring An OSC string containing the information needed to construct the packet.
                         */
                        packet(std::string OSCstring);

                        /**
                         * Constructor.
                         * @tparam T The type of data. (either a bundle or a message)
                         * @param t The data for the packet.
                         */
                        template <typename T>
                        packet(T t) : variant(t){};

                        /**
                         * String representation.
                         * @return A string representation of the packet.
                         */
                        [[nodiscard]] std::string string() const;
        };
}  // namespace oscc::type
