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

#include <fstream>
#include <iterator>
#include <vector>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/range.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/test/unit_test.hpp>

#include "bibtex.hpp"

int main(int argc, char** argv)
{
    using bibtex::BibTeXEntry;
    using boost::spirit::istream_iterator;

    std::ifstream in("test.bib");
    in.unsetf(std::ios_base::skipws);

    std::vector<BibTeXEntry> e;

    bool result = parse(istream_iterator(in), istream_iterator(), e);
}
