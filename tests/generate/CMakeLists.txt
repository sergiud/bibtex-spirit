find_package (bibtex_spirit REQUIRED NO_MODULE)
find_package (Boost 1.46 REQUIRED unit_test_framework)

include_directories (${bibtex_spirit_INCLUDE_DIR} ${bibtex_spirit_INCLUDE_DIRS})
include_directories (${Boost_INCLUDE_DIRS})

add_definitions (${bibtex_spirit_DEFINITIONS})

add_executable (test_generate main.cpp ${BIBTEX_HPP})

target_link_libraries (test_generate ${Boost_LIBRARIES}
  ${bibtex_spririt_LIBRARY} ${bibtex_spririt_LIBRARIES}
  ${Boost_LIBRARIES})
