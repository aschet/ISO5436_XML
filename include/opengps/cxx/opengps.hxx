#ifndef _OPENGPS_HXX
#define _OPENGPS_HXX

#ifndef _OPENGPS_H
# include <opengps/opengps.h>
#endif

#include <string>

namespace OpenGPS
{
typedef char Byte;
typedef Byte* BytePtr;

typedef unsigned char UnsignedByte;
typedef UnsignedByte* UnsignedBytePtr;

class String : public

#ifdef _UNICODE
   std::wstring
#else
   std::string
#endif /* _UNICODE */

{
   typedef

      #ifdef _UNICODE
   std::wstring
#else
   std::string
#endif /* _UNICODE */

      BaseType;

public:
   String();
   String(const BaseType& s);
   String(const OGPS_Character* s);
   ~String();

   const char* ToChar();

#ifdef _UNICODE
private:
   char* m_Chars;
#endif /* _UNICODE */

}; /* class String */

} /* namespace OpenGPS */

#endif	/* _OPENGPS_HXX */
