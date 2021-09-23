#include <iostream>
#include <array>
#include <functional>
#include <type_traits>
#include "OSCC_core.hpp"
#include "OSCC_util.hpp"


size_t memdiv(const char *arr1, const char *arr2, size_t size);

int main () {
        using namespace std;
        //                    0   1   2   3   4    5   6      7   8   9   10  11    12  13              14              15      16
        //                    |   |   |   |   |    |   |      |   |   |   |   |     |   |               |               |       |
        char messageData[] = "size/main/help/*//3\0,ifsb\0\0\0iiiiffffa string:)\0\0size\x01\x23\x45\x67\x89\xAB\xCD\xEF\xFF\0\0";
        ((int*)messageData)[0] = (int) sizeof(messageData) - sizeof(oscc::types::int32);
        ((int*)messageData)[7] = (int) 86;
        ((float*)messageData)[8] = 3.14f;
        ((int*)messageData)[12] = (int) 9;

        //                   0   1   2    3   4   5   6   7   8   9    10  11     12  13  14  15  16    17  18              19              20        21  22  23  24  25   26  27     28  29  30  31  32    33  34              35              36              37              38        39
        //                   |   |   |    |   |   |   |   |   |   |    |   |      |   |   |   |   |     |   |               |               |         |   |   |   |   |    |   |      |   |   |   |   |     |   |               |               |               |               |         |
        char bundleData[] = "size#bundle\0timetimesize/main/help/*//3\0,ifsb\0\0\0iiiiffffa string:)\0\0size\x01\x23\x45\x67\x89\xAB\xCD\xEF\xFF\0\0\0size/main/help/*//3\0,ifsb\0\0\0iiiiffffa string:)\0\0size\x01\x23\x45\x67\x89\xAB\xCD\xEF\xFF\x01\x23\x45\x67\x89\xAB\xCD\xEF\xFF\0";
        ((int*)bundleData)[0] = (int) sizeof(bundleData) - sizeof(oscc::types::int32);
        ((oscc::types::time*)(bundleData + 3 * 4))[0] = (oscc::types::time) 1234;
        ((int*)bundleData)[0+5] = (int) sizeof(messageData) - sizeof(oscc::types::int32);
        ((int*)bundleData)[7+5] = (int) 69;
        ((float*)bundleData)[8+5] = 3.14f;
        ((int*)bundleData)[12+5] = (int) 9;
        ((int*)bundleData)[0+21] = (int) sizeof(messageData)+8 - sizeof(oscc::types::int32);
        ((int*)bundleData)[7+21] = (int) 420;
        ((float*)bundleData)[8+21] = 12.34f;
        ((int*)bundleData)[12+21] = (int) 9+9;

        //                         0   1   2    3   4   5   6   7   8   9    10  11     12  13  14  15  16    17  18              19              20        21  22  23  24  25   26  27     28  29  30  31  32    33  34              35              36              37              38          39
        //                         |   |   |    |   |   |   |   |   |   |    |   |      |   |   |   |   |     |   |               |               |         |   |   |   |   |    |   |      |   |   |   |   |     |   |               |               |               |               |           |
        char bundlebundleData[] = "size#bundle\0timetimesize/main/help/*//3\0,ifsb\0\0\0iiiiffffa string:)\0\0size\x01\x23\x45\x67\x89\xAB\xCD\xEF\xFF\0\0\0size/main/help/*//3\0,ifsb\0\0\0iiiiffffa string:)\0\0size\x01\x23\x45\x67\x89\xAB\xCD\xEF\xFF\x01\x23\x45\x67\x89\xAB\xCD\xEF\xFF\0\0size#bundle\0timetimesize/main/help/*//3\0,ifsb\0\0\0iiiiffffa string:)\0\0size\x01\x23\x45\x67\x89\xAB\xCD\xEF\xFF\0\0\0size/main/help/*//3\0,ifsb\0\0\0iiiiffffa string:)\0\0size\x01\x23\x45\x67\x89\xAB\xCD\xEF\xFF\x01\x23\x45\x67\x89\xAB\xCD\xEF\xFF\0";
        ((int*)bundlebundleData)[0] = (int) sizeof(bundlebundleData) - sizeof(oscc::types::int32);
        ((oscc::types::time*)(bundlebundleData + 3 * 4))[0] = (oscc::types::time) 12345678;
        ((int*)bundlebundleData)[0+5] = (int) sizeof(messageData) - sizeof(oscc::types::int32);
        ((int*)bundlebundleData)[7+5] = (int) 69;
        ((float*)bundlebundleData)[8+5] = 3.14f;
        ((int*)bundlebundleData)[12+5] = (int) 9;
        ((int*)bundlebundleData)[0+21] = (int) sizeof(messageData)+8 - sizeof(oscc::types::int32);
        ((int*)bundlebundleData)[7+21] = (int) 420;
        ((float*)bundlebundleData)[8+21] = 12.34f;
        ((int*)bundlebundleData)[12+21] = (int) 9+9;
        ((int*)bundlebundleData)[0+39] = (int) sizeof(bundleData) - sizeof(oscc::types::int32);
        ((oscc::types::time*)(bundlebundleData + (3 + 39) * 4))[0] = (oscc::types::time) 87654321;
        ((int*)bundlebundleData)[0+39+5] = (int) sizeof(messageData) - sizeof(oscc::types::int32);
        ((int*)bundlebundleData)[7+39+5] = (int) 69;
        ((float*)bundlebundleData)[8+39+5] = 3.14f;
        ((int*)bundlebundleData)[12+39+5] = (int) 9;
        ((int*)bundlebundleData)[0+39+21] = (int) sizeof(messageData)+8 - sizeof(oscc::types::int32);
        ((int*)bundlebundleData)[7+39+21] = (int) 420;
        ((float*)bundlebundleData)[8+39+21] = 12.34f;
        ((int*)bundlebundleData)[12+39+21] = (int) 9+9;

        #define msgdta bundlebundleData
        auto check = oscc::core::OSCToBytes(oscc::core::bytesToOSC(msgdta, sizeof(msgdta)));
        cout << "COMPARE: " << memcmp(msgdta, check.data(), min(sizeof(msgdta), check.size())) << " (" << memdiv(msgdta, check.data(), min(sizeof(msgdta), check.size())) << ")" << endl;
        auto j = 0;
        auto jj = 0;
        for(int i = 0; i < max(sizeof(msgdta), check.size()); i++) {
                if(j == 4) cout << "  ";
                if(j == 8) {cout << endl; j = 0; jj++;}
                if(j == 0) cout << setw(2) << setfill('0') << dec << jj*2 << " " << setw(4) << setfill('0') << jj * 8 << ".   ";
                j++;

                if(i < sizeof(msgdta)) cout << setfill('0') << setw(2) << uppercase << hex << (int) (unsigned char) msgdta[i] << ".";
                else cout << "  " << ".";

                if(i < check.size()) cout << setfill('0') << setw(2) << uppercase << hex << (int) (unsigned char) check[i];
                else cout << "  ";

                if(i < sizeof(msgdta) && i < check.size() && msgdta[i] == check[i]) cout << "   ";
                else cout << " ! ";
        }
        cout << endl;

        array<pair<char*, size_t>, 3> testData {
                pair{messageData, sizeof(messageData)},
                pair{bundleData, sizeof(bundleData)},
                pair{bundlebundleData, sizeof(bundlebundleData)}
        };

        for(pair<char*, size_t> data : testData) {
                size_t indent = 0;
                function<void(oscc::types::packet)> visitor = [&indent, &visitor](oscc::types::packet &&val) {
                        using namespace std;
                        const string i1 = string(indent*8, ' ');
                        const string i2 = string((indent+1)*8, ' ');

                        if (holds_alternative<oscc::types::bundle>(val)) {
                                const oscc::types::bundle &bundle = get<oscc::types::bundle>(val);
                                cout << i1 << "TYPE: BUNDLE" << endl;
                                cout << i1<< "TIMESTAMP: " << bundle.time() << endl;
                                cout << i1<< "CONTENTS:" << endl;
                                indent++;
                                for(auto& content : bundle.contents()) {
                                        cout << i2 << string(40,'+') << endl;
                                        visit(visitor, content);
                                        cout << i2 << string(40,'+') << endl;
                                }
                                indent--;
                        } else if (holds_alternative<oscc::types::message>(val)) {
                                const oscc::types::message &message = get<oscc::types::message>(val);
                                cout << i1 << "TYPE: MESSAGE" << endl;
                                cout << i1 << "ADDRESS: " << message.address() << endl;
                                cout << i1 << "ARGUMENTS:" << endl;
                                for (const oscc::types::argument &argument: message.arguments()) {
                                        visit([&i2](auto &&arg) {
                                                using T = decay_t<decltype(arg)>;
                                                if constexpr(is_same_v<T, oscc::types::int32>) {
                                                        cout << i2 << string(20,'-') << endl;
                                                        cout << i2 << "TYPE: INT 32" << endl;
                                                        cout << i2 << "VALUE: " << arg << endl;
                                                } else if constexpr(is_same_v<T, oscc::types::float32>) {
                                                        cout << i2 << string(20,'-') << endl;
                                                        cout << i2 << "TYPE: FLOAT 32" << endl;
                                                        cout << i2 << "VALUE: " << arg << endl;
                                                } else if constexpr(is_same_v<T, oscc::types::string>) {
                                                        cout << i2 << string(20,'-') << endl;
                                                        cout << i2 << "TYPE: STRING" << endl;
                                                        cout << i2 << "VALUE: \"" << arg << "\"" << endl;
                                                } else if constexpr(is_same_v<T, oscc::types::blob>) {
                                                        cout << i2 << string(20,'-') << endl;
                                                        cout << i2 << "TYPE: BLOB:" << endl;
                                                        ios init(NULL);
                                                        init.copyfmt(cout);
                                                        int i = 0;
                                                        for (const char &c: (oscc::types::blob) arg) {
                                                                if (i == 4) cout << "   ";
                                                                else if (i == 8) {
                                                                        cout << "\n";
                                                                        i = 0;
                                                                }
                                                                if (i == 0) cout << i2;
                                                                cout << setfill('0') << setw(2) << uppercase << hex
                                                                     << (int) ((unsigned char) c) << " ";
                                                                i++;
                                                        }
                                                        cout.copyfmt(init);
                                                        cout << endl;
                                                } else {
                                                        cout << i2 << string(20, '-') << endl;
                                                        cout << i2 << "TYPE: ERROR (" << typeid(T).name() << ")" << endl;
                                                }
                                        }, argument);
                                }
                        } else cout << i1 << "TYPE: ERROR" << endl;
                };

                cout << "SIZE: " << data.second << endl;
                oscc::core::util::arrayConsumer<char> ac{data.first, data.second};
                auto extracted = oscc::core::bytesToOSC(ac);
                visit(visitor, extracted);
                cout << string(40,'-') << endl;
        }

        return 0;
}

size_t memdiv(const char *arr1, const char *arr2, size_t size) {
        for (size_t i = 0; i < size; ++i)
                if(arr1[i] != arr2[i])
                        return i;
        return -1;
}
