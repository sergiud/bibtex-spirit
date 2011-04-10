cmake_minimum_required (VERSION 2.8)
project (bibtex-spirit)

find_package (Boost 1.46 REQUIRED)

include_directories (${Boost_INCLUDE_DIRS})
include_directories (include)

add_subdirectory (tests)
