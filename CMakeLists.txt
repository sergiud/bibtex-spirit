cmake_minimum_required (VERSION 2.8)
project (bibtex-spirit)

option (Boost_USE_STATIC_LIBS "Use static Boost libraries" ON)

enable_testing ()

set (bibtex_spirit_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set (CPACK_GENERATOR ZIP TGZ)
include (CPack)

# Do not link Boost libraries automatically
add_definitions (-DBOOST_ALL_NO_LIB)

file (GLOB BIBTEX_HPP include/*.hpp)

add_subdirectory (tests)

install (FILES ${BIBTEX_HPP} DESTINATION include/bibtex_spirit)
