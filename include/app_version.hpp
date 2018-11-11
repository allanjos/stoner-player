/**
 * Information about the application.
 *
 * \file app_version.hpp
 * \author Allann Jones
 */
#ifndef APP_VERSION_HPP
#define APP_VERSION_HPP

#define APP_NAME "stoner"
#define APP_DESCRIPTION "Simple audio recorder and player"

/** Application major version. */
#define APP_VERSION_MAJOR "0"

/** Application minor (revision) version. */
#define APP_VERSION_MINOR "0"

/** Application release control. */
#define APP_REVISION_NUMBER "1"

/** Application version string. */
#if defined(APP_REVISION_NUMBER)
#  define APP_VERSION APP_VERSION_MAJOR "." APP_VERSION_MINOR "." APP_REVISION_NUMBER
#else
#  define APP_VERSION APP_VERSION_MAJOR "." APP_VERSION_MINOR
#endif

#endif /* APP_VERSION_HPP */
