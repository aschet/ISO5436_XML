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

#ifndef _OPENGPS_VALID_BUFFER_HXX
#define _OPENGPS_VALID_BUFFER_HXX

#ifndef _OPENGPS_HXX
#  include <opengps/opengps.hxx>
#endif

#include <iostream>

namespace OpenGPS
{
   class ValidBuffer
   {
   public:
      ValidBuffer();
      ~ValidBuffer();

      virtual OGPS_Boolean Allocate(const unsigned int size);
      virtual OGPS_Boolean IsAllocated() const;

      virtual OGPS_Boolean Read(std::basic_istream<OpenGPS::UnsignedByte>& stream, const unsigned int pointCount);
      virtual OGPS_Boolean Write(std::ostream& stream);

      virtual OGPS_Boolean SetValid(const unsigned int index, const OGPS_Boolean value);
      virtual OGPS_Boolean IsValid(const unsigned int index) const;

   protected:
      virtual OGPS_Boolean AllocateRaw(const unsigned int rawSize);
      virtual void Reset();

   private:
      OpenGPS::UnsignedBytePtr m_Valid;
      unsigned long m_Size; /* logically */
      unsigned long m_RawSize; /* real buffer size in bytes */
   };
}

#endif /* _OPENGPS_VALID_BUFFER_HXX */