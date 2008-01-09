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

#ifndef _OPENGPS_POINT_VECTOR_IOSTREAM_HXX
#define _OPENGPS_POINT_VECTOR_IOSTREAM_HXX

#ifndef _OPENGPS_CXX_OPENGPS_HXX
#  include <opengps/cxx/opengps.hxx>
#endif

#include <xlocale>
#include <sstream>
#include <fstream>

// TODO: std::* typedefs (wchar_t/char)

namespace OpenGPS
{
   class PointVectorWhitespaceFacet : public std::ctype<OGPS_Character>
   {
      typedef std::ctype<OGPS_Character> BaseType;

      public:
         PointVectorWhitespaceFacet(const size_t refs = 0);
         ~PointVectorWhitespaceFacet();

   protected:
      virtual bool do_is(mask msk, OGPS_Character ch) const;
   };

   class PointVectorInvariantLocale : public std::locale
   {
      typedef std::locale BaseType;

   public:
      PointVectorInvariantLocale();
      ~PointVectorInvariantLocale();

      static const PointVectorInvariantLocale& GetInstance();

   private:
      static PointVectorInvariantLocale m_Instance;
   };

   class PointVectorInputStringStream : public std::basic_istringstream<OGPS_Character>
   {
      typedef std::basic_istringstream<OGPS_Character> BaseType;

   public:
      PointVectorInputStringStream();
      PointVectorInputStringStream(const OpenGPS::String& s);
      ~PointVectorInputStringStream();
   };

   class PointVectorOutputStringStream : public std::basic_ostringstream<OGPS_Character>
   {
      typedef std::basic_ostringstream<OGPS_Character> BaseType;

   public:
      PointVectorOutputStringStream();
      ~PointVectorOutputStringStream();
   };

   class PointVectorInputBinaryFileStream : public std::basic_ifstream<OpenGPS::UnsignedByte>
   {
      typedef std::basic_ifstream<OpenGPS::UnsignedByte> BaseType;

   public:
      PointVectorInputBinaryFileStream(const OpenGPS::String& filePath);
      ~PointVectorInputBinaryFileStream();
   };

   class PointVectorOutputBinaryFileStream : public std::basic_ofstream<OpenGPS::UnsignedByte>
   {
      typedef std::basic_ofstream<OpenGPS::UnsignedByte> BaseType;

   public:
      PointVectorOutputBinaryFileStream(const OpenGPS::String& filePath);
      ~PointVectorOutputBinaryFileStream();
   };
}

#endif /* _OPENGPS_POINT_VECTOR_IOSTREAM_HXX */
