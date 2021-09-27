#include <functional>
#include <iostream>
#include <type_traits>
#include "lib/oscc_util.hpp"

int main() {
        using namespace std;
        ios init(nullptr);
        init.copyfmt(cout);

        const string pats[] = {
                R"(/abc)", R"(/abc)", "t",                                          //
                R"(/abc)", R"(/abcd)", "f",                                         //
                R"(/abc)", R"(/dabc)", "f",                                         //
                R"(/ab?)", R"(/abc)", "t",                                          //
                R"(/a?c)", R"(/abc)", "t",                                          //
                R"(/?bc)", R"(/abc)", "t",                                          //
                R"(/ab?)", R"(/abic)", "f",                                         //
                R"(/a?c)", R"(/abic)", "f",                                         //
                R"(/?bc)", R"(/abic)", "f",                                         //
                                                                                    //
                R"(/???/*abc)", R"(/abc/xxxabc)", "t",                              //
                R"(/???/abc*)", R"(/abc/abcxxx)", "t",                              //
                R"(/???/*abc*)", R"(/abc/abcxxxabc)", "t",                          //
                R"(/???/*abc)", R"(/abc/xxxbc)", "f",                               //
                R"(/???/abc*)", R"(/abc/abxxx)", "f",                               //
                R"(/???/*abc*)", R"(/abc/xxxbxxx)", "f",                            //
                                                                                    //
                R"(/???/[abc])", R"(/abc/a)", "t",                                  //
                R"(/???/[abc])", R"(/abc/b)", "t",                                  //
                R"(/???/[abc])", R"(/abc/c)", "t",                                  //
                R"(/???/[abc])", R"(/abc/d)", "f",                                  //
                R"(/???/[a-z])", R"(/abc/a)", "t",                                  //
                R"(/???/[a-z])", R"(/abc/r)", "t",                                  //
                R"(/???/[a-z])", R"(/abc/z)", "t",                                  //
                R"(/???/[-a])", R"(/abc/-)", "t",                                   //
                R"(/???/[a-])", R"(/abc/-)", "t",                                   //
                R"(/???/[a-])", R"(/abc/a)", "t",                                   //
                R"(/???/[a-])", R"(/abc/b)", "f",                                   //
                                                                                    //
                R"(/???/[!abc])", R"(/abc/a)", "f",                                 //
                R"(/???/[!abc])", R"(/abc/b)", "f",                                 //
                R"(/???/[!abc])", R"(/abc/c)", "f",                                 //
                R"(/???/[!abc])", R"(/abc/d)", "t",                                 //
                R"(/???/[!a-z])", R"(/abc/a)", "f",                                 //
                R"(/???/[!a-z])", R"(/abc/r)", "f",                                 //
                R"(/???/[!a-z])", R"(/abc/z)", "f",                                 //
                R"(/???/[!-a])", R"(/abc/-)", "f",                                  //
                R"(/???/[!a-])", R"(/abc/-)", "f",                                  //
                R"(/???/[!a-])", R"(/abc/a)", "f",                                  //
                R"(/???/[!a-])", R"(/abc/b)", "t",                                  //
                                                                                    //
                R"({amy,lisa,hana}/???/[xyz]/[!aeiou])", R"(amy/XYZ/z/t)", "t",     //
                R"({amy,lisa,hana}/???/[xyz]/[!aeiou])", R"(lisa/dei/x/f)", "t",    //
                R"({amy,lisa,hana}/???/[xyz]/[!aeiou])", R"(hana/wie/y/s)", "t",    //
                R"({amy,lisa,hana}/???/[xyz]/[!aeiou])", R"(jame/XYZ/z/t)", "f",    //
                R"({amy,lisa,hana}/???/[xyz]/[!aeiou])", R"(amy/aXYZ/z/t)", "f",    //
                R"({amy,lisa,hana}/???/[xyz]/[!aeiou])", R"(amy/XYZ/Z/t)", "f",     //
                R"({amy,lisa,hana}/???/[xyz]/[!aeiou])", R"(amy/XYZ/z/a)", "f",     //
                R"({amy,lisa,hana}/???/[xyz]/[!aeiou])", R"(amy/XYZ/zz/t)", "f",    //
                R"({amy,lisa,hana}/???/[xyz]/[!aeiou])", R"(amy/XYZ/z/tr)", "f",    //
                                                                                    //
                R"(/abc/abc/abc)", R"(/abc/abc/abc)", "t",                          //
                R"(/abc/abc/abc)", R"(/abcd/abcd/abcd)", "f",                       //
                R"(/abc/abc/abc)", R"(/dabc/dabc/dabc)", "f",                       //
                R"(/ab?/ab?/ab?)", R"(/abc/abc/abc)", "t",                          //
                R"(/a?c/a?c/a?c)", R"(/abc/abc/abc)", "t",                          //
                R"(/?bc/?bc/?bc)", R"(/abc/abc/abc)", "t",                          //
                R"(/ab?/ab?/ab?)", R"(/abic/abic/abic)", "f",                       //
                R"(/a?c/a?c/a?c)", R"(/abic/abic/abic)", "f",                       //
                R"(/?bc/?bc/?bc)", R"(/abic/abic/abic)", "f",                       //
                R"(/???/*abc/???/*abc)", R"(/abc/xxxabc/abc/xxxabc)", "t",          //
                R"(/???/abc*/???/abc*)", R"(/abc/abcxxx/abc/abcxxx)", "t",          //
                R"(/???/*abc*/???/*abc*)", R"(/abc/abcxxxabc/abc/abcxxxabc)", "t",  //
                R"(/???/*abc/???/*abc)", R"(/abc/xxxbc/abc/xxxbc)", "f",            //
                R"(/???/abc*/???/abc*)", R"(/abc/abxxx/abc/abxxx)", "f",            //
                R"(/???/*abc*/???/*abc*)", R"(/abc/xxxbxxx/abc/xxxbxxx)", "f",      //
                R"(/???/[abc]/???/[abc])", R"(/abc/a/abc/a)", "t",                  //
                R"(/???/[abc]/???/[abc])", R"(/abc/b/abc/b)", "t",                  //
                R"(/???/[abc]/???/[abc])", R"(/abc/c/abc/c)", "t",                  //
                R"(/???/[abc]/???/[abc])", R"(/abc/d/abc/d)", "f",                  //
                R"(/???/[a-z]/???/[a-z])", R"(/abc/a/abc/a)", "t",                  //
                R"(/???/[a-z]/???/[a-z])", R"(/abc/r/abc/r)", "t",                  //
                R"(/???/[a-z]/???/[a-z])", R"(/abc/z/abc/z)", "t",                  //
                R"(/???/[-a]/???/[-a])", R"(/abc/-/abc/-)", "t",                    //
                R"(/???/[a-]/???/[a-])", R"(/abc/-/abc/-)", "t",                    //
                R"(/???/[a-]/???/[a-])", R"(/abc/a/abc/a)", "t",                    //
                R"(/???/[a-]/???/[a-])", R"(/abc/b/abc/b)", "f",                    //
                R"(/???/[!abc]/???/[!abc])", R"(/abc/a/abc/a)", "f",                //
                R"(/???/[!abc]/???/[!abc])", R"(/abc/b/abc/b)", "f",                //
                R"(/???/[!abc]/???/[!abc])", R"(/abc/c/abc/c)", "f",                //
                R"(/???/[!abc]/???/[!abc])", R"(/abc/d/abc/d)", "t",                //
                R"(/???/[!a-z]/???/[!a-z])", R"(/abc/a/abc/a)", "f",                //
                R"(/???/[!a-z]/???/[!a-z])", R"(/abc/r/abc/r)", "f",                //
                R"(/???/[!a-z]/???/[!a-z])", R"(/abc/z/abc/z)", "f",                //
                R"(/???/[!-a]/???/[!-a])", R"(/abc/-/abc/-)", "f",                  //
                R"(/???/[!a-]/???/[!a-])", R"(/abc/-/abc/-)", "f",                  //
                R"(/???/[!a-]/???/[!a-])", R"(/abc/a/abc/a)", "f",                  //
                R"(/???/[!a-]/???/[!a-])", R"(/abc/b/abc/b)", "t",                  //
                R"(/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou])",
                R"(/amy/XYZ/z/t/amy/XYZ/z/t)",
                "t",  //
                R"(/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou])",
                R"(/lisa/dei/x/f/lisa/dei/x/f)",
                "t",  //
                R"(/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou])",
                R"(/hana/wie/y/s/hana/wie/y/s)",
                "t",  //
                R"(/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou])",
                R"(/jame/XYZ/z/t/jame/XYZ/z/t)",
                "f",  //
                R"(/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou])",
                R"(/amy/aXYZ/z/t/amy/aXYZ/z/t)",
                "f",  //
                R"(/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou])",
                R"(/amy/XYZ/Z/t/amy/XYZ/Z/t)",
                "f",  //
                R"(/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou])",
                R"(/amy/XYZ/z/a/amy/XYZ/z/a)",
                "f",  //
                R"(/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou])",
                R"(/amy/XYZ/zz/t/amy/XYZ/zz/t)",
                "f",  //
                R"(/{amy,lisa,hana}/???/[xyz]/[!aeiou]/{amy,lisa,hana}/???/[xyz]/[!aeiou])",
                R"(/amy/XYZ/z/tr/amy/XYZ/z/tr)",
                "f"  //

        };

        cout.copyfmt(init);
        cout << "+++ MATCH +++" << endl;
        for (size_t i = 0; i < sizeof(pats) / sizeof(pats[0]); i += 3) {
                bool res      = oscc::util::isMatch(pats[i + 1], pats[i]);
                bool expected = (pats[i + 2] == "t");
                cout << ((res == expected) ? "OK     " : "ERROR  ") << pats[i + 1] << " == " << pats[i] << "   -> "
                     << res << "\t\t[" << i << "]" << endl;
        }

        auto t = oscc::util::strToOSC(
                R"(#1230[/path 1 2.0 1i 2f 3l 4d "5" '6' {7} x0194 :123 #01020304 .01030507 [1[2]3];/path2;#;#0;#10000[/path3; /path4 [3 4 5]]; /*/hello 5 4 "bye"])");
        cout.copyfmt(init);
        cout << t.string() << endl;
        return 0;
}