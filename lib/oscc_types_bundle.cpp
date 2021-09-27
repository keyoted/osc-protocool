#include "oscc_types.hpp"
#include "oscc_util.hpp"

namespace oscc::type {

        bundle::bundle(oscc::type::time time) : time_(time) {}
        type::time           bundle::time() const { return time_; }
        const type::packets& bundle::contents() const { return contents_; }
        std::string          bundle::string() const { return util::string(*this); }

}  // namespace oscc::type
