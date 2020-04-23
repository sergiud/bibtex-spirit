cmake_minimum_required (VERSION 3.5)
project (bibtex-spirit
  VERSION 0.1
  LANGUAGES CXX
)

set (CPACK_GENERATOR ZIP TGZ)

find_package (Boost 1.58 REQUIRED unit_test_framework)

include (CPack)
include (CTest)

# Do not link Boost libraries automatically
add_compile_definitions (BOOST_ALL_NO_LIB)
add_compile_definitions (BOOST_TEST_DYN_LINK)

add_library (bibtex-spirit INTERFACE)
target_include_directories (bibtex-spirit INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries (bibtex-spirit INTERFACE Boost::boost)

set (BIBTEX_HPP
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bibtexentry.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bibtexreader.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/bibtexwriter.hpp
)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (BUILD_TESTING)
  add_executable (test_generate tests/test_generate.cpp)
  target_link_libraries (test_generate PRIVATE Boost::unit_test_framework
    bibtex-spirit)

  add_executable (test_parse tests/test_parse.cpp)
  target_link_libraries (test_parse PRIVATE Boost::unit_test_framework
    bibtex-spirit)

  add_test (NAME structure-1 COMMAND test_parse -t structure_1)
  add_test (NAME structure-2 COMMAND test_parse -t structure_2)
  add_test (NAME newline-comment-1 COMMAND test_parse -t newline_comment_1)
  add_test (NAME newline-comment-2 COMMAND test_parse -t newline_comment_2)
  add_test (NAME missing_key COMMAND test_parse -t missing_key)
  add_test (NAME empty COMMAND test_parse -t empty)
  add_test (NAME comparison COMMAND test_parse -t comparison)
  add_test (NAME multiple-1 COMMAND test_parse -t multiple_1)
  add_test (NAME multiple-2 COMMAND test_parse -t multiple_2)
  add_test (NAME mixed-multiple COMMAND test_parse -t mixed_multiple)
  add_test (NAME separated COMMAND test_parse -t separated)
  add_test (NAME junk COMMAND test_parse -t junk)

  add_executable (test_read_file tests/test_read_file.cpp)

  target_link_libraries (test_read_file PRIVATE Boost::unit_test_framework
    bibtex-spirit)

  set (TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)

  add_test (NAME read-file-1 COMMAND test_read_file
    ${TEST_DATA_DIR}/test1.bib 1151)
  add_test (NAME read-file-2 COMMAND test_read_file
    ${TEST_DATA_DIR}/test2.bib 2710)
  add_test (NAME read-file-3 COMMAND test_read_file
    ${TEST_DATA_DIR}/test3.bib 3302)
  add_test (NAME read-file-4 COMMAND test_read_file
    ${TEST_DATA_DIR}/test4.bib 2649)
  add_test (NAME read-file-5 COMMAND test_read_file
    ${TEST_DATA_DIR}/test5.bib 4649)
  add_test (NAME read-file-6 COMMAND test_read_file
    ${TEST_DATA_DIR}/test6.bib 21099)
  add_test (NAME read-file-biblatex-examples COMMAND test_read_file
    ${TEST_DATA_DIR}/biblatex-examples.bib 86)
endif (BUILD_TESTING)

install (FILES ${BIBTEX_HPP} DESTINATION include/bibtex_spirit)
