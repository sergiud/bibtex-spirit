add_executable (test_parse_file main.cpp ${BIBTEX_HPP})

target_link_libraries (test_parse_file ${Boost_LIBRARIES})

add_test (parse-file test_parse_file ${CMAKE_CURRENT_SOURCE_DIR}/test.bib 1151)
