.. -*- restructuredtext -*-

bibtex-spirit
=============

.. image:: https://travis-ci.org/sergiud/bibtex-spirit.svg?branch=master
   :target: https://travis-ci.org/sergiud/bibtex-spirit
.. image:: https://coveralls.io/repos/github/sergiud/bibtex-spirit/badge.svg?branch=master
   :target: https://coveralls.io/github/sergiud/bibtex-spirit?branch=master
.. image:: https://ci.appveyor.com/api/projects/status/lsf2a69373gl335r?svg=true
   :alt: Build status
   :target: https://ci.appveyor.com/project/sergiud/bibtex-spirit
.. image:: https://codecov.io/gh/sergiud/bibtex-spirit/branch/master/graph/badge.svg
   :target: https://codecov.io/gh/sergiud/bibtex-spirit
.. image:: https://api.codacy.com/project/badge/Grade/04c4cda4e559482c916befe75f0e75c7
   :alt: Codacy Badge
   :target: https://app.codacy.com/manual/sergiud/bibtex-spirit?utm_source=github.com&utm_medium=referral&utm_content=sergiud/bibtex-spirit&utm_campaign=Badge_Grade_Dashboard
.. image:: https://img.shields.io/lgtm/alerts/g/sergiud/bibtex-spirit.svg?logo=lgtm&logoWidth=18
   :alt: Total alerts
   :target: https://lgtm.com/projects/g/sergiud/bibtex-spirit/alerts/
.. image:: https://img.shields.io/lgtm/grade/cpp/g/sergiud/bibtex-spirit.svg?logo=lgtm&logoWidth=18
   :alt: Language grade: C/C++
   :target: https://lgtm.com/projects/g/sergiud/bibtex-spirit/context:cpp


This repository provides an implementation of a C++ BibTeX parser based on Boost.Spirit.

Getting started
===============

bibtex-spirit requires the Boost C++ Libraries 1.58. Previous versions might
work as well. To compile the tests make sure first CMake 3.5 is installed, then
enter ::

  $ cmake . -DCMAKE_BUILD_TYPE=Release

in your terminal or command prompt on Windows inside project's directory to
generate the appropriate configuration that can be used to compile the tests
using make/nmake or inside an IDE.

.. vi: ft=rst
