#pragma once
#include "oscc_types.hpp"

namespace oscc::type {
        //! An OSC bundle
        /**
         * An OSC bundle contains an execution time and contents, in the form of
         * packets, that will only be dispatched into the system during the
         * specified time.
         */
        class bundle {
                private:
                        /** The bundle's execution time. */
                        type::time time_;

                        /** The bundle's packet contents. */
                        type::packets contents_;

                public:
                        /**
                         * Push a packet into the bundle's contents.
                         * @tparam T The type of the package.
                         * @param val The value to put in the contents.
                         */
                        template <typename T>
                        void push(T&& val) {
                                contents_.push_back(std::forward<T>(val));
                        }

                        /**
                         * Constructor.
                         * @param time The bundles execution time.
                         */
                        explicit bundle(type::time time);

                        /**
                         * String representation.
                         * @return The string representation of this bundle.
                         */
                        [[nodiscard]] std::string string() const;

                        /**
                         * Getter.
                         * @return The bundle's execution time.
                         */
                        [[nodiscard]] type::time time() const;

                        /**
                         * Getter.
                         * @return The bundle's contents.
                         */
                        [[nodiscard]] const type::packets& contents() const;
        };
}  // namespace oscc::type
