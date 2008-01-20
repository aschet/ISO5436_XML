/***************************************************************************
 *   Copyright by Johannes Herwig (NanoFocus AG) 2007                      *
 *   Copyright by Georg Wiora (NanoFocus AG) 2007                          *
 *                                                                         *
 *   This file is part of the openGPS (R)[TM] software library.            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License (LGPL)    *
 *   as published by the Free Software Foundation; either version 3 of     *
 *   the License, or (at your option) any later version.                   *
 *   for detail see the files "licence_LGPL-3.0.txt" and                   *
 *   "licence_GPL-3.0.txt".                                                *
 *                                                                         *
 *   openGPS is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 *   The name "openGPS" and the logo are registered as                     *
 *   European trade mark No. 006178354 for                                 *
 *   Physikalisch Technische Bundesanstalt (PTB)                           *
 *   http://www.ptb.de/                                                    *
 *                                                                         *
 *   More information about openGPS can be found at                        *
 *   http://www.opengps.eu/                                                *
 ***************************************************************************/

#ifndef _OPENGPS_CXX_OPENGPS_HXX
#define _OPENGPS_CXX_OPENGPS_HXX

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

   typedef char OGPS_ExceptionChar;

   class _OPENGPS_EXPORT String : public

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
      String(const OGPS_Character* const s);
      ~String();

      const char* ToChar();
      void FromChar(const char* const s);

      OGPS_Boolean ConvertToMd5(OpenGPS::UnsignedByte md5[16]) const;
      OGPS_Boolean ConvertFromMd5(const OpenGPS::UnsignedByte md5[16]);

      size_t CopyTo(OGPS_Character* const target, const size_t size) const;

#ifdef _UNICODE
   private:
      char* m_Chars;
#endif /* _UNICODE */

   }; /* classOpenGPS::String */

   class _OPENGPS_EXPORT Info
   {
   public:
      static void GetName(OpenGPS::String* const text);
      static void GetDescription(OpenGPS::String* const text);
      static void GetVersion(OpenGPS::String* const text);

      static void PrintVersion();

      static void GetCopyright(OpenGPS::String* const text);
      static void GetLicense(OpenGPS::String* const text);

      static void PrintCopyright();
      static void PrintLicense();

   private:
      Info();
      ~Info();
   };

} /* namespace OpenGPS */

#endif	/* _OPENGPS_CXX_OPENGPS_HXX */
