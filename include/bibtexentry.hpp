// Copyright (c) 2012 Sergiu Dotenco
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
 * @brief BibTeX entry.
 * @file bibtexentry.hpp
 * @author Sergiu Dotenco
 * @version 0.1
 */

#ifndef BIBTEXENTRY_HPP
#define BIBTEXENTRY_HPP

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/operators.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/typeof/typeof.hpp>

#ifndef BOOST_SPIRIT_AUTO
#define BOOST_SPIRIT_AUTO(domain_, name, expr)                \
    typedef boost::proto::result_of::                         \
        deep_copy<BOOST_TYPEOF(expr)>::type name##_expr_type; \
    BOOST_SPIRIT_ASSERT_MATCH(                                \
        boost::spirit::domain_::domain, name##_expr_type);    \
    BOOST_AUTO(const name, boost::proto::deep_copy(expr));
#endif // BOOST_SPIRIT_AUTO

namespace bibtex {

namespace detail {

namespace encoding = boost::spirit::standard_wide;

} // namespace detail

typedef std::vector<std::string> ValueVector;
typedef std::pair<std::string, ValueVector> KeyValue;
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
    KeyValueVector fields;
};

inline bool operator==(const BibTeXEntry& lhs, const BibTeXEntry& rhs)
{
    return lhs.tag == rhs.tag && lhs.tag == rhs.tag &&
        lhs.fields == rhs.fields;
}

} // namespace bibtex

BOOST_FUSION_ADAPT_STRUCT(
    bibtex::BibTeXEntry,
    (std::string, tag)
    (boost::optional<std::string>, key)
    (bibtex::KeyValueVector, fields)
)

#endif // BIBTEXENTRY_HPP
