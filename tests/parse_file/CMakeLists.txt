add_executable (test_parse_file main.cpp ${BIBTEX_HPP})

target_link_libraries (test_parse_file ${Boost_LIBRARIES})

add_test (parse-file-1 test_parse_file ${CMAKE_CURRENT_SOURCE_DIR}/test1.bib
  1151)
add_test (parse-file-2 test_parse_file ${CMAKE_CURRENT_SOURCE_DIR}/test2.bib
  2710)
add_test (parse-file-3 test_parse_file ${CMAKE_CURRENT_SOURCE_DIR}/test3.bib
  3302)
add_test (parse-file-4 test_parse_file ${CMAKE_CURRENT_SOURCE_DIR}/test4.bib
  2649)
add_test (parse-file-5 test_parse_file ${CMAKE_CURRENT_SOURCE_DIR}/test5.bib
  4649)
