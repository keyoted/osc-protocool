#include "oscc_address_space.hpp"

namespace oscc {
        void address_space::delayed_dispatch(type::bundle bdl, const type::call_back& not_found, type::int64 sleep) {
                std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
                for(const auto& pck : bdl.contents()) {
                        dispatch(pck, not_found);
                }
        }

        const type::call_back& address_space::find(std::string pattern, const type::call_back& not_found) {
                mutex_.lock();
                for(const auto &adr : functions_) {
                        if(oscc::core::util::isMatch(adr.first, pattern)) {
                                mutex_.unlock();
                                return adr.second;
                        }
                }
                mutex_.unlock();
                return not_found;
        }

        void address_space::registerFunction(const type::address adr, const type::call_back cal) {
                // TODO: ensure address contains only legal characters
                // TODO: ensure address is only address and not address pattern
                mutex_.lock();
                functions_.insert_or_assign(adr.string(), cal);
                mutex_.unlock();
        }

        void address_space::dispatch(type::packet pack, const type::call_back& not_found) {
                if(std::holds_alternative<type::message>(pack)) {
                        const auto& msg = std::get<type::message>(pack);
                        const auto& fcn = find(msg.pattern().string(), not_found);
                        mutex_.lock();
                        fcn(pack);
                        mutex_.unlock();
                } else {
                        const auto& bdl = std::get<type::bundle>(pack);
                        const type::int64 milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                        if(milliseconds_since_epoch >= bdl.time()) delayed_dispatch(bdl, not_found, 0);
                        else delayed_dispatch(bdl, not_found, bdl.time() - milliseconds_since_epoch);
                }
        }

}