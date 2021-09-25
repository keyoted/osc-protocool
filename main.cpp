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

        auto t = oscc::core::util::strToOSC(R"(#123[/path 1 2.0 1i 2f 3l 4d "5" '6' {7} x0194 :123 #01020304 .01030507 [1[2]3];/path2;#;#0;#10000[/path3; /path4 [3 4 5]]; /*/hello 5 4 "bye"])");

        return 0;
}

size_t memdiv(const char *arr1, const char *arr2, size_t size) {
        for (size_t i = 0; i < size; ++i)
                if(arr1[i] != arr2[i])
                        return i;
        return -1;
}
