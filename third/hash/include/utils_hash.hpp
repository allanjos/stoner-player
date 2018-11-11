/*
 **************************************************************************
 *                                                                        *
 *          General Purpose Hash Function Algorithms Library              *
 *                                                                        *
 * Author: Arash Partow - 2002                                            *
 * URL: http://www.partow.net                                             *
 * URL: http://www.partow.net/programming/hashfunctions/index.html        *
 *                                                                        *
 * Copyright notice:                                                      *
 * Free use of the General Purpose Hash Function Algorithms Library is    *
 * permitted under the guidelines and in accordance with the most current *
 * version of the Common Public License.                                  *
 * http://www.opensource.org/licenses/cpl.php                             *
 *                                                                        *
 **************************************************************************
*/


#ifndef _UTILS_HASH_HPP_
#define _UTILS_HASH_HPP_


#include <string>
#include "ui/gui.hpp"


typedef unsigned int HashKeyType_t;
typedef unsigned int (*HashFunction)(const std::string&);

class UtilsHash {
  public:
    static unsigned int RSHash  (const std::string& str);

    static unsigned int JSHash  (const std::string& str);

    static unsigned int PJWHash (const std::string& str);

    static unsigned int ELFHash (const std::string& str);

    static unsigned int BKDRHash(const std::string& str);

    static unsigned int SDBMHash(const std::string& str);

    static unsigned int DJBHash (const std::string& str);

    static unsigned int DEKHash (const std::string& str);

    static unsigned int APHash  (const std::string& str);

    static unsigned int GetHash(wxString str);
};

#endif /* _UTILS_HASH_HPP_ */
