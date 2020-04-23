.. -*- restructuredtext -*-

.. image:: https://travis-ci.org/sergiud/bibtex-spirit.svg?branch=master
    :target: https://travis-ci.org/sergiud/bibtex-spirit
.. image:: https://coveralls.io/repos/github/sergiud/bibtex-spirit/badge.svg?branch=master
    :target: https://coveralls.io/github/sergiud/bibtex-spirit?branch=master

What is bibtex-spirit?
======================

*bibtex-spirit* is C++ BibTeX parser based on Boost.Spirit.

Getting started
===============

*bibtex-spirit* requires the Boost C++ Libraries 1.46. Previous versions might
work as well. To compile the tests make sure first CMake 2.8 is installed, then
enter ::

  $ cmake . -DCMAKE_BUILD_TYPE=Release

in your terminal or command prompt on Windows inside project's directory to
generate the appropriate configuration that can be used to compile the tests
using make/nmake or inside an IDE.

.. vi: ft=rst


.. image:: https://api.codacy.com/project/badge/Grade/04c4cda4e559482c916befe75f0e75c7
   :alt: Codacy Badge
   :target: https://app.codacy.com/manual/sergiud/bibtex-spirit?utm_source=github.com&utm_medium=referral&utm_content=sergiud/bibtex-spirit&utm_campaign=Badge_Grade_Dashboard