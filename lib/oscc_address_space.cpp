#include "oscc_address_space.hpp"
#include <thread>

namespace oscc {
        const type::call_back& address_space::find_(const std::string& pattern, const type::call_back& not_found) {
                if (!oscc::core::util::isValidPattern(pattern))
                        throw std::domain_error("Invalid pattern: [" + pattern + "]");
                mutex_.lock();
                for (const auto& adr : functions_) {
                        if (oscc::core::util::isMatch(adr.first, pattern)) {
                                mutex_.unlock();
                                return adr.second;
                        }
                }
                mutex_.unlock();
                return not_found;
        }

        void address_space::registerFunction(const type::address& adr, const type::call_back& cal) {
                // TODO: ensure address contains only legal characters
                // TODO: ensure address is only address and not address pattern
                if (!oscc::core::util::isValidAddress(adr.string()))
                        throw std::domain_error(std::string("Invalid address: [") + adr.string() + "]");
                mutex_.lock();
                functions_.insert_or_assign(adr.string(), cal);
                mutex_.unlock();
        }

        void address_space::dispatch_(type::packet pack, const type::call_back& not_found) {
                if (std::holds_alternative<type::message>(pack)) {
                        auto&       msg = std::get<type::message>(pack);
                        const auto& fcn = find_(msg.pattern().string(), not_found);
                        mutex_.lock();
                        fcn(pack);
                        mutex_.unlock();
                        std::exit(0);
                } else {
                        const auto& bdl            = std::get<type::bundle>(pack);
                        const auto  ms_since_epoch = core::util::getCurrentTime().unix;
                        if (ms_since_epoch >= bdl.time().unix) {
                                dispatch_(pack, not_found);
                        } else {
                                const auto sleep = bdl.time().unix - ms_since_epoch;
                                std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
                                for (const auto& pck : bdl.contents()) { dispatch(pck, not_found); }
                        }
                        std::exit(0);
                }
        }

        void address_space::dispatch(type::packet pack, const type::call_back& not_found) {
                std::thread(&address_space::dispatch_, this, pack, not_found).detach();
        }

}  // namespace oscc