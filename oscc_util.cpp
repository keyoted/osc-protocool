#include "oscc_util.hpp"

namespace oscc::core::util {
        oscc::types::time NTPtoUNIX(const oscc::types::time &NTP) {
                if ((NTP & 1U) == 1) return 0; // Least significant bit means now
                else {
                        const auto seconds = ((oscc::types::int32*)&NTP) [0];
                        const auto fraction = ((oscc::types::int32*)&NTP) [1];
                        const auto millis = (seconds * 1000) + ((fraction * 1000) / 0x100000000L);
                        return millis + 2208988800U; // There is a constant offset of seconds between 1900/1/1 UTC and 1970/1/1 UTC
                }
        }

        oscc::types::time UNIXtoNTP(const oscc::types::time &UNIX) {
                const auto millis = UNIX - 2208988800U; // There is a constant offset of seconds between 1900/1/1 UTC and 1970/1/1 UTC
                const oscc::types::int32 secconds = millis / 1000;
                const oscc::types::int32 fraction = ((millis % 1000) * 0x100000000L) / 1000;
                const oscc::types::int32 ret[2] {secconds, fraction};
                return *((oscc::types::time*)ret);
        }

}