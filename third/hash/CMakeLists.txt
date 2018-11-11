# Sources to compile and integrate in executable

SET (SOURCES src/utils_hash.cpp)

INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES (${WXWIDGETS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES (${APPLICATION_SOURCE_DIR}/include)

# Main executable
ADD_LIBRARY (utils_hash STATIC ${SOURCES})
