#include "oscc_types.hpp"

namespace oscc::types {

        void address_space::delayed_dispatch(bundle bdl, const call_back& not_found, int64 sleep) {
                std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
                for(const auto& pck : bdl.contents()) {
                        dispatch(pck, not_found);
                }
        }

        const call_back& address_space::find(std::string pattern, const call_back& not_found) {
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

        void address_space::registerFunction(const address adr, const call_back cal) {
                // TODO: ensure address contains only legal characters
                // TODO: ensure address is only address and not address pattern
                mutex_.lock();
                functions_.insert_or_assign(adr.string(), cal);
                mutex_.unlock();
        }

        void address_space::dispatch(packet pack, const call_back& not_found) {
                if(std::holds_alternative<message>(pack)) {
                        const auto& msg = std::get<message>(pack);
                        const auto& fcn = find(msg.pattern().string(), not_found);
                        mutex_.lock();
                        fcn(pack);
                        mutex_.unlock();
                } else {
                        const auto& bdl = std::get<bundle>(pack);
                        const int64 milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                        if(milliseconds_since_epoch >= bdl.time()) delayed_dispatch(bdl, not_found, 0);
                        else delayed_dispatch(bdl, not_found, bdl.time() - milliseconds_since_epoch);
                }
        }

}