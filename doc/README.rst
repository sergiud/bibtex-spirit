.. -*- restructuredtext -*-

=============
bibtex-spirit
=============

-------------------------
BibTeX C++ parser library
-------------------------

:Author:    Sergiu Dotenco
:Date:      April 17, 2011
:Copyright: This document is placed in the public domain.

.. contents::


What is bibtex-spirit?
======================

bibtex-spirit is a header-only C++ BibTeX_ parser library based on Boost.Spirit_
and released under the MIT license. The latest version can be found at
https://bitbucket.org/sergiu/bibtex-spirit.

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

in the command prompt, ``<dir>`` being the bibtex-spirit's source directory.
CMake will generate the appropriate configuration that can be used to compile
the tests using make/nmake or inside an IDE.

.. _Boost:
  http://www.boost.org/

.. _CMake:
  http://www.cmake.org/


Using the library
=================

Reading BibTeX files is really easy. Just open an input stream and pass it to
the ``read`` function together with a container of ``bibtex::BibTeXEntry``
objects as shown below. After the call the container will contain all the
entries that have been successfully read.

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
``read`` function will return ``false``.

.. code-block:: c++

  bool parsed = read(text, e);

  if (!parsed) {
      // bibliography entry contains a syntax error
  }


.. [#] Iterators passed to the ``read`` function have to be *forward* iterators.

.. vi: sw=2 ts=2 tw=80 et ft=rst fenc=utf-8
