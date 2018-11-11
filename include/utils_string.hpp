/**
 * String utilities modules - Header file.
 *
 * \file utils_string.cpp
 * \author Allann Jones
 */

#ifndef UTILS_STRING_HPP
#define UTILS_STRING_HPP

#include "ui/gui.hpp"

#define CONST_STR_MAX_LEN 1024
#define CONST_STR_FILENAME_MAX_LEN 2048

class UtilsString {
    public:
        static wxString RegexSafeDirPath(wxString str);

        static wxString NormalizeDirPath(wxString str);
};

#endif /* UTILS_STRING_HPP */
