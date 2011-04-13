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

/**
 * @brief Boost.Spirit v2 based BibTeX parser.
 * @file bibtex.hpp
 * @author Sergiu Dotenco
 */

#ifndef BIBTEX_HPP
#define BIBTEX_HPP

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/operators.hpp>
#include <boost/optional.hpp>
#include <boost/range.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/utility/enable_if.hpp>

#ifndef BOOST_SPIRIT_AUTO
#define BOOST_SPIRIT_AUTO(domain_, name, expr)                \
    typedef boost::proto::result_of::                         \
        deep_copy<BOOST_TYPEOF(expr)>::type name##_expr_type; \
    BOOST_SPIRIT_ASSERT_MATCH(                                \
        boost::spirit::domain_::domain, name##_expr_type);    \
    BOOST_AUTO(name, boost::proto::deep_copy(expr));
#endif // BOOST_SPIRIT_AUTO

namespace bibtex {

namespace detail {

namespace encoding = boost::spirit::standard;

} // namespace detail

typedef std::vector<std::string> ValueVector;
typedef std::pair<std::string,  ValueVector> KeyValue;
typedef std::vector<KeyValue> KeyValueVector;
typedef detail::encoding::space_type Space;

BOOST_SPIRIT_AUTO(qi, space, detail::encoding::space |
    '%' >> *(boost::spirit::qi::char_ - boost::spirit::qi::eol)
    >> boost::spirit::qi::eol)

/**
 * @brief Single BibTeX entry.
 */
struct BibTeXEntry
    : boost::equality_comparable<BibTeXEntry>
{
    //! Entry's tag.
    std::string tag;
    //! Entry's optional key.
    boost::optional<std::string> key;
    //! Entry's key/value pairs.
    KeyValueVector entries;
};

inline bool operator==(const BibTeXEntry& lhs, const BibTeXEntry& rhs)
{
    return lhs.tag == rhs.tag && lhs.tag == rhs.tag &&
        lhs.entries == rhs.entries;
}

template <class InputIterator, class Skipper>
class BibTeXParser
    : public boost::spirit::qi::grammar<InputIterator, BibTeXEntry(), Skipper>
{
public:
    BibTeXParser()
        : BibTeXParser::base_type(start_, "single BibTeX entry")
    {
        using namespace boost::spirit;
        namespace ph = boost::phoenix;
        namespace sn = detail::encoding;

        escapedBrace.add
            ("\\{", '{')
            ("\\}", '}')
            ;
        escapedQuote.add
            ("\\\"", '"')
            ;

        tag_ = +detail::encoding::alnum;
        entryKey_ = +(qi::char_ - ',');
        key_ = +~qi::char_("=,})");

        escape_
            =
            qi::char_('{')
            >>
                *((escapedBrace | qi::char_) - '}')
            >>
            qi::char_('}')
            ;

        quoted_
            = qi::lexeme
            [
                (
                    '"'
                    >>
                        *qi::as_string
                        [
                            +(escapedQuote | ~qi::char_("{\""))
                            |
                            escape_
                        ]
                    >>
                    '"'
                )
                |
                (
                    '{'
                    >>
                        *qi::as_string
                        [
                            +(escapedBrace | ~qi::char_("{}"))
                            |
                            escape_
                        ]
                    >>
                    '}'
                )
            ]
            [
                _val = ph::accumulate(_1, ph::construct<std::string>())
            ]
            ;

        value_
            = quoted_
            | +~qi::char_(",})#")
            ;

        values_
            = value_ % '#'
            ;

        entry_
            = key_ >> '=' >> values_
            ;

        entries_ = -(entry_ % ',');

        body_
            = -entryKey_ >> ',' // tolerate an empty key
            >> entries_
            >> -qi::lit(',') // accept a trailing comma
            ;

        generic_
            =
            '@' >> tag_
            [
                ph::at_c<0>(_val) = _1
            ]
            >>
            (
                ('{' >> body_ >> '}')
                |
                ('(' >> body_ >> ')')
            )
            [
                ph::at_c<1>(_val) = ph::at_c<0>(_1),
                ph::at_c<2>(_val) = ph::at_c<1>(_1)
            ]
            ;

        string_
            =
            '@' >> sn::no_case
            [
                qi::string("string")
                [
                    ph::at_c<0>(_val) = _1
                ]
            ]
            >>
            (
                ('{' >> entry_ >> '}')
                |
                ('(' >> entry_ >> ')')
            )
            [
                ph::assign(ph::bind(&BibTeXEntry::entries, _val), 1, _1)
            ]
            ;

        simple_
            =
            '@' >> sn::no_case
            [
                (
                    sn::string("comment")
                    |
                    sn::string("include")
                    |
                    sn::string("preamble")
                )
                [
                    ph::at_c<0>(_val) = _1
                ]
            ]
            >>
            (
                ('{' >> values_ >> '}')
                |
                ('(' >> values_ >> ')')
            )
            [
                ph::assign(ph::bind(&BibTeXEntry::entries, _val), 1,
                    ph::construct<KeyValue>(KeyValue::first_type(), _1))
            ]
            ;

        start_
            = string_
            | simple_
            | generic_
            ;
    }

private:
    typedef boost::spirit::qi::rule<InputIterator, std::string(), Skipper>
        StringRule;

    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> generic_;
    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> include_;
    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> simple_;
    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> start_;
    boost::spirit::qi::rule<InputIterator, BibTeXEntry(), Skipper> string_;
    boost::spirit::qi::rule<InputIterator, ValueVector(), Skipper> values_;
    boost::spirit::qi::rule<InputIterator,
        boost::fusion::vector<boost::optional<std::string>,
        KeyValueVector>(), Skipper> body_;
    boost::spirit::qi::rule<InputIterator, KeyValue(), Skipper> entry_;
    boost::spirit::qi::rule<InputIterator, KeyValueVector(), Skipper> entries_;
    boost::spirit::qi::symbols<char, char> escapedBrace;
    boost::spirit::qi::symbols<char, char> escapedQuote;
    StringRule quoted_;
    StringRule entryKey_;
    StringRule key_;
    StringRule tag_;
    StringRule value_;
    boost::spirit::qi::rule<InputIterator, std::string()> escape_;
};

template <class InputIterator, class Container, class Skipper>
class BibTeXContainerParser
    : public boost::spirit::qi::grammar<InputIterator, Container(), Skipper>
{
public:
    BibTeXContainerParser()
        : BibTeXContainerParser::base_type(start_, "multiple BibTeX entries")
    {
        start_ = *parser_;
    }

private:
    BibTeXParser<InputIterator, Skipper> parser_;
    boost::spirit::qi::rule<InputIterator, Container(), Skipper> start_;
};

template<class InputIterator, class Skipper>
inline bool parse(InputIterator first, InputIterator last, Skipper& skipper,
                  BibTeXEntry& entry)
{
    BibTeXParser<InputIterator, Skipper> parser;
    return boost::spirit::qi::phrase_parse(first, last, parser, skipper, entry);
}

template<class InputIterator, class Skipper, class Container>
inline bool parse(InputIterator first, InputIterator last, Skipper& skipper,
    Container& entries, boost::enable_if<boost::is_same<
        typename Container::value_type, BibTeXEntry> >* /*dummy*/ = NULL)
{
    BibTeXContainerParser<InputIterator, Container, Skipper> parser;
    return boost::spirit::qi::phrase_parse(first, last, parser, skipper,
        entries);
}

template<class SinglePassRange, class Skipper>
inline bool parse(SinglePassRange range, Skipper& skipper, BibTeXEntry& entry)
{
    return parse(boost::const_begin(range), boost::const_end(range), entry);
}

template<class SinglePassRange, class Skipper, class Container>
inline bool parse(SinglePassRange range, Skipper& skipper, Container& entries,
    boost::enable_if<boost::is_same<
        typename Container::value_type, BibTeXEntry> >* dummy = NULL)
{
    return parse(boost::const_begin(range), boost::const_end(range), entries);
}

template<class InputIterator>
inline bool parse(InputIterator first, InputIterator last, BibTeXEntry& entry)
{
    return parse(first, last, bibtex::space, entry);
}

template<class InputIterator, class Container>
inline bool parse(InputIterator first, InputIterator last, Container& entries,
    boost::enable_if<boost::is_same<
        typename Container::value_type, BibTeXEntry> >* /*dummy*/ = NULL)
{
    return parse(first, last, bibtex::space, entries);
}

template<class SinglePassRange>
inline bool parse(SinglePassRange range, BibTeXEntry& entry)
{
    return parse(range, bibtex::space, entry);
}

template<class SinglePassRange, class Container>
inline bool parse(SinglePassRange range, Container& entries)
{
    return parse(range, bibtex::space, entries);
}

} // namespace bibtex

BOOST_FUSION_ADAPT_STRUCT(
    bibtex::BibTeXEntry,
    (std::string, tag)
    (boost::optional<std::string>, key)
    (bibtex::KeyValueVector, entries)
)

#endif // BIBTEX_HPP
