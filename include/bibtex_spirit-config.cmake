find_package (Boost 1.46 REQUIRED)

set (bibtex_spirit_DEFINITIONS)
set (bibtex_spirit_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
set (bibtex_spirit_LIBRARIES ${Boost_LIBRARIES})
set (bibtex_spirit_LIBRARY)

get_filename_component (_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component (bibtex_spirit_INCLUDE_DIR ${_DIR} ABSOLUTE)
