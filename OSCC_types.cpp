#include "OSCC_types.hpp"

#include <utility>

namespace OSCC::types {
        message::message(OSCC::types::address path)
        :address(std::move(path))
        {}

        template<class T>
        void message::push(T &&val) {
                arguments.push_back(std::forward<T>(val));
        }

        bundle::bundle(OSCC::types::time time)
        : time(time)
        {}

        template<class T>
        void bundle::push(T &&val) {
                contents.push_back(std::forward<T>(val));
        }
}
