/**
 * Filesystem utilities library
 *
 * \author Allann Jones
 */

#ifndef UTILS_FILESYSTEM_HPP
#define UTILS_FILESYSTEM_HPP

#include "ui/gui.hpp"

typedef int FileType_t;

/**
 * Filesystem utilities class.
 */
class UtilsFilesystem {
    public:
        static const int FILE_TYPE_FOLDER          = 0;
        static const int FILE_TYPE_AUDIO           = 1;
        static const int FILE_TYPE_NONE            = 1000;

        static bool IsDir(wxString &path);

        static FileType_t GetFileType(wxString &path);

        static wxString GetDirSeparator(void);
};

#endif /* _UTILS_FILESYSTEM_HPP_ */
