#include <iostream>
#include <array>
#include <functional>
#include <type_traits>
#include "lib/oscc_core.hpp"
#include "lib/oscc_util.hpp"


size_t memdiv(const char *arr1, const char *arr2, size_t size);

int main () {
        using namespace std;
        ios init(NULL);
        init.copyfmt(cout);

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
                function<void(oscc::types::packet)> visitor = [&indent, &visitor, &init](oscc::types::packet &&val) {
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
                                        visit([&i2, &init](auto &&arg) {
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

                cout.copyfmt(init);
                cout << "SIZE: " << data.second << endl;
                oscc::core::util::arrayConsumer<char> ac{data.first, data.second};
                auto extracted = oscc::core::bytesToOSC(ac);
                visit(visitor, extracted);
                cout << string(40,'-') << endl;
        }

        const string pats[] = {
                "/abc", "/abc", "t",
                "/abc", "/abcd", "f",
                "/abc", "/dabc", "f",
                "/ab?", "/abc", "t",
                "/a?c", "/abc", "t",
                "/?bc", "/abc", "t",
                "/ab?", "/abic", "f",
                "/a?c", "/abic", "f",
                "/?bc", "/abic", "f",

                "/???/*abc", "/abc/xxxabc", "t",
                "/???/abc*", "/abc/abcxxx", "t",
                "/???/*abc*", "/abc/abcxxxabc", "t",
                "/???/*abc", "/abc/xxxbc", "f",
                "/???/abc*", "/abc/abxxx", "f",
                "/???/*abc*", "/abc/xxxbxxx", "f",

                "/???/[abc]", "/abc/a", "t",
                "/???/[abc]", "/abc/b", "t",
                "/???/[abc]", "/abc/c", "t",
                "/???/[abc]", "/abc/d", "f",
                "/???/[a-z]", "/abc/a", "t",
                "/???/[a-z]", "/abc/r", "t",
                "/???/[a-z]", "/abc/z", "t",
                "/???/[-a]", "/abc/-", "t",
                "/???/[a-]", "/abc/-", "t",
                "/???/[a-]", "/abc/a", "t",
                "/???/[a-]", "/abc/b", "f",

                "/???/[!abc]", "/abc/a", "f",
                "/???/[!abc]", "/abc/b", "f",
                "/???/[!abc]", "/abc/c", "f",
                "/???/[!abc]", "/abc/d", "t",
                "/???/[!a-z]", "/abc/a", "f",
                "/???/[!a-z]", "/abc/r", "f",
                "/???/[!a-z]", "/abc/z", "f",
                "/???/[!-a]", "/abc/-", "f",
                "/???/[!a-]", "/abc/-", "f",
                "/???/[!a-]", "/abc/a", "f",
                "/???/[!a-]", "/abc/b", "t",

                "{amy,lisa,hana}/???/[xyz]/[!aeiou]", "amy/XYZ/z/t", "t",
                "{amy,lisa,hana}/???/[xyz]/[!aeiou]", "lisa/dei/x/f", "t",
                "{amy,lisa,hana}/???/[xyz]/[!aeiou]", "hana/wie/y/s", "t",
                "{amy,lisa,hana}/???/[xyz]/[!aeiou]", "jame/XYZ/z/t", "f",
                "{amy,lisa,hana}/???/[xyz]/[!aeiou]", "amy/aXYZ/z/t", "f",
                "{amy,lisa,hana}/???/[xyz]/[!aeiou]", "amy/XYZ/Z/t", "f",
                "{amy,lisa,hana}/???/[xyz]/[!aeiou]", "amy/XYZ/z/a", "f",
                "{amy,lisa,hana}/???/[xyz]/[!aeiou]", "amy/XYZ/zz/t", "f",
                "{amy,lisa,hana}/???/[xyz]/[!aeiou]", "amy/XYZ/z/tr", "f",

                "/abc/abc/abc", "/abc/abc/abc", "t",
                "/abc/abc/abc", "/abcd/abcd/abcd", "f",
                "/abc/abc/abc", "/dabc/dabc/dabc", "f",
                "/ab?/ab?/ab?", "/abc/abc/abc", "t",
                "/a?c/a?c/a?c", "/abc/abc/abc", "t",
                "/?bc/?bc/?bc", "/abc/abc/abc", "t",
                "/ab?/ab?/ab?", "/abic/abic/abic", "f",
                "/a?c/a?c/a?c", "/abic/abic/abic", "f",
                "/?bc/?bc/?bc", "/abic/abic/abic", "f",
                "/???/*abc/???/*abc", "/abc/xxxabc/abc/xxxabc", "t",
                "/???/abc*/???/abc*", "/abc/abcxxx/abc/abcxxx", "t",
                "/???/*abc*/???/*abc*", "/abc/abcxxxabc/abc/abcxxxabc", "t",
                "/???/*abc/???/*abc", "/abc/xxxbc/abc/xxxbc", "f",
                "/???/abc*/???/abc*", "/abc/abxxx/abc/abxxx", "f",
                "/???/*abc*/???/*abc*", "/abc/xxxbxxx/abc/xxxbxxx", "f",
                "/???/[abc]/???/[abc]", "/abc/a/abc/a", "t",
                "/???/[abc]/???/[abc]", "/abc/b/abc/b", "t",
                "/???/[abc]/???/[abc]", "/abc/c/abc/c", "t",
                "/???/[abc]/???/[abc]", "/abc/d/abc/d", "f",
                "/???/[a-z]/???/[a-z]", "/abc/a/abc/a", "t",
                "/???/[a-z]/???/[a-z]", "/abc/r/abc/r", "t",
                "/???/[a-z]/???/[a-z]", "/abc/z/abc/z", "t",
                "/???/[-a]/???/[-a]", "/abc/-/abc/-", "t",
                "/???/[a-]/???/[a-]", "/abc/-/abc/-", "t",
                "/???/[a-]/???/[a-]", "/abc/a/abc/a", "t",
                "/???/[a-]/???/[a-]", "/abc/b/abc/b", "f",
                "/???/[!abc]/???/[!abc]", "/abc/a/abc/a", "f",
                "/???/[!abc]/???/[!abc]", "/abc/b/abc/b", "f",
                "/???/[!abc]/???/[!abc]", "/abc/c/abc/c", "f",
                "/???/[!abc]/???/[!abc]", "/abc/d/abc/d", "t",
                "/???/[!a-z]/???/[!a-z]", "/abc/a/abc/a", "f",
                "/???/[!a-z]/???/[!a-z]", "/abc/r/abc/r", "f",
                "/???/[!a-z]/???/[!a-z]", "/abc/z/abc/z", "f",
                "/???/[!-a]/???/[!-a]", "/abc/-/abc/-", "f",
                "/???/[!a-]/???/[!a-]", "/abc/-/abc/-", "f",
                "/???/[!a-]/???/[!a-]", "/abc/a/abc/a", "f",
                "/???/[!a-]/???/[!a-]", "/abc/b/abc/b", "t",
                "/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou]", "/amy/XYZ/z/t/amy/XYZ/z/t", "t",
                "/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou]", "/lisa/dei/x/f/lisa/dei/x/f", "t",
                "/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou]", "/hana/wie/y/s/hana/wie/y/s", "t",
                "/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou]", "/jame/XYZ/z/t/jame/XYZ/z/t", "f",
                "/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou]", "/amy/aXYZ/z/t/amy/aXYZ/z/t", "f",
                "/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou]", "/amy/XYZ/Z/t/amy/XYZ/Z/t", "f",
                "/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou]", "/amy/XYZ/z/a/amy/XYZ/z/a", "f",
                "/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou]", "/amy/XYZ/zz/t/amy/XYZ/zz/t", "f",
                "/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou]", "/amy/XYZ/z/tr/amy/XYZ/z/tr", "f"

        };

        cout.copyfmt(init);
        cout << "+++ MATCH +++" << endl;
        for(size_t i = 0; i < sizeof(pats)/sizeof(pats[0]); i+=3) {
                bool res = oscc::core::util::isMatch(pats[i+1], pats[i]);
                bool expected = (pats[i + 2] == "t");
                cout << ((res == expected) ? "OK     " : "ERROR  ") << pats[i+1] << " == " << pats[i] << "   -> " << res << "\t\t[" << i << "]" << endl;
        }

        return 0;
}

size_t memdiv(const char *arr1, const char *arr2, size_t size) {
        for (size_t i = 0; i < size; ++i)
                if(arr1[i] != arr2[i])
                        return i;
        return -1;
}
