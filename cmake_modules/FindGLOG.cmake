#Default locations to search for on various platforms.
LIST(APPEND SEARCH_LIBS /usr/lib)
LIST(APPEND SEARCH_LIBS /usr/local/lib)
LIST(APPEND SEARCH_LIBS $ENV{HOME}/.local/lib)

LIST(APPEND SEARCH_HEADERS /usr/include)
LIST(APPEND SEARCH_HEADERS /usr/local/include)
LIST(APPEND SEARCH_HEADERS $ENV{HOME}/.local/include)

# Google libraries
message("-- Check for Google Log")
find_library(GLOG_LIB NAMES glog PATHS ${SEARCH_LIBS})
find_path(GLOG_INCLUDE NAMES glog/logging.h PATHS ${SEARCH_HEADERS})
set(GLOG_FOUND ON)
