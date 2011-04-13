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

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include <boost/spirit/include/support_istream_iterator.hpp>

#include "bibtex.hpp"

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Not enough arguments" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream in(argv[1]);

    if (!in) {
        std::cerr << "Could not open " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    in.unsetf(std::ios_base::skipws);

    using bibtex::BibTeXEntry;
    using boost::spirit::istream_iterator;

    typedef std::vector<BibTeXEntry> BibTeXEntryVector;

    BibTeXEntryVector e;
    const BibTeXEntryVector::size_type expected = 1151;

    bool result = parse(istream_iterator(in), istream_iterator(), e);

    std::cout << "Found " << e.size() << " entries" << std::endl;

    return result && e.size() == expected ? EXIT_SUCCESS : EXIT_FAILURE;
}
