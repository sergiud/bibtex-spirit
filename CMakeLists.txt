cmake_minimum_required (VERSION 2.8)
project (bibtex-spirit)

option (Boost_USE_STATIC_LIBS "Use static Boost libraries" ON)

enable_testing ()

find_package (Boost 1.46 REQUIRED unit_test_framework)

# Do not link Boost libraries automatically
add_definitions (-DBOOST_ALL_NO_LIB)

include_directories (${Boost_INCLUDE_DIRS})
include_directories (include)

add_subdirectory (tests)
