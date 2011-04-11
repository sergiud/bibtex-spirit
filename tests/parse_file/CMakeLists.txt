add_executable (test_parse_file main.cpp ${BIBTEX_HPP})

target_link_libraries (test_parse_file ${Boost_LIBRARIES})
