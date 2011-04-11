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
