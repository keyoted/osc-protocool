#pragma once

#include <iostream>
#include <mutex>
#include <thread>

#include "oscc_util.hpp"

namespace oscc {
        //! Represents an OSC address space.
        /**
         * An OSC address space contains a series of system functions/methods
         * which can be referenced by their paths. (not their pattern)
         */
        class address_space {
                private:
                        /**
                         * A map which makes the correspondence between a
                         * function and its address.
                         */
                        std::unordered_map<std::string, type::call_back> functions_;

                        /**
                         * Mutex to for thread-safety.
                         */
                        std::mutex mutex_;

                        /**
                         * Finds all functions whose address matches a given
                         * pattern.
                         * @param pattern The pattern.
                         * @return T
                         */
                        std::vector<type::call_back> find_(const std::string& pattern);

                        /**
                         * Dispatch a packet to its corresponding functions.
                         * This function should not be called from the main
                         * thread.
                         * @param pack The packet to dispatch.
                         * @param not_found A callback to call in case the
                         * packet could not be dispatched.
                         */
                        void dispatch_(type::packet pack, const type::call_back& not_found);

                        /**
                         * Default callback for an un-dispatched packet.
                         * @param pkt The packet that was not dispatched.
                         */
                        static void default_not_found_call_back_(const type::packet& pkt);

                public:
                        /**
                         * Register a function into the address space.
                         * @param address The address of the function.
                         * @param call_back The function to register.
                         */
                        void registerFunction(const type::address& address, const type::call_back& call_back);

                        /**
                         * Dispatch a packet using the registered call_backs.
                         * @param pack The packet to dispatch.
                         * @param not_found The function to call in case the
                         * packet could not be dispatched.
                         */
                        void dispatch(const type::packet& pack, const type::call_back& not_found);

                        /**
                         * Dispatch a packet using the registered call_backs and
                         * the default callback for non-dispatched callbacks.
                         * @param pack The packet to dispatch.
                         */
                        void dispatch(const type::packet& pack);
        };
}  // namespace oscc