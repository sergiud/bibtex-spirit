#ifndef BIBTEX_HPP
#define BIBTEX_HPP

#pragma once

#include <string>
#include <utility>
#include <vector>

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

namespace detail {

namespace spirit_string = boost::spirit::standard;

} // namespace detail

BOOST_SPIRIT_AUTO(qi, space, detail::spirit_string::space |
    '%' >> *(boost::spirit::qi::char_ - boost::spirit::qi::eol)
    >> boost::spirit::qi::eol);

struct BibTeXEntry
{
    std::string tag;
    boost::optional<std::string> key;
    KeyValueVector entries;
};

template <class InputIterator, class Skipper>
struct BibTeXParser
    : boost::spirit::qi::grammar<InputIterator, BibTeXEntry(), Skipper>
{
    BibTeXParser()
        : BibTeXParser::base_type(start_)
    {
        using namespace boost::spirit;
        namespace ph = boost::phoenix;
        namespace sn = detail::spirit_string;

        escapedBrace.add("\\{", '{')("\\}", '}');
        escapedQuote.add("\\\"", '"');

        tag_ = +ascii::alnum;
        key_ = +ascii::alnum;

        braceValue_ = qi::lexeme
            [
                '{'
                    >> *(escapedBrace | qi::char_ - '}') >>
                '}'
            ];

        quotedValue_
            = qi::lexeme
            [
                '"'
                    >> *(escapedQuote | qi::char_ - '"') >>
                '"'
            ]
            | braceValue_
            ;

        value_ = +ascii::digit | quotedValue_;
        entry = key_ >> '=' >> value_;
        entries_ = entry >> +(',' >> entry);

        entryKey_ = +(qi::char_ - ',');

        generic_ =
            '@' >> tag_ >>
            '{'
                >> -entryKey_ >> ','// tolerate an empty key
                >> entries_
                >> -qi::lit(',') // accept a trailing comma
                >>
            '}'
            ;

        string_ =
            '@' >> sn::no_case
            [
                qi::string("string")
                [
                    ph::at_c<0>(_val) = _1
                ]
            ]
            >>
            '{'
                >> entry
                [
                    ph::assign(ph::bind(&BibTeXEntry::entries, _val), 1, _1)
                ]
                >>
            '}'
            ;

        simple_
            =
            '@' >> sn::no_case
            [
                (sn::string("comment") | sn::string("include"))
                [
                    ph::at_c<0>(_val) = _1
                ]
            ]
            >> braceValue_
            [
                ph::assign(ph::bind(&BibTeXEntry::entries, _val), 1,
                    ph::construct<KeyValue>(KeyValue::first_type(), _1))
            ]
            ;

        start_
            = string_
            | simple_
            | generic_;
    }

    typedef boost::spirit::qi::rule<InputIterator, std::string(), Skipper>
        StringRule;

    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> generic_;
    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> include_;
    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> simple_;
    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> start_;
    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> string_;
    boost::spirit::qi::rule<InputIterator, KeyValue(), Skipper> entry;
    boost::spirit::qi::rule<InputIterator, KeyValueVector(), Skipper> entries_;
    boost::spirit::qi::symbols<char, char> escapedBrace;
    boost::spirit::qi::symbols<char, char> escapedQuote;
    StringRule braceValue_;
    StringRule entryKey_;
    StringRule key_;
    StringRule quotedValue_;
    StringRule tag_;
    StringRule value_;

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
    return parse(boost::const_begin(range), boost::const_end(range), entry);
}

template<class InputIterator>
bool parse(InputIterator first, InputIterator last, BibTeXEntry& entry)
{
    return parse(first, last, bibtex::space, entry);
}

template<class SinglePassRange>
bool parse(SinglePassRange range, BibTeXEntry& entry)
{
    return parse(range, bibtex::space, entry);
}

} // namespace bibtex

BOOST_FUSION_ADAPT_STRUCT(
    bibtex::BibTeXEntry,
    (std::string, tag)
    (boost::optional<std::string>, key)
    (bibtex::KeyValueVector, entries)
)

#endif // BIBTEX_HPP
