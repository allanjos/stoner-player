# Sources to compile and integrate in executable

SET (SOURCES audio/audio.cpp
             ui/gui.cpp
             ui/utils_audio_plot.cpp
             ui/panel_audio_player.cpp
             ui/panel_audio_recorder.cpp
             ui/panel_audio.cpp
             ui/dlg_main.cpp
             app.cpp
             app.rc)

# wxWidgets

INCLUDE_DIRECTORIES (${WXWIDGETS_INCLUDE_DIRS})

# Header directory

INCLUDE_DIRECTORIES (${APPLICATION_SOURCE_DIR}/include)

# Libsndfile

IF (LIBSNDFILE_ACTIVATED)
  INCLUDE_DIRECTORIES (${LIBSNDFILE_INCLUDE_DIRS})
ENDIF (LIBSNDFILE_ACTIVATED)

# Base 64

INCLUDE_DIRECTORIES (${LIBB64_INCLUDE_DIRS})

# SQLite

INCLUDE_DIRECTORIES (${SQLITE_INCLUDE_DIRS})

# Sndlib

IF (SNDLIB_ACTIVATED)
  INCLUDE_DIRECTORIES (${SNDLIB_DIR}/include ${SNDLIB_DIR}/src/os/unix ${SNDLIB_DIR}/src/common)

  IF (UNIX)
    INCLUDE_DIRECTORIES (${SNDLIB_DIR}/include/linux)
  ENDIF (UNIX)
ENDIF (SNDLIB_ACTIVATED)

# Resources

ADD_CUSTOM_COMMAND(OUTPUT app_rc.o
                   COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR}
                                       -I${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/win32/lib
                                       -I${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/win32/include
                                       -I${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/${OS_DISTRO}/include/wx-${WXWIDGETS_VERSION_SUFFIX}
                                       -I${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/win32/lib/mswu
                                       -I${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/win32/lib/wx/include/msw-unicode${WXWIDGETS_RELEASE_PART_NAME}-static-${WXWIDGETS_VERSION_SUFFIX}
                                       -i${CMAKE_CURRENT_SOURCE_DIR}/app.rc
                                       -o ${CMAKE_CURRENT_BINARY_DIR}/app_rc.o)


# Main executable

IF (UNIX)
  ADD_EXECUTABLE (${APP_NAME} ${SOURCES})
ELSE (UNIX)
  ADD_EXECUTABLE (${APP_NAME} WIN32 ${SOURCES}) #  ${CMAKE_CURRENT_BINARY_DIR}/app_rc.o
ENDIF (UNIX)

#set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS_RELEASE "")
#set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS_DEBUG "")
#set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "")

# Sndfile library

IF (LIBSNDFILE_ACTIVATED)
  FIND_LIBRARY (SNDFILE_LIBRARY NAMES sndfile PATHS ${LIBRARY_OUTPUT_PATH})

  IF (SNDFILE_LIBRARY)
    SET (SNDFILE_FOUND TRUE)
    TARGET_LINK_LIBRARIES (${APP_NAME} ${SNDFILE_LIBRARY})
  ENDIF (SNDFILE_LIBRARY)

  INCLUDE_DIRECTORIES (${LIBSNDFILE_INCLUDE_DIRS})
ENDIF (LIBSNDFILE_ACTIVATED)

# PortAudio library

IF (PORTAUDIO_ACTIVATED)
  FIND_LIBRARY (PORTAUDIO_LIBRARY NAMES portaudio PATHS ${LIBRARY_OUTPUT_PATH})

  IF (PORTAUDIO_LIBRARY)
    SET (PORTAUDIO_FOUND TRUE)
    TARGET_LINK_LIBRARIES (${APP_NAME} ${PORTAUDIO_LIBRARY})
  ENDIF (PORTAUDIO_LIBRARY)

  INCLUDE_DIRECTORIES(${PORTAUDIO_INCLUDE_DIRS})
ENDIF (PORTAUDIO_ACTIVATED)

# Base 64 library

FIND_LIBRARY (B64_LIBRARY NAMES b64 PATHS ${LIBRARY_OUTPUT_PATH})

IF (B64_LIBRARY)
  SET (B64_FOUND TRUE)
  TARGET_LINK_LIBRARIES (${APP_NAME} ${B64_LIBRARY})
ENDIF (B64_LIBRARY)

# SQLite library

FIND_LIBRARY (SQLITE_LIBRARY NAMES SQLITE PATHS ${LIBRARY_OUTPUT_PATH})

IF (SQLITE_LIBRARY)
  TARGET_LINK_LIBRARIES (${APP_NAME} ${SQLITE_LIBRARY})
ENDIF (SQLITE_LIBRARY)

# wxWidgets library linking

TARGET_LINK_LIBRARIES (${APP_NAME} ${WXWIDGETS_LIBRARIES})

# Link system libraries

IF (UNIX)
  TARGET_LINK_LIBRARIES (${APP_NAME} asound m rt)
ELSE (UNIX)
ENDIF (UNIX)

# Link GCC compiler statically

IF (NOT UNIX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static-libstdc++ -mthreads -Wl,--subsystem,windows") # -mwindows

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -mthreads -Wl,--subsystem,windows") # -mwindows
ENDIF(NOT UNIX)
