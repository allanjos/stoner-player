# Sources to compile and integrate in executable

SET (SOURCES src/common/pa_allocation.c
             src/common/pa_converters.c
             src/common/pa_cpuload.c
             src/common/pa_dither.c
             src/common/pa_front.c
             src/common/pa_process.c
             src/common/pa_skeleton.c
             src/common/pa_stream.c
             src/common/pa_trace.c)

IF (UNIX)
SET (SOURCES ${SOURCES}
             src/hostapi/alsa/pa_linux_alsa.c
             src/hostapi/alsa/pa_linux_alsa.c
             src/hostapi/oss/pa_unix_oss.c
             src/os/unix/pa_unix_hostapis.c
             src/os/unix/pa_unix_util.c)
ELSE (UNIX)
SET (SOURCES ${SOURCES}
             src/hostapi/wmme/pa_win_wmme.c
             src/os/win/pa_win_hostapis.c
             src/os/win/pa_win_util.c)
ENDIF (UNIX)

INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR}/include
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/common)

IF (UNIX)
  INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR}/src/os/unix)

  ADD_DEFINITIONS (-DPA_LITTLE_ENDIAN -DPACKAGE_NAME=\\"\\" -DPACKAGE_TARNAME=\\"\\" -DPACKAGE_VERSION=\\"\\" -DPACKAGE_STRING=\\"\\" -DPACKAGE_BUGREPORT=\\"\\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=1 -DSIZEOF_SHORT=2 -DSIZEOF_INT=4 -DSIZEOF_LONG=4 -DHAVE_CLOCK_GETTIME=1 -DHAVE_NANOSLEEP=1 -DPA_USE_ALSA=1 -DPA_USE_OSS=1)

ELSE (UNIX)
  ADD_DEFINITIONS (-DPA_LITTLE_ENDIAN -DPA_NO_DS -DPA_NO_ASIO -DPA_NO_WDMKS -mthreads -DPACKAGE_NAME=\\"\\" -DPACKAGE_TARNAME=\\"\\" -DPACKAGE_VERSION=\\"\\" -DPACKAGE_STRING=\\"\\" -DPACKAGE_BUGREPORT=\\"\\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DSIZEOF_SHORT=2 -DSIZEOF_INT=4 -DSIZEOF_LONG=4)

  INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR}/src/os/unix)
ENDIF (UNIX)


# Main executable
ADD_LIBRARY (portaudio STATIC ${SOURCES})


IF (UNIX)
  TARGET_LINK_LIBRARIES (portaudio rt asound m pthread)
ELSE (UNIX)
  TARGET_LINK_LIBRARIES (portaudio rt asound m pthread winmm)
ENDIF (UNIX)
