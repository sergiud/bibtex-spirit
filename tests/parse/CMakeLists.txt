find_package (bibtex_spirit REQUIRED NO_MODULE)

include_directories (${bibtex_spirit_INCLUDE_DIR} ${bibtex_spirit_INCLUDE_DIRS})

add_definitions (${bibtex_spirit_DEFINITIONS})

add_executable (test_parse main.cpp ${BIBTEX_HPP})

target_link_libraries (test_parse ${Boost_LIBRARIES}
  ${bibtex_spririt_LIBRARY} ${bibtex_spririt_LIBRARIES})

add_test (structure-1 test_parse -t structure_1)
add_test (structure-2 test_parse -t structure_2)
add_test (newline-comment-1 test_parse -t newline_comment_1)
add_test (newline-comment-2 test_parse -t newline_comment_2)
add_test (missing_key test_parse -t missing_key)
add_test (empty test_parse -t empty)
add_test (comparison test_parse -t comparison)
add_test (multiple-1 test_parse -t multiple_1)
add_test (multiple-2 test_parse -t multiple_2)
add_test (mixed-multiple test_parse -t mixed_multiple)
add_test (separated test_parse -t separated)
add_test (junk test_parse -t junk)
