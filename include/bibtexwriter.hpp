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
 * @brief Boost.Spirit v2 based BibTeX generator.
 * @file bibtexwriter.hpp
 * @author Sergiu Dotenco
 */

#ifndef BIBTEXWRITER_HPP
#define BIBTEXWRITER_HPP

#pragma once

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include "bibtexentry.hpp"

namespace bibtex {

template<class OutputIterator, class Skipper>
class BibTeXWriter
    : public boost::spirit::karma::grammar
    <
        OutputIterator,
        BibTeXEntry(),
        Skipper
    >
{
public:
    BibTeXWriter()
        : BibTeXWriter::base_type(start_, "single BibTeX entry")
    {
        using namespace boost::spirit;
        namespace ph = boost::phoenix;
        namespace enc = detail::encoding;

        escapedBrace.add
            ('{', "\\{")
            ('}', "\\}")
            ;

        tag_ = +enc::alnum;
        key_ = +~karma::char_(',');
        value_ = *(escapedBrace | ~enc::char_("{}"));

        values_
            = ('{' << value_ << '}') % " # ";
            ;

        field_
            = enc::string
            << " = "
            << values_
            ;

        fields_
            = (repeat(4)[' '] << field_) % (',' << karma::eol)
            ;

        generic_
            = '@' << tag_ <<
            '{'
                << -key_ << ',' << karma::eol
                << fields_ << karma::eol <<
            '}'
            ;

        simple_
            =
            &
            (
                enc::string("comment")
                |
                enc::string("include")
                |
                enc::string("preamble")
            )
            [
                _1 = ph::bind(tolower, ph::at_c<0>(_val))
            ]
            <<
            '@'
            <<
            enc::string
            [
                _1 = ph::at_c<0>(_val)
            ]
            <<
            '{'
                << values_
                [
                    _1 = ph::bind(&KeyValue::second,
                        ph::front(ph::at_c<2>(_val)))
                ]
                <<
            '}'
            ;

        string_
            =
            &enc::string("string")
            [
                _1 = ph::bind(tolower, ph::at_c<0>(_val))
            ]
            <<
            '@'
            <<
            enc::string
            [
                _1 = ph::at_c<0>(_val)
            ]
            <<
            '{'
                << field_
                [
                    ph::let(ph::local_names::_a = ph::front(ph::at_c<2>(_val)))
                    [
                        _1 = ph::construct<KeyValue>
                            (ph::bind(&KeyValue::first, ph::local_names::_a),
                            ph::bind(&KeyValue::second, ph::local_names::_a))
                    ]
                ]
                <<
            '}'
            ;

        start_
            = string_
            | simple_
            | generic_
            ;
    }

    static std::string tolower(const std::string& in)
    {
        return boost::algorithm::to_lower_copy(in);
    }

private:
    boost::spirit::karma::rule<OutputIterator, BibTeXEntry(), Skipper> start_;
    boost::spirit::karma::rule<OutputIterator, BibTeXEntry(), Skipper> generic_;
    boost::spirit::karma::rule<OutputIterator, BibTeXEntry(), Skipper> simple_;
    boost::spirit::karma::rule<OutputIterator, BibTeXEntry()> string_;
    boost::spirit::karma::rule<OutputIterator, std::string()> tag_;
    boost::spirit::karma::rule<OutputIterator, std::string()> key_;
    boost::spirit::karma::rule<OutputIterator, std::string()> value_;
    boost::spirit::karma::rule<OutputIterator, ValueVector(), Skipper>
        values_;
    boost::spirit::karma::rule<OutputIterator, KeyValue(), Skipper>
        field_;
    boost::spirit::karma::rule<OutputIterator, KeyValueVector(), Skipper>
        fields_;
    boost::spirit::karma::symbols<char, const char*> escapedBrace;
};

template<class OutputIterator, class Range, class Skipper>
inline bool write(OutputIterator out, const Range& range, Skipper& skipper,
    typename boost::enable_if<boost::has_range_iterator<Range> >::type*
        /*dummy*/ = NULL)
{
    BibTeXWriter<OutputIterator, Skipper> writer;
    return boost::spirit::karma::generate_delimited(out, *writer,
        boost::spirit::karma::eol, range);
}

template<class OutputIterator, class Range>
inline bool write(OutputIterator out, const Range& range,
    typename boost::enable_if<boost::has_range_iterator<Range> >::type*
        /*dummy*/ = NULL)
{
    return write(out, range, space);
}

} // namespace bibtex

#endif // BIBTEXWRITER_HPP
