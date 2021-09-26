#include "oscc_types.hpp"
#include "oscc_util.hpp"

namespace oscc::type {
        bundle::bundle(oscc::type::time time)
        : time_(time)
        {}

        type::time bundle::time() const {
                return time_;
        }

        type::packets bundle::contents() const {
                return contents_;
        }

        std::string bundle::string() const {
                std::string str{"#"};

                const auto cur_time = core::util::getCurrentTime();
                if(time().unix <= cur_time.unix) str += '0';
                else str += std::to_string(time().unix - cur_time.unix);

                str += '[';
                std::for_each(contents_.begin(),  contents_.end(), [&str](const auto &packet) {
                        str += packet.string() + "; ";
                });
                if(contents_.empty()) str += "] ";
                else *(str.end()-2) = ']';

                return str;
        }
}
