// Copyright (c) 2011 Sergiu Dotenco
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define BOOST_TEST_MODULE BibTeX

#include <vector>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/assign.hpp>
#include <boost/range.hpp>
#include <boost/test/unit_test.hpp>

#include "bibtex.hpp"

using bibtex::BibTeXEntry;
using boost::algorithm::iequals;

BOOST_AUTO_TEST_CASE(structure_1)
{
    BibTeXEntry e;

    const char test[] =
        "@article{boa:12,"
        "   a = {b asd asd adas das },"
        "   c = \"d asd { \" }adasd{ \" } a\","
        "   d = {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\","
        "   e = {b {asd} asd adas das }"
        "}";

    BOOST_REQUIRE(parse(boost::make_iterator_range(test), e));

    BOOST_CHECK(iequals(e.tag, "article"));

    BOOST_CHECK(!!e.key);
    BOOST_CHECK_EQUAL(*e.key, "boa:12");

    BOOST_REQUIRE_EQUAL(e.entries.size(), 5);

    BOOST_CHECK_EQUAL(e.entries[0].second.front(), "b asd asd adas das ");
    BOOST_CHECK_EQUAL(e.entries[0].first, "a");

    BOOST_CHECK_EQUAL(e.entries[1].first, "c");
    BOOST_CHECK_EQUAL(e.entries[1].second.front(), "d asd { \" }adasd{ \" } a");

    BOOST_CHECK_EQUAL(e.entries[2].first, "d");
    BOOST_CHECK_EQUAL(e.entries[2].second.front(), "d asd } adasd a");

    BOOST_CHECK_EQUAL(e.entries[3].first, "d123");
    BOOST_CHECK_EQUAL(e.entries[3].second.front(), "test test\" aa");

    BOOST_CHECK_EQUAL(e.entries[4].first, "e");
    BOOST_CHECK_EQUAL(e.entries[4].second.front(), "b {asd} asd adas das ");
}

BOOST_AUTO_TEST_CASE(structure_2)
{
    BibTeXEntry e;

    const char test[] =
        "@article(boa12,"
        "   a = {b asd asd adas das },"
        "   c = {d asd adasd a},"
        "   d = {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        ")";

    BOOST_REQUIRE(parse(boost::make_iterator_range(test), e));

    BOOST_CHECK(iequals(e.tag, "article"));

    BOOST_CHECK(!!e.key);
    BOOST_CHECK_EQUAL(*e.key, "boa12");

    BOOST_REQUIRE_EQUAL(e.entries.size(), 4);

    BOOST_CHECK_EQUAL(e.entries[0].second.front(), "b asd asd adas das ");
    BOOST_CHECK_EQUAL(e.entries[0].first, "a");

    BOOST_CHECK_EQUAL(e.entries[1].first, "c");
    BOOST_CHECK_EQUAL(e.entries[1].second.front(), "d asd adasd a");

    BOOST_CHECK_EQUAL(e.entries[2].first, "d");
    BOOST_CHECK_EQUAL(e.entries[2].second.front(), "d asd } adasd a");

    BOOST_CHECK_EQUAL(e.entries[3].first, "d123");
    BOOST_CHECK_EQUAL(e.entries[3].second.front(), "test test\" aa");
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

    BOOST_REQUIRE(parse(boost::make_iterator_range(test), e));

    BOOST_CHECK(iequals(e.tag, "book"));

    BOOST_CHECK(!!e.key);
    BOOST_CHECK_EQUAL(*e.key, "abc1");

    BOOST_REQUIRE_EQUAL(e.entries.size(), 3);

    BOOST_CHECK_EQUAL(e.entries[0].first, "a");
    BOOST_CHECK_EQUAL(e.entries[0].second.front(),
        "b asd asd\nsecond line\nthird line adas das ");

    BOOST_CHECK_EQUAL(e.entries[1].first, "d");
    BOOST_CHECK_EQUAL(e.entries[1].second.front(), "d asd } adasd a");

    BOOST_CHECK_EQUAL(e.entries[2].first, "d123");
    BOOST_CHECK_EQUAL(e.entries[2].second.front(), "test test\" aa");
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

    BOOST_REQUIRE(parse(boost::make_iterator_range(test), e));

    BOOST_CHECK(iequals(e.tag, "book"));

    BOOST_CHECK(!!e.key);
    BOOST_CHECK_EQUAL(*e.key, "abc1");

    BOOST_REQUIRE_EQUAL(e.entries.size(), 3);

    BOOST_CHECK_EQUAL(e.entries[0].first, "a");
    BOOST_CHECK_EQUAL(e.entries[0].second.front(),
        "b asd asd\nsecond line\nthird line adas das ");

    BOOST_CHECK_EQUAL(e.entries[1].first, "c");
    BOOST_CHECK_EQUAL(e.entries[1].second.front(), "d asd } adasd a");

    BOOST_CHECK_EQUAL(e.entries[2].first, "d123");
    BOOST_CHECK_EQUAL(e.entries[2].second.front(), "test test\" aa");
}

BOOST_AUTO_TEST_CASE(missing_key)
{
    BibTeXEntry e;

    const char test[] =
        "@book{,"
        "   a = \"aasdasd adas d\""
        "}";

    BOOST_REQUIRE(parse(boost::make_iterator_range(test), e));

    BOOST_CHECK(iequals(e.tag, "book"));
    BOOST_CHECK(!e.key);

    BOOST_REQUIRE_EQUAL(e.entries.size(), 1);

    BOOST_CHECK_EQUAL(e.entries[0].first, "a");
    BOOST_CHECK_EQUAL(e.entries[0].second.front(), "aasdasd adas d");
}

BOOST_AUTO_TEST_CASE(empty)
{
    BibTeXEntry e;

    const char test[] =
        "@book{,"
        "}";

    BOOST_REQUIRE(parse(boost::make_iterator_range(test), e));

    BOOST_CHECK(iequals(e.tag, "book"));
    BOOST_CHECK(!e.key);

    BOOST_REQUIRE(e.entries.empty());
}

BOOST_AUTO_TEST_CASE(comparison)
{
    BibTeXEntry e;

    const char test[] =
        "@book{,"
        "}";

    BOOST_REQUIRE(parse(boost::make_iterator_range(test), e));

    BOOST_CHECK(e == e);
}

BOOST_AUTO_TEST_CASE(special)
{
    BibTeXEntry e;

    const char test[] =
        "@string{b-test = \"asd asdasd a\" }";

    BOOST_REQUIRE(parse(boost::make_iterator_range(test), e));

    BOOST_CHECK_EQUAL(e.tag, "string");

    BOOST_REQUIRE_EQUAL(e.entries.size(), 1);
    BOOST_CHECK_EQUAL(e.entries[0].first, "b-test");
    BOOST_CHECK_EQUAL(e.entries[0].second.front(), "asd asdasd a");
}

BOOST_AUTO_TEST_CASE(multiple_1)
{
    std::vector<BibTeXEntry> e;

    const char test[] =
        "%\n@book{abc1,"
        "   a = {b asd asd\nsecond line\nthird line adas das },\n"
        "   c = %{d asd adasd a}, d =\n {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}"
        "\n"
        "%\n@book{abc2,"
        "   a = {b asd asd\nsecond line\nthird line adas das },\n"
        "   c = %{d asd adasd a}, d =\n {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    BOOST_REQUIRE(parse(boost::make_iterator_range(test), e));
    BOOST_REQUIRE_EQUAL(e.size(), 2);

    BOOST_CHECK(e[0] == e[1]);
}

BOOST_AUTO_TEST_CASE(multiple_2)
{
    std::vector<BibTeXEntry> e;

    const char test[] =
        "@book{abc1,"
        "   a = {b asd asd\nsecond line\nthird line adas das },\n"
        "   c = %{d asd adasd a}, d =\n {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\","
        "   bcd-e = ack,"
        "}"
        "@book{abc2,"
        "   a = {b asd asd\nsecond line\nthird line adas das },\n"
        "   c = %{d asd adasd a}, d =\n {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\","
        "   bcd-e = ack,"
        "}";

    BOOST_REQUIRE(parse(boost::make_iterator_range(test), e));
    BOOST_REQUIRE_EQUAL(e.size(), 2);

    BOOST_CHECK(e[0] == e[1]);
}

BOOST_AUTO_TEST_CASE(mixed_multiple)
{
    std::vector<BibTeXEntry> e;

    const char test[] =
        "%\n@book(abc1,"
        "   a = {b asd asd\nsecond line\nthird line adas das },\n"
        "   c = %{d asd adasd a}, d =\n {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        ")"
        "\n"
        "%\n@book{abc2,"
        "   a = {b asd asd\nsecond line\nthird line adas das },\n"
        "   c = %{d asd adasd a}, d =\n {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    BOOST_REQUIRE(parse(boost::make_iterator_range(test), e));
    BOOST_REQUIRE_EQUAL(e.size(), 2);

    BOOST_CHECK(e[0] == e[1]);
}

BOOST_AUTO_TEST_CASE(separated)
{
    BibTeXEntry e;

    const char test[] =
        "@book{abc1,"
        "   a = {b asd asd\nsecond line\nthird line adas das },\n"
        "   c = %{d asd adasd a}, d =\n {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\","
        "   bcd-e = ack,"
        "   more = abc # dfg # { , } # a,"
        "   normal = test,"
        "   last = \"last\" # { one}"
        "}";

    BOOST_REQUIRE(parse(boost::make_iterator_range(test), e));
    BOOST_REQUIRE_EQUAL(e.entries.size(), 7);

    const bibtex::ValueVector values1 =
        boost::assign::list_of("abc")("dfg")(" , ")("a");
    const bibtex::ValueVector values2 =
        boost::assign::list_of("last")(" one");

    BOOST_CHECK_EQUAL(e.entries[4].first, "more");
    BOOST_CHECK(e.entries[4].second == values1);
    BOOST_CHECK_EQUAL(e.entries[6].first, "last");
    BOOST_CHECK(e.entries[6].second == values2);
}
