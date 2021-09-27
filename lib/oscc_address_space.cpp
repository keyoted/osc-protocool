#include "oscc_address_space.hpp"

namespace oscc {
        std::vector<type::call_back> address_space::find_(const std::string& pattern) {
                if (!oscc::util::isValidPattern(pattern)) throw std::domain_error("Invalid pattern: [" + pattern + "]");

                std::vector<type::call_back> functions{};
                mutex_.lock();
                for (const auto& adr : functions_) {
                        if (oscc::util::isMatch(adr.first, pattern)) {
                                mutex_.unlock();
                                functions.push_back(adr.second);
                        }
                }
                mutex_.unlock();
                return functions;
        }

        void address_space::registerFunction(const type::address& address, const type::call_back& call_back) {
                if (!oscc::util::isValidAddress(address.string()))
                        throw std::domain_error(std::string("Invalid address: [") + address.string() + "]");
                mutex_.lock();
                functions_.insert_or_assign(address.string(), call_back);
                mutex_.unlock();
        }

        void address_space::dispatch_(type::packet pack, const type::call_back& not_found) {
                if (std::holds_alternative<type::message>(pack)) {
                        auto&       msg  = std::get<type::message>(pack);
                        const auto& fcns = find_(msg.pattern().string());
                        if (fcns.empty()) std::thread(not_found, pack).detach();
                        else
                                for (const auto& fcn : fcns) std::thread(fcn, pack).detach();
                        std::exit(0);
                } else {
                        const auto& bdl            = std::get<type::bundle>(pack);
                        const auto  ms_since_epoch = util::getCurrentTime().unix;
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

        void address_space::dispatch(const type::packet& pack, const type::call_back& not_found) {
                std::thread(&address_space::dispatch_, this, pack, not_found).detach();
        }

        void address_space::default_not_found_call_back_(const type::packet& packet) {
                std::cout << ("PACKET COULD NOT BE DELIVERED: " + packet.string() + "\n");
        }

        void address_space::dispatch(const type::packet& pack) {
                dispatch(pack, &address_space::default_not_found_call_back_);
        }

}  // namespace oscc