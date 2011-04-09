#include <string>

#include "bibtex.hpp"

int main()
{
    const std::string test =
        "@article{boa12,"
        "   a = {b asd asd adas das },"
        "   c = {d asd adasd a},"
        "   d = {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    const std::string test2 =
        "@article{abcd1a,"
        "   a = {b asd asd adas das },"
        "   c = {d asd adasd a},"
        "   d = {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    const std::string test3 =
        "@journal{aaad1,"
        "   a = {b asd asd\nsecond line\nthird line adas das },"
        "   c = {d asd adasd a},"
        "   d = {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    const std::string test4 =
        "%\n@book{abc1,"
        "   a = {b asd asd\nsecond line\nthird line adas das },\n"
        "   c = {d asd adasd a},"
        "   d = {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    const std::string test5 =
        "%\n@ article{aaa2a,"
        "   a = {b asd asd\nsecond line\nthird line adas das },\n"
        "   c = {d asd adasd a},"
        "   d = {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    const std::string test6 =
        "@include{"
        "   b asd asd\nsecond line\nthird line adas das \\},\n"
        "}";

    const std::string test7 =
        "@book{abcd,"
        "   a = \"aasdasd adas d\""
        "}";

    using bibtex::parse;

    parse(test7.begin(), test7.end());
    parse(test6.begin(), test6.end());
    parse(test5.begin(), test5.end());
    parse(test4.begin(), test4.end());
    parse(test.begin(), test.end());
    parse(test2.begin(), test2.end());
    parse(test3.begin(), test3.end());
}
