find_package (bibtex_spirit REQUIRED NO_MODULE)
find_package (Boost 1.54 REQUIRED unit_test_framework)

include_directories (${bibtex_spirit_INCLUDE_DIR} ${bibtex_spirit_INCLUDE_DIRS})
include_directories (${Boost_INCLUDE_DIRS})

add_definitions (${bibtex_spirit_DEFINITIONS})

add_executable (test_parse main.cpp ${BIBTEX_HPP})

target_link_libraries (test_parse ${Boost_LIBRARIES}
  ${bibtex_spririt_LIBRARY} ${bibtex_spririt_LIBRARIES}
  ${Boost_LIBRARIES})

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
