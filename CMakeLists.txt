#
# Audio recorder/player application project
#
# Allann Jones
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT (APPLICATION)

SET (APP_NAME "stoner")

SET (EXECUTABLE_OUTPUT_PATH ${APPLICATION_BINARY_DIR}/bin)
SET (LIBRARY_OUTPUT_PATH ${APPLICATION_BINARY_DIR}/lib)

IF (UNIX)
  SET (OPERATING_SYSTEM "unix")
ELSE (UNIX)
  SET (OPERATING_SYSTEM "windows")
ENDIF (UNIX)

# OS

IF (UNIX) # Linux
  SET (OS_DISTRO fc25)
ELSE (UNIX)
  # win32, win64
  SET (OS_DISTRO win32)
  SET (OS_BUILDTYPE "msys")
ENDIF (UNIX)

SET (CMAKE_VERBOSE_MAKEFILE "ON")

# Flags for release mode

SET (CMAKE_C_FLAGS_RELEASE "")
SET (CMAKE_CXX_FLAGS_RELEASE "")
SET (CMAKE_C_FLAGS_RELEASE_INIT "")
SET (CMAKE_CXX_FLAGS_RELEASE_INIT "-NDEBUG")

SET (LIBSNDFILE_ACTIVATED TRUE)
SET (SNDLIB_ACTIVATED TRUE)
SET (PORTAUDIO_ACTIVATED TRUE)

SET (CMAKE_BUILD_TYPE "Release")

SET (WXWIDGETS_DIR ${APPLICATION_SOURCE_DIR}/third/wxWidgets)
# Subfolder inside folder third/wxWidgets/ that store binary wxWidgets package
SET (WXWIDGETS_DIST_DIR "dist")

# Hash library

SET (HASHLIB_DIR ${APPLICATION_SOURCE_DIR}/third/hash)

INCLUDE_DIRECTORIES (${HASHLIB_DIR}/include)

# Sndlib

#IF (SNDLIB_ACTIVATED)
#  SET (SNDLIB_DIR ${APPLICATION_SOURCE_DIR}/third/sndlib)
#ENDIF (SNDLIB_ACTIVATED)

# Libsndfile

IF (LIBSNDFILE_ACTIVATED)
  SET (LIBSNDFILE_DIR ${APPLICATION_SOURCE_DIR}/third/libsndfile)
  SET (LIBSNDFILE_DIR ${APPLICATION_SOURCE_DIR}/third/libsndfile)

  IF (UNIX)
    SET (LIBSNDFILE_INCLUDE_DIRS ${LIBSNDFILE_DIR} ${LIBSNDFILE_DIR}/src/GSM610
                                 ${LIBSNDFILE_DIR}/src
                                 ${LIBSNDFILE_DIR}/src/G72x
                                 ${LIBSNDFILE_DIR}/dist/linux/${LINUX_DISTRO}/include)
    SET (LIBSNDFILE_LIBRARIES_DIRS ${LIBSNDFILE_DIR}/dist/linux/${LINUX_DISTRO}/lib)
  ELSE (UNIX)
    SET (LIBSNDFILE_INCLUDE_DIRS ${APPLICATION_BINARY_DIR}/third/libsndfile/src # Search, firstly in integrated project
                                 ${LIBSNDFILE_DIR}/dist/windows/src ${LIBSNDFILE_DIR}/dist/windows/include) # Second, check in third

    SET (LIBSNDFILE_LIBRARIES_DIRS ${LIBSNDFILE_DIR}/dist/windows/lib)
  ENDIF (UNIX)
ENDIF (LIBSNDFILE_ACTIVATED)

SET (PDFDOC_DIR ${APPLICATION_SOURCE_DIR}/third/wxpdfdoc)
SET (PDFDOC_LIBRARIES wxpdfdoc)
SET (PDFDOC_INCLUDE_DIR ${PDFDOC_DIR}/include)

# PortAudio

IF (PORTAUDIO_ACTIVATED)
  SET (PORTAUDIO_DIR ${APPLICATION_SOURCE_DIR}/third/portaudio)

  INCLUDE_DIRECTORIES (${PORTAUDIO_DIR}/include
                       ${PORTAUDIO_DIR}/src/common)
ENDIF (PORTAUDIO_ACTIVATED)

# Base64 algorithm library

SET (LIBB64_DIR ${APPLICATION_SOURCE_DIR}/third/b64)
SET (LIBB64_INCLUDE_DIRS ${LIBB64_DIR}/include)

# SQLite

SET (SQLITE_DIR ${TAXONOMY_SOURCE_DIR}/third/sqlite)
SET (SQLITE_INCLUDE_DIRS ${SQLITE_DIR}/include)

# wxWidgets

SET (WXWIDGETS_INCLUDE_DIRS "")

IF (NOT MSVC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
ENDIF (NOT MSVC)

# wxWidgets - Linux / Unix

IF (UNIX)
  SET (WXWIDGETS_VERSION_MAJOR 3.1)
  SET (WXWIDGETS_VERSION_SUFFIX ${WXWIDGETS_VERSION_MAJOR})
  SET (WXWIDGETS_UNICODE_SUFFIX "u")
  SET (WXWIDGETS_DEBUG_SUFFIX "")
  # SET (WXWIDGETS_RELEASE_PART_NAME "")
  SET (WXWIDGETS_RELEASE_PART_NAME "")

  SET (WXWIDGETS_PLATFORM_SUFFIX "gtk3")

  #ADD_DEFINITIONS (-D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -g -pthread -Wall -DwxDEBUG_LEVEL=0)
  ADD_DEFINITIONS (-D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -g -pthread -Wall)

  IF (WXWIDGETS_PLATFORM_SUFFIX STREQUAL "gtk2")
    SET (WXWIDGETS_LIBRARIES
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_richtext-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_adv-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_html-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_aui-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_propgrid-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_core-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_qa-${WXWIDGETS_VERSION_SUFFIX}
                             #wx_${WXWIDGETS_PLATFORM_SUFFIX}u_gl-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_xrc-${WXWIDGETS_VERSION_SUFFIX}
                             #wx_${WXWIDGETS_PLATFORM_SUFFIX}u_media-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_ribbon-${WXWIDGETS_VERSION_SUFFIX}
                             wx_baseu_xml-${WXWIDGETS_VERSION_SUFFIX}
                             wx_baseu_net-${WXWIDGETS_VERSION_SUFFIX}
                             wx_baseu-${WXWIDGETS_VERSION_SUFFIX}
                             wxregexu-${WXWIDGETS_VERSION_SUFFIX}

                             gtk-x11-2.0
                             gdk-x11-2.0
                             pangocairo-1.0
                             atk-1.0
                             cairo
                             gdk_pixbuf-2.0
                             gio-2.0
                             pangoft2-1.0
                             pango-1.0
                             gobject-2.0
                             fontconfig
                             freetype
                             gthread-2.0
                             glib-2.0
                             X11
                             Xxf86vm
                             SM
                             gtk-x11-2.0
                             gdk-x11-2.0
                             pangocairo-1.0
                             atk-1.0
                             cairo
                             gdk_pixbuf-2.0
                             gio-2.0
                             pangoft2-1.0
                             pango-1.0
                             gobject-2.0
                             glib-2.0
                             fontconfig
                             freetype
                             notify
                             gdk_pixbuf-2.0
                             gio-2.0
                             gobject-2.0
                             glib-2.0
                             png
                             z
                             jpeg
                             tiff
                             pthread
                             expat
                             #z
                             dl
                             m)
  ELSE ()
    SET (WXWIDGETS_LIBRARIES
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_richtext-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_adv-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_html-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_aui-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_propgrid-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_core-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_qa-${WXWIDGETS_VERSION_SUFFIX}
                             #wx_${WXWIDGETS_PLATFORM_SUFFIX}u_gl-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_xrc-${WXWIDGETS_VERSION_SUFFIX}
                             #wx_${WXWIDGETS_PLATFORM_SUFFIX}u_media-${WXWIDGETS_VERSION_SUFFIX}
                             wx_${WXWIDGETS_PLATFORM_SUFFIX}u_ribbon-${WXWIDGETS_VERSION_SUFFIX}
                             wx_baseu_xml-${WXWIDGETS_VERSION_SUFFIX}
                             wx_baseu_net-${WXWIDGETS_VERSION_SUFFIX}
                             wx_baseu-${WXWIDGETS_VERSION_SUFFIX}
                             wxregexu-${WXWIDGETS_VERSION_SUFFIX}

                             expat
                             gtk-3
                             gdk-3
                             pangocairo-1.0
                             pango-1.0
                             atk-1.0
                             cairo-gobject
                             cairo
                             gdk_pixbuf-2.0
                             gio-2.0
                             gobject-2.0
                             gthread-2.0
                             glib-2.0
                             X11
                             Xxf86vm
                             SM
                             notify
                             webkitgtk-3.0
                             soup-2.4
                             javascriptcoregtk-3.0
                             png
                             z
                             jpeg
                             tiff
                             pthread
                             dl
                             m)
  ENDIF ()

    #INCLUDE_DIRECTORIES (${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/linux/${LINUX_DISTRO}/lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode-release-static-${WXWIDGETS_VERSION_SUFFIX}
    #                     ${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/linux/${LINUX_DISTRO}/include/wx-${WXWIDGETS_VERSION_MAJOR})

  SET (WXWIDGETS_INCLUDE_DIRS ${WXWIDGETS_INCLUDE_DIRS}
  # gtk2-unicode-3.0
  # ./lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode-3.0/wx/setup.h
                              ${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/linux/${LINUX_DISTRO}/lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode${WXWIDGETS_RELEASE_PART_NAME}-${WXWIDGETS_VERSION_SUFFIX}
                              ${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/linux/${LINUX_DISTRO}/lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode-static-${WXWIDGETS_VERSION_SUFFIX}
                              ${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/linux/${LINUX_DISTRO}/lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode${WXWIDGETS_RELEASE_PART_NAME}-${WXWIDGETS_VERSION_SUFFIX}
                              ${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/linux/${LINUX_DISTRO}/include/wx-${WXWIDGETS_VERSION_MAJOR})


  LINK_DIRECTORIES (/usr/X11R6/lib
                    ${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/linux/${LINUX_DISTRO}/lib)

# wxWidgets - Windows

ELSE (UNIX)
  SET (WXWIDGETS_VERSION_INCLUDE_DIR "3.1")
  SET (WXWIDGETS_VERSION_SUFFIX "3.1")
  SET (WXWIDGETS_UNICODE_SUFFIX "u")
  SET (WXWIDGETS_DEBUG_SUFFIX "")

  #ADD_DEFINITIONS(-DwxDEBUG_LEVEL=0 -D__WXMSW__ -mwindows -mthreads)
  #SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -D__WXMSW__ -MD -MP -DNOPCH -DWXUSINGDLL -D_UNICODE -DUNICODE -mwindows -mthreads")
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__WXMSW__ -D_UNICODE -D_LARGEFILE_SOURCE=unknown")

  LINK_DIRECTORIES (${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/${OS_DISTRO}/lib)

  IF (OS_BUILDTYPE STREQUAL "msys")
    SET (WXWIDGETS_INCLUDE_DIRS ${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/${OS_DISTRO}/lib/wx/include/msw-unicode-static-${WXWIDGETS_VERSION_INCLUDE_DIR}
                                ${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/${OS_DISTRO}/lib/mswu
                                ${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/${OS_DISTRO}/include/wx-${WXWIDGETS_VERSION_INCLUDE_DIR}
                                ${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/${OS_DISTRO}/lib/msw-${WXWIDGETS_UNICODE_SUFFIX})

    SET(WXWIDGETS_LIBRARIES wx_mswu_aui-3.1
                            wx_baseu_xml-3.1
                            wx_mswu_html-3.1
                            wx_mswu_adv-3.1
                            wx_mswu_core-3.1
                            wx_baseu-3.1
                            wxtiff-3.1
                            wxjpeg-3.1
                            wxpng-3.1
                            wxregexu-3.1
                            wxzlib-3.1 # MSys

                            rpcrt4
                            oleaut32
                            ole32
                            uuid
                            winspool
                            winmm
                            shell32
                            shlwapi
                            comctl32
                            comdlg32
                            advapi32
                            version
                            wsock32
                            gdi32
                            uxtheme
                            oleacc
                            #z # MSys 2
                            )
  ELSE ()
    SET (WXWIDGETS_INCLUDE_DIRS ${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/${OS_DISTRO}/lib
                                ${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/${OS_DISTRO}/include
                                ${WXWIDGETS_DIR}/${WXWIDGETS_DIST_DIR}/${OS_DISTRO}/lib/mswu)

    SET(WXWIDGETS_LIBRARIES wxmsw31u_adv
                            wxmsw31u_html
                            wxmsw31u_aui
                            wxbase31u_xml
                            wxmsw31u_core
                            wxbase31u
                            wxtiff
                            wxjpeg
                            wxpng
                            wxzlib
                            wxregexu
                            wxexpat
                            kernel32
                            user32
                            gdi32
                            comdlg32
                            winspool
                            winmm
                            shell32
                            comctl32
                            ole32
                            oleaut32
                            uuid
                            rpcrt4
                            advapi32
                            wsock32
                            wininet)
  ENDIF ()

  #INCLUDE_DIRECTORIES (${WXWIDGETS_DIR}/include
  #                     ${WXWIDGETS_DIR}/lib/vc_lib/msw${WXWIDGETS_UNICODE_SUFFIX}${WXWIDGETS_DEBUG_SUFFIX})
ENDIF (UNIX)

ADD_SUBDIRECTORY(third)
ADD_SUBDIRECTORY(src)
