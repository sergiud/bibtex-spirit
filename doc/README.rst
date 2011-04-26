.. -*- restructuredtext -*-

=============
bibtex-spirit
=============

-------------------------
BibTeX C++ parser library
-------------------------

:Author:    Sergiu Dotenco
:Date:      April 23, 2011
:Version:   0.1
:Copyright: This document is placed in the public domain.

.. contents::


What is bibtex-spirit?
======================

bibtex-spirit is a header-only C++ BibTeX_ parser library based on Boost.Spirit_
and released under the MIT license. The latest version of the library can be
found at https://bitbucket.org/sergiu/bibtex-spirit.

.. _BibTeX:
  http://en.wikipedia.org/wiki/BibTeX

.. _Boost.Spirit:
  http://boost-spirit.com/home/


Getting started
===============

bibtex-spirit requires at least version 1.46.1 of Boost_ C++ Libraries, although
previous versions might work as well. To compile the tests make sure CMake_ 2.8
is installed first, then enter ::

  $ cmake <dir> -DCMAKE_BUILD_TYPE=Release

in the command prompt, ``<dir>`` being bibtex-spirit's source directory.  CMake
will generate the appropriate configuration that can be used to compile the
tests using make/nmake or inside an IDE.

.. _Boost:
  http://www.boost.org/

.. _CMake:
  http://www.cmake.org/


Using the library
=================

To use the library just copy the headers from the ``include`` directory to a
location, where the compiler can find them.


Reading entries
---------------

Reading BibTeX bibliographies is easy. Simply pass an input stream to the
``read`` function together with a container of ``bibtex::BibTeXEntry`` objects
as shown below. After the call the container will contain all the entries that
have been successfully read.

.. code-block:: c++

  #include <fstream>
  #include <vector>

  #include "bibtexreader.hpp"

  int main()
  {
      std::ifstream in("test.bib");

      std::vector<bibtex::BibTeXEntry> entries;
      read(in, entries); // reads all BibTeX entries
  }


Instead of reading multiple entries at once, it's possible to read one BibTeX
entry after another. This can be achieved by passing a ``BibTeXEntry`` object to
``read`` instead of a container as second parameter:

.. code-block:: c++

  std::ifstream in("test.bib");

  bibtex::BibTeXEntry entry;
  read(in, entry); // reads at most one BibTeX entry

BibTeX entries can not only be read from an ``std::ifstream`` but from every
type of input streams and even from iterator ranges [#]_. This allows, for
instance, to parse bibliographies that are located in memory instead of in a
file:

.. _string-example:

.. code-block:: c++

  #include <string>
  // ...
  const std::string text = "@string{IEEE = {IEEE Computer Society}}";

  bibtex::BibTeXEntry e;
  read(text.begin(), text.end(), e); // reads the BibTeX entry from the string

The above ``read`` call can be shortened using

.. code-block:: c++

  read(text, e);

To indicate that the bibliography being parsed contains syntax errors the
``read`` function will return ``false``. For instance, unexpected characters
between bibliography entries (TeX comments excluded) are treated as syntax
errors.

.. code-block:: c++

  bool parsed = read(text, e);

  if (!parsed) {
      // bibliography entry contains a syntax error
  }


The BibTeXEntry structure
-------------------------

The members of the ``BibTeXEntry`` structure defined in the ``bibtex`` namespace
as |BibTeXEntry|

.. |BibTeXEntry| code-block:: c++

    struct BibTeXEntry
    {
        std::string tag;
        boost::optional<std::string> key;
        KeyValueVector fields;
    };


have the following meanings.

tag
  The tag of a BibTeX entry that identifies its type. For example, the tag of
  the entry ::

    @string{IEEE = {IEEE Computer Society}}

  seen previously has the value ``string``.

key
  *Optional* entry key. BibTeX entry types ``string``, ``preamble`` and
  ``comment``, for instance, don't have a key.

fields
  A vector of key/value pairs that identify the fields of a BibTeX entry. The
  key contains the name of a field and the value a vector of strings. In most
  cases the value vector will contain just a single element.  Multiple value
  strings can occur if field's value consists of several strings separated by
  ``#``, as in::

    month = jan # ", " # feb


.. [#] Iterators passed to the ``read`` function have to be *forward* iterators.


Writing entries
---------------


Reference
=========

.. code-block:: c++

  typedef std::vector<std::string> ValueVector;
  typedef std::pair<std::string, ValueVector> KeyValue;
  typedef std::vector<KeyValue> KeyValueVector;

|BibTeXEntry|

.. code-block:: c++

  bool operator==(const BibTeXEntry& lhs, const BibTeXEntry& rhs);
  bool operator!=(const BibTeXEntry& lhs, const BibTeXEntry& rhs);


Reading entries
---------------

.. code-block:: c++

  // Single entries

  template<class ForwardIterator>
  bool read(ForwardIterator first, ForwardIterator last, BibTeXEntry& e);

  template<class ForwardRange>
  bool read(const ForwardRange& range, BibTeXEntry& e);

  template<class E, class T>
  bool read(std::basic_istream<E, T>& in, BibTeXEntry& e);

  template<class E, class T>
  std::basic_istream<E, T>& operator>>(std::basic_istream<E, T>& in, BibTeXEntry& entry);

  // Multiple entries

  template<class ForwardIterator, class Container>
  bool read(ForwardIterator first, ForwardIterator last, Container& entries);

  template<class ForwardRange, class Container>
  bool read(const ForwardRange& range, Container& entries);

  template<class E, class T, class Container>
  bool read(std::basic_istream<E, T>& in, Container& entries);

  template<class E, class T, class Container>
  std::basic_istream<E, T>& operator>>(std::basic_istream<E, T>& in, Container& entries);

Writing entries
---------------

.. code-block:: c++

  // Single entries

  template<class OutputIterator>
  bool write(OutputIterator out, const BibTeXEntry& e);

  template<class E, class T>
  bool write(std::basic_ostream<E, T>& out, const BibTeXEntry& e);

  template<class E, class T>
  std::basic_ostream<E, T>& operator<<(std::basic_ostream<E, T>& out, const BibTeXEntry& entry);

  // Multiple entries

  template<class OutputIterator, class Range>
  bool write(OutputIterator out, const Range& entries);

  template<class E, class T, class Range>
  bool write(std::basic_ostream<E, T>& out, const Range& entries);

  template<class E, class T, class Range>
  std::basic_ostream<E, T>& operator<<(std::basic_ostream<E, T>& out, const Range& entries);


.. vi: sw=2 ts=2 tw=80 et ft=rst fenc=utf-8 spell spelllang=en
