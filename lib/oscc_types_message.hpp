#pragma once
#include "oscc_types.hpp"

namespace oscc::type {
        //! An OSC bundle
        /**
         * An OSC message contains an address pattern that matches methods in
         * the system and it also contains a list of arguments.
         * When the system matches one or more methods with the message's
         * pattern, then the message's arguments are passed into the method(s)
         * that were matched.
         */
        class message {
                private:
                        /** The message's address pattern. */
                        type::address address_pattern_;

                        /** The message's arguments. */
                        type::arguments arguments_;

                public:
                        /**
                         * Constructor.
                         * @param pattern The message's address pattern.
                         */
                        explicit message(type::address pattern);

                        /**
                         * Push an argument into the message.
                         * @tparam T The type of the argument.
                         * @param val The argument to push.
                         */
                        template <typename T>
                        void push(T&& val) {
                                arguments_.emplace_back(std::forward<T>(val));
                        }

                        /**
                         * Push an argument into the message.
                         * @param arg The argument to push.
                         */
                        void push(type::argument& arg);

                        /**
                         * String representation.
                         * @return A string representation of the message.
                         */
                        [[nodiscard]] std::string string() const;

                        /**
                         * Getter.
                         * @return The message's pattern.
                         */
                        [[nodiscard]] type::address& pattern();

                        /**
                         * Getter.
                         * @return The message's arguments.
                         */
                        [[nodiscard]] type::arguments& arguments();

                        /**
                         * Getter.
                         * @return The message's pattern.
                         */
                        [[nodiscard]] const type::address& pattern() const;

                        /**
                         * Getter.
                         * @return The message's arguments.
                         */
                        [[nodiscard]] const type::arguments& arguments() const;
        };
}  // namespace oscc::type
