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
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/**
 * @brief Boost.Spirit based BibTeX parser.
 * @file bibtexreader.hpp
 * @author Sergiu Dotenco
 * @version 0.1
 */

#ifndef BIBTEXREADER_HPP
#define BIBTEXREADER_HPP

#pragma once

#include <istream>
#include <string>

#include <boost/fusion/include/vector.hpp>
#include <boost/io/ios_state.hpp>
#include <boost/mpl/and.hpp>
#include <boost/range.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/utility/enable_if.hpp>

#include "bibtexentry.hpp"

namespace bibtex {

/**
 * @brief Single BibTeX entry reader.
 *
 * @tparam ForwardIterator Input sequence iterator type.
 * @tparam Skipper Skipper type.
 */
template <class ForwardIterator, class Skipper>
class BibTeXReader
    : public boost::spirit::qi::grammar<ForwardIterator, BibTeXEntry(), Skipper>
{
public:
    BibTeXReader()
        : BibTeXReader::base_type(start_, "single BibTeX entry")
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
        entryKey_ = qi::lexeme[+(~qi::char_(',') - sn::space)];
        key_ = qi::lexeme[+(~qi::char_("=,})") - sn::space)];

        escapedText_
            = !qi::lit('{') >> +(escapedBrace | ~qi::char_("{}"))
            ;

        quoteText_
            = +(escapedQuote | ~qi::char_("\"{}"))
            ;

        innerBraceText_
            %=
            (
                qi::char_('{')
                >>
                    *(innerBraceText_ | escapedText_)
                >>
                qi::char_('}')
            )
            [
                _val = _1 + ph::accumulate(_2, ph::construct<std::string>()) +
                    _3
            ]
            |
            escapedText_
            ;

        innerQuoteText_
            %=
            (
                qi::char_('{')
                >>
                    *(innerQuoteText_ | escapedText_)
                >>
                qi::char_('}')
            )
            [
                _val = _1 + ph::accumulate(_2, ph::construct<std::string>()) +
                    _3
            ]
            |
            quoteText_
            ;

        quoted_
            = qi::lexeme
            [
                ( '"' >> *innerQuoteText_ >> '"' )
                |
                ( '{' >> *innerBraceText_ >> '}' )
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

        field_
            = key_ >> '=' >> values_
            ;

        fields_ = -(field_ % ',');

        body_
            = -entryKey_ >> ',' // tolerate an empty key
            >> fields_
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
                ('{' >> field_ >> '}')
                |
                ('(' >> field_ >> ')')
            )
            [
                ph::assign(ph::bind(&BibTeXEntry::fields, _val), 1, _1)
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
                ph::assign(ph::bind(&BibTeXEntry::fields, _val), 1,
                    ph::construct<KeyValue>
                        (ph::construct<KeyValue::first_type>(), _1))
            ]
            ;

        entry_
            = string_
            | simple_
            | generic_
            ;

        junk_
            = *~qi::lit('@')
            ;

        start_
            = junk_ >> entry_
            ;
    }

private:
    typedef boost::spirit::qi::rule<ForwardIterator, std::string(), Skipper>
        StringRule;
    typedef boost::spirit::qi::rule<ForwardIterator, std::string()>
        SimpleStringRule;

    boost::spirit::qi::rule<ForwardIterator, BibTeXEntry(), Skipper> generic_;
    boost::spirit::qi::rule<ForwardIterator, BibTeXEntry(), Skipper> include_;
    boost::spirit::qi::rule<ForwardIterator, BibTeXEntry(), Skipper> simple_;
    boost::spirit::qi::rule<ForwardIterator, BibTeXEntry(), Skipper> start_;
    boost::spirit::qi::rule<ForwardIterator, BibTeXEntry(), Skipper> string_;
    boost::spirit::qi::rule<ForwardIterator, BibTeXEntry(), Skipper> entry_;
    boost::spirit::qi::rule<ForwardIterator, ValueVector(), Skipper> values_;
    boost::spirit::qi::rule<ForwardIterator, Skipper> junk_;
    boost::spirit::qi::rule<ForwardIterator,
        boost::fusion::vector<boost::optional<std::string>,
        KeyValueVector>(), Skipper> body_;
    boost::spirit::qi::rule<ForwardIterator, KeyValue(), Skipper> field_;
    boost::spirit::qi::rule<ForwardIterator, KeyValueVector(), Skipper>
        fields_;
    boost::spirit::qi::symbols<char, char> escapedBrace;
    boost::spirit::qi::symbols<char, char> escapedQuote;
    StringRule quoted_;
    StringRule entryKey_;
    StringRule key_;
    StringRule tag_;
    StringRule value_;
    SimpleStringRule innerBraceText_;
    SimpleStringRule escapedText_;
    SimpleStringRule innerQuoteText_;
    SimpleStringRule quoteText_;
};

template<class ForwardIterator, class Skipper>
inline bool read(ForwardIterator first, ForwardIterator last, Skipper& skipper,
                  BibTeXEntry& entry)
{
    BibTeXReader<ForwardIterator, Skipper> parser;
    return boost::spirit::qi::phrase_parse(first, last, parser, skipper, entry);
}

template<class ForwardIterator, class Skipper, class Container>
inline bool read(ForwardIterator first, ForwardIterator last, Skipper& skipper,
    Container& entries, boost::enable_if<boost::is_same<
        typename Container::value_type, BibTeXEntry> >* /*dummy*/ = NULL)
{
    BibTeXReader<ForwardIterator, Skipper> parser;
    return boost::spirit::qi::phrase_parse(first, last, *parser, skipper,
        entries);
}

template<class ForwardRange, class Skipper>
inline bool read(const ForwardRange& range, Skipper& skipper,
                 BibTeXEntry& entry)
{
    return read(boost::const_begin(range), boost::const_end(range), entry);
}

template<class ForwardRange, class Skipper, class Container>
inline bool read(const ForwardRange& range, Skipper& skipper,
    Container& entries, boost::enable_if<boost::is_same<
        typename Container::value_type, BibTeXEntry> >* /*dummy*/ = NULL)
{
    return read(boost::const_begin(range), boost::const_end(range), entries);
}

template<class ForwardIterator>
inline bool read(ForwardIterator first, ForwardIterator last,
                 BibTeXEntry& entry)
{
    return read(first, last, bibtex::space, entry);
}

template<class ForwardIterator, class Container>
inline bool read(ForwardIterator first, ForwardIterator last,
    Container& entries, boost::enable_if<boost::is_same<
        typename Container::value_type, BibTeXEntry> >* /*dummy*/ = NULL)
{
    return read(first, last, bibtex::space, entries);
}

template<class ForwardRange>
inline bool read(const ForwardRange& range, BibTeXEntry& entry,
    typename boost::enable_if<
        boost::has_range_iterator<ForwardRange> >::type* /*dummy*/ = NULL)
{
    return read(range, bibtex::space, entry);
}

template<class ForwardRange, class Container>
inline bool read(const ForwardRange& range, Container& entries,
    typename boost::enable_if<
        boost::has_range_iterator<ForwardRange> >::type* /*dummy*/ = NULL)
{
    return read(range, bibtex::space, entries);
}

template<class E, class T>
inline bool read(std::basic_istream<E, T>& in, BibTeXEntry& e)
{
    boost::io::ios_flags_saver saver(in);
    in.unsetf(std::ios_base::skipws);

    typedef boost::spirit::basic_istream_iterator<E> istream_iterator;
    return read(istream_iterator(in), istream_iterator(), e);
}

template<class E, class T, class Container>
inline bool read(std::basic_istream<E, T>& in, Container& entries)
{
    boost::io::ios_flags_saver saver(in);
    in.unsetf(std::ios_base::skipws);

    typedef boost::spirit::basic_istream_iterator<E> istream_iterator;
    return read(istream_iterator(in), istream_iterator(), entries);
}

template<class E, class T>
inline std::basic_istream<E, T>& operator>>(std::basic_istream<E, T>& in,
    BibTeXEntry& entry)
{
    read(in, entry);
    return in;
}

template<class E, class T, class Range>
inline typename boost::enable_if<
    boost::mpl::and_<
        boost::has_range_iterator<Range>,
        boost::is_same<typename Range::value_type, BibTeXEntry>
    >, std::basic_istream<E, T>
>::type& operator>>(std::basic_istream<E, T>& in, Range& entries)
{
    read(in, entries);
    return in;
}

} // namespace bibtex

#endif // BIBTEXREADER_HPP
