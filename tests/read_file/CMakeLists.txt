find_package (bibtex_spirit REQUIRED NO_MODULE)

include_directories (${bibtex_spirit_INCLUDE_DIR} ${bibtex_spirit_INCLUDE_DIRS})

add_definitions (${bibtex_spirit_DEFINITIONS})

add_executable (test_read_file main.cpp ${BIBTEX_HPP})

target_link_libraries (test_read_file ${Boost_LIBRARIES}
  ${bibtex_spririt_LIBRARY} ${bibtex_spririt_LIBRARIES})

set (TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../data)

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
