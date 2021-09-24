#pragma once

#include <thread>
#include <mutex>

#include "oscc_types.hpp"
#include "oscc_core.hpp"

namespace oscc {
        class address_space {
                private:
                        std::unordered_map<std::string, type::call_back> functions_;
                        std::mutex mutex_;
                        const type::call_back& find(std::string pattern, const type::call_back& not_found);
                        void delayed_dispatch(type::bundle bdl, const type::call_back& not_found, type::int64 sleep);
                public:
                        void registerFunction(type::address adr, type::call_back cal);
                        void dispatch(type::packet pack, const type::call_back& not_found);
        };
}