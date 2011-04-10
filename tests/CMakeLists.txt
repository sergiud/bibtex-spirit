add_executable (test_main main.cpp ${BIBTEX_HPP})

target_link_libraries (test_main ${Boost_LIBRARIES})

add_test (structure test_main -t structure)
add_test (newline-comment-1 test_main -t newline_comment_1)
add_test (newline-comment-2 test_main -t newline_comment_2)
add_test (missing_key test_main -t missing_key)
add_test (empty test_main -t empty)
