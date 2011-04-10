#include <string>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/optional.hpp>
#include <boost/range.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/typeof/typeof.hpp>

#ifndef BOOST_SPIRIT_AUTO
#define BOOST_SPIRIT_AUTO(domain_, name, expr)                \
    typedef boost::proto::result_of::                         \
        deep_copy<BOOST_TYPEOF(expr)>::type name##_expr_type; \
    BOOST_SPIRIT_ASSERT_MATCH(                                \
        boost::spirit::domain_::domain, name##_expr_type);    \
    BOOST_AUTO(name, boost::proto::deep_copy(expr));
#endif // BOOST_SPIRIT_AUTO

namespace bibtex {

typedef std::pair<std::string, std::string> KeyValue;
typedef std::vector<KeyValue> KeyValueVector;
typedef boost::spirit::standard::space_type Space;

BOOST_SPIRIT_AUTO(qi, space, boost::spirit::ascii::space |
    '%' >> *(boost::spirit::qi::char_ - boost::spirit::qi::eol)
    >> boost::spirit::qi::eol);

struct BibTeXEntry
{
    std::string tag;
    std::string key;
    KeyValueVector entries;

    ~BibTeXEntry()
    {
    }
};

template <class InputIterator, class Skipper>
struct BibTeXParser
    : boost::spirit::qi::grammar<InputIterator, BibTeXEntry(), Skipper>
{
    BibTeXParser()
        : BibTeXParser::base_type(start)
    {
        using namespace boost::spirit;
        namespace ph = boost::phoenix;

        escapedBrace.add("\\{", '{')("\\}", '}');
        escapedQuote.add("\\\"", '"');

        tag = +ascii::alnum;
        key = +ascii::alnum;

        braceValue = qi::lexeme
            [
                '{'
                    >> *(escapedBrace | qi::char_ - '}') >>
                '}'
            ];

        quotedValue
            = qi::lexeme
            [
                '"'
                    >> *(escapedQuote | qi::char_ - '"') >>
                '"'
            ]
            | braceValue
            ;

        value = +ascii::digit | quotedValue;
        entry = key >> '=' >> value;
        entries = entry >> +(',' >> entry);

        entryKey = +(qi::char_ - ',');

        genericEntry = '@' >> tag >>
            '{'
                >> entryKey >> ','
                >> entries
                >> -qi::lit(',') // accept a trailing comma
                >>
            '}'
            ;

        stringEntry =
            '@' >> standard::no_case[qi::lit("string")] >>
            '{'
                >> entry
                [
                    ph::bind(&KeyValueVector::assign,
                        ph::bind(&BibTeXEntry::entries, qi::_val),
                            1, qi::_1)
                ]
                >>
            '}'
            ;

        includeEntry
            = '@' >> standard::no_case[qi::lit("include")]
            >> braceValue
            [
                ph::bind(&KeyValueVector::assign,
                    ph::bind(&BibTeXEntry::entries, qi::_val), 1,
                    ph::construct<KeyValue>(
                        KeyValue::first_type(), qi::_1))
            ]
            ;

        comment
            = '@' >> standard::no_case[qi::lit("comment")]
            >> braceValue
            [
                ph::bind(&KeyValueVector::assign,
                    ph::bind(&BibTeXEntry::entries, qi::_val), 1,
                    ph::construct<KeyValue>(
                        KeyValue::first_type(), qi::_1))
            ]
            ;

        start = stringEntry | includeEntry | comment | genericEntry;
    }

    typedef boost::spirit::qi::rule<InputIterator, std::string(), Skipper>
        StringRule;

    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> start;
    boost::spirit::qi::rule<InputIterator, KeyValue(), Skipper> entry;
    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> stringEntry;
    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> genericEntry;
    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> includeEntry;
    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> comment;
    boost::spirit::qi::rule<InputIterator, KeyValueVector(), Skipper> entries;
    boost::spirit::qi::symbols<char, char> escapedBrace;
    boost::spirit::qi::symbols<char, char> escapedQuote;
    StringRule key;
    StringRule entryKey;
    StringRule quotedValue;
    StringRule tag;
    StringRule value;
    StringRule braceValue;
};

template<class InputIterator, class Skipper>
bool parse(InputIterator first, InputIterator last, Skipper& skipper,
                 BibTeXEntry& entry)
{
    BibTeXParser<InputIterator, Skipper> parser;
    return boost::spirit::qi::phrase_parse(first, last, parser, skipper, entry);
}

template<class SinglePassRange, class Skipper>
bool parse(SinglePassRange range, Skipper& skipper, BibTeXEntry& entry)
{
    return parse(boost::begin(range), boost::end(range), entry);
}

template<class InputIterator>
bool parse(InputIterator first, InputIterator last)
{
    BibTeXEntry entry;
    return parse(first, last, bibtex::space, entry);
}

template<class SinglePassRange>
bool parse(SinglePassRange range)
{
    BibTeXEntry entry;
    return parse(range, bibtex::space, entry);
}

} // namespace bibtex

BOOST_FUSION_ADAPT_STRUCT(
    bibtex::BibTeXEntry,
    (std::string, tag)
    (std::string, key)
    (bibtex::KeyValueVector, entries)
)

int main()
{
    const std::string test =
        "@comment{boa12,"
        "   a = {b asd asd adas das },"
        "   c = {d asd adasd a},"
        "   d = {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    const std::string test2 =
        "@comment{"
        "   a = {b asd asd adas das },"
        "   c = {d asd adasd a},"
        "   d = {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    const std::string test3 =
        "@comment{"
        "   a = {b asd asd\nsecond line\nthird line adas das },"
        "   c = {d asd adasd a},"
        "   d = {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    const std::string test4 =
        "%\n@book{abc1"
        "   a = {b asd asd\nsecond line\nthird line adas das },\n"
        "   c = {d asd adasd a},"
        "   d = {d asd \\} adasd a},"
        "   d123 = \"test test\\\" aa\""
        "}";

    const std::string test5 =
        "%\n@ comment{"
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
        "   a = {aasdasd adas d}"
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
