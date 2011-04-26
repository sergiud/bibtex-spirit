add_executable (test_read_file main.cpp ${BIBTEX_HPP})

target_link_libraries (test_read_file ${Boost_LIBRARIES})

add_test (read-file-1 test_read_file ${CMAKE_CURRENT_SOURCE_DIR}/test1.bib 1151)
add_test (read-file-2 test_read_file ${CMAKE_CURRENT_SOURCE_DIR}/test2.bib 2710)
add_test (read-file-3 test_read_file ${CMAKE_CURRENT_SOURCE_DIR}/test3.bib 3302)
add_test (read-file-4 test_read_file ${CMAKE_CURRENT_SOURCE_DIR}/test4.bib 2649)
add_test (read-file-5 test_read_file ${CMAKE_CURRENT_SOURCE_DIR}/test5.bib 4649)
add_test (read-file-6 test_read_file ${CMAKE_CURRENT_SOURCE_DIR}/test6.bib
  21099)
