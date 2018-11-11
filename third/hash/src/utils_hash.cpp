#include "utils_hash.hpp"

unsigned int UtilsHash::RSHash(const std::string& str)
{
   unsigned int b    = 378551;
   unsigned int a    = 63689;
   unsigned int hash = 0;


   for(std::size_t i = 0; i < str.length(); i++)
   {
     hash = hash * a + str[i];
     a    = a * b;
   }

   return (hash & 0x7FFFFFFF);
}
/* End Of RS Hash Function */


unsigned int UtilsHash::JSHash(const std::string& str)
{
   unsigned int hash = 1315423911;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash ^= ((hash << 5) + str[i] + (hash >> 2));
   }

   return (hash & 0x7FFFFFFF);
}
/* End Of JS Hash Function */


unsigned int UtilsHash::PJWHash(const std::string& str)
{
   unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
   unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
   unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
   unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
   unsigned int hash              = 0;
   unsigned int test              = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = (hash << OneEighth) + str[i];

      if((test = hash & HighBits)  != 0)
      {
         hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
      }
   }

   return (hash & 0x7FFFFFFF);
}
/* End Of  P. J. Weinberger Hash Function */


unsigned int UtilsHash::ELFHash(const std::string& str)
{
   unsigned int hash = 0;
   unsigned int x    = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = (hash << 4) + str[i];
      if((x = hash & 0xF0000000L) != 0)
      {
         hash ^= (x >> 24);
         hash &= ~x;
      }
   }

   return (hash & 0x7FFFFFFF);
}
/* End Of ELF Hash Function */


unsigned int UtilsHash::BKDRHash(const std::string& str)
{
   unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
   unsigned int hash = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = (hash * seed) + str[i];
   }

   return (hash & 0x7FFFFFFF);
}
/* End Of BKDR Hash Function */


unsigned int UtilsHash::SDBMHash(const std::string& str)
{
   unsigned int hash = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = str[i] + (hash << 6) + (hash << 16) - hash;
   }

   return (hash & 0x7FFFFFFF);
}
/* End Of SDBM Hash Function */


unsigned int UtilsHash::DJBHash(const std::string& str)
{
   unsigned int hash = 5381;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = ((hash << 5) + hash) + str[i];
   }

   return (hash & 0x7FFFFFFF);
}
/* End Of DJB Hash Function */


unsigned int UtilsHash::DEKHash(const std::string& str)
{
   unsigned int hash = static_cast<unsigned int>(str.length());

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = ((hash << 5) ^ (hash >> 27)) ^ str[i];
   }

   return (hash & 0x7FFFFFFF);
}
/* End Of DEK Hash Function */


unsigned int UtilsHash::APHash(const std::string& str)
{
   unsigned int hash = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ str[i] ^ (hash >> 3)) :
                               (~((hash << 11) ^ str[i] ^ (hash >> 5)));
   }

   return (hash & 0x7FFFFFFF);
}
/* End Of AP Hash Function */


unsigned int UtilsHash::GetHash(wxString str)
{
   unsigned int b    = 378551;
   unsigned int a    = 63689;
   unsigned int hash = 0;
   char strANSI[strlen(str.mb_str(wxConvUTF8)) + 1];

   strcpy(strANSI, (const char *) str.mb_str(wxConvUTF8));

   std::cout << "strANSI: " << strANSI << std::endl;

   /*
   for (std::size_t i = 0; i < str.Length(); i++) {
     std::cout << "UtilsHash::GetHash: " << str.at(i) << std::endl;

     hash = hash * a + str.at(i);
     a    = a * b;
   }
   */

   for (size_t i = 0; i < strlen(strANSI); i++) {
     hash = hash * a + strANSI[i];
     a    = a * b;
   }

   return (hash & 0x7FFFFFFF);

  return 0;
}
