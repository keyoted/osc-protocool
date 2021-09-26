#pragma once

#include <mutex>
#include <thread>

#include "oscc_core.hpp"
#include "oscc_types.hpp"

namespace oscc {
        class address_space {
                private:
                        std::unordered_map<std::string, type::call_back> functions_;
                        std::mutex                                       mutex_;
                        const type::call_back& find_(const std::string& pattern, const type::call_back& not_found);
                        void                   dispatch_(type::packet pack, const type::call_back& not_found);

                public:
                        void registerFunction(const type::address& adr, const type::call_back& cal);
                        void dispatch(type::packet pack, const type::call_back& not_found);
        };
}  // namespace oscc