#define BOOST_TEST_MODULE BibTeX

#include <boost/algorithm/string/predicate.hpp>
#include <boost/range.hpp>
#include <boost/test/unit_test.hpp>

#include "bibtex.hpp"

using bibtex::BibTeXEntry;
using boost::algorithm::iequals;

BOOST_AUTO_TEST_CASE(structure)
{
    BibTeXEntry e;

    const char test[] =
        "@article{boa12,"
        "   a = {b asd asd adas das },"
        "   c = {d asd adasd a},"
        "   d = {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    BOOST_CHECK(parse(boost::make_iterator_range(test), e));

    BOOST_CHECK(iequals(e.tag, "article"));

    BOOST_CHECK(!!e.key);
    BOOST_CHECK_EQUAL(*e.key, "boa12");

    BOOST_REQUIRE_EQUAL(e.entries.size(), 4);

    BOOST_CHECK_EQUAL(e.entries[0].second, "b asd asd adas das ");
    BOOST_CHECK_EQUAL(e.entries[0].first, "a");

    BOOST_CHECK_EQUAL(e.entries[1].first, "c");
    BOOST_CHECK_EQUAL(e.entries[1].second, "d asd adasd a");

    BOOST_CHECK_EQUAL(e.entries[2].first, "d");
    BOOST_CHECK_EQUAL(e.entries[2].second, "d asd } adasd a");

    BOOST_CHECK_EQUAL(e.entries[3].first, "d123");
    BOOST_CHECK_EQUAL(e.entries[3].second, "test test\" aa");
}

BOOST_AUTO_TEST_CASE(newline_comment_1)
{
    BibTeXEntry e;

    const char test[] =
        "%\n@book{abc1,"
        "   a = {b asd asd\nsecond line\nthird line adas das },\n"
        "  %c = {d asd adasd a},\n"
        "   d = {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    BOOST_CHECK(parse(boost::make_iterator_range(test), e));

    BOOST_CHECK(iequals(e.tag, "book"));

    BOOST_CHECK(!!e.key);
    BOOST_CHECK_EQUAL(*e.key, "abc1");

    BOOST_REQUIRE_EQUAL(e.entries.size(), 3);

    BOOST_CHECK_EQUAL(e.entries[0].first, "a");
    BOOST_CHECK_EQUAL(e.entries[0].second,
        "b asd asd\nsecond line\nthird line adas das ");

    BOOST_CHECK_EQUAL(e.entries[1].first, "d");
    BOOST_CHECK_EQUAL(e.entries[1].second, "d asd } adasd a");

    BOOST_CHECK_EQUAL(e.entries[2].first, "d123");
    BOOST_CHECK_EQUAL(e.entries[2].second, "test test\" aa");
}

BOOST_AUTO_TEST_CASE(newline_comment_2)
{
    BibTeXEntry e;

    const char test[] =
        "%\n@book{abc1,"
        "   a = {b asd asd\nsecond line\nthird line adas das },\n"
        "   c = %{d asd adasd a}, d =\n {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    BOOST_CHECK(parse(boost::make_iterator_range(test), e));

    BOOST_CHECK(iequals(e.tag, "book"));

    BOOST_CHECK(!!e.key);
    BOOST_CHECK_EQUAL(*e.key, "abc1");

    BOOST_REQUIRE_EQUAL(e.entries.size(), 3);

    BOOST_CHECK_EQUAL(e.entries[0].first, "a");
    BOOST_CHECK_EQUAL(e.entries[0].second,
        "b asd asd\nsecond line\nthird line adas das ");

    BOOST_CHECK_EQUAL(e.entries[1].first, "c");
    BOOST_CHECK_EQUAL(e.entries[1].second, "d asd } adasd a");

    BOOST_CHECK_EQUAL(e.entries[2].first, "d123");
    BOOST_CHECK_EQUAL(e.entries[2].second, "test test\" aa");
}

BOOST_AUTO_TEST_CASE(missing_key)
{
    BibTeXEntry e;

    const char test[] =
        "@book{,"
        "   a = \"aasdasd adas d\""
        "}";

    BOOST_CHECK(parse(boost::make_iterator_range(test), e));

    BOOST_CHECK(iequals(e.tag, "book"));
    BOOST_CHECK(!e.key);

    BOOST_REQUIRE_EQUAL(e.entries.size(), 1);

    BOOST_CHECK_EQUAL(e.entries[0].first, "a");
    BOOST_CHECK_EQUAL(e.entries[0].second, "aasdasd adas d");
}
