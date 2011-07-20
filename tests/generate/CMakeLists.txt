find_package (bibtex_spirit REQUIRED NO_MODULE)

include_directories (${bibtex_spirit_INCLUDE_DIR} ${bibtex_spirit_INCLUDE_DIRS})

add_definitions (${bibtex_spirit_DEFINITIONS})

add_executable (test_generate main.cpp ${BIBTEX_HPP})

target_link_libraries (test_generate ${Boost_LIBRARIES}
  ${bibtex_spririt_LIBRARY} ${bibtex_spririt_LIBRARIES})
