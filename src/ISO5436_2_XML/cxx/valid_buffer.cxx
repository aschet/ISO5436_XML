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

#include "valid_buffer.hxx"
#include "point_buffer.hxx"
#include "stdafx.hxx"

#include <opengps/cxx/exceptions.hxx>

ValidBuffer::ValidBuffer(PointBuffer* const value)
   : PointValidityProvider(value)
{
   m_ValidityBuffer = NULL;
   m_RawSize = 0;
}

ValidBuffer::~ValidBuffer()
{
   Reset();
}

void ValidBuffer::Allocate() throw(...)
{
   _ASSERT(!m_ValidityBuffer && m_RawSize == 0 && GetPointBuffer());

   const unsigned int size = GetPointBuffer()->GetSize();

   if(size > 0)
   {
      unsigned int rawSize = size / 8;

      if(rawSize * 8 < size)
      {
         ++rawSize;
      }

      AllocateRaw(rawSize);
   }
}

void ValidBuffer::AllocateRaw(const unsigned int rawSize) throw(...)
{
   _ASSERT(!m_ValidityBuffer);

   m_ValidityBuffer = (OpenGPS::UnsignedBytePtr)malloc(rawSize);

   if(!m_ValidityBuffer)
   {
      throw OpenGPS::Exception(
         OGPS_ExGeneral,
         _EX_T("Attempt to allocate memory failed."),
         _EX_T("Could not allocate memory using malloc. This usually means there has not been left enough virtual memory on your system to fulfill the request. You may close some applications currently running and then try again or upgrade your computer hardware."),
         _EX_T("ValidBuffer::AllocateRaw"));
   }

   _VERIFY(memset(m_ValidityBuffer, 255, rawSize));
   m_RawSize = rawSize;
}

void ValidBuffer::Reset()
{
   _OPENGPS_FREE(m_ValidityBuffer);

   m_RawSize = 0;
}

OGPS_Boolean ValidBuffer::IsAllocated() const
{
   return (m_ValidityBuffer != NULL);
}

void ValidBuffer::SetValid(const unsigned int index, const OGPS_Boolean value) throw(...)
{
   _ASSERT(m_ValidityBuffer);
   _ASSERT(index < GetPointBuffer()->GetSize());

   unsigned int bytePosition = index / 8;
   unsigned int bitPosition = index % 8;

   OpenGPS::UnsignedByte bitValue = (OpenGPS::UnsignedByte)(((OpenGPS::UnsignedByte)1) << bitPosition);

   OpenGPS::UnsignedBytePtr rawByte = &m_ValidityBuffer[bytePosition];

   if(value)
   {
      *rawByte |= bitValue;
   }
   else
   {
      *rawByte &= ~bitValue;
   }
}

OGPS_Boolean ValidBuffer::IsValid(const unsigned int index) const throw(...)
{
   if(!m_ValidityBuffer)
   {
      return TRUE;
   }

   _ASSERT(index < GetPointBuffer()->GetSize());

   unsigned int bytePosition = index / 8;
   unsigned int bitPosition = index % 8;

   OpenGPS::UnsignedByte bitValue = (OpenGPS::UnsignedByte)(((OpenGPS::UnsignedByte)1) << bitPosition);

   OpenGPS::UnsignedBytePtr rawByte = &m_ValidityBuffer[bytePosition];

   return ((*rawByte & bitValue) != 0);
}

OGPS_Boolean ValidBuffer::Read(std::basic_istream<OpenGPS::UnsignedByte>& stream) throw(...)
{
   // get length of file:
   stream.seekg (0, std::ios::end);
   if(!stream.fail())
   {
      const unsigned int length = stream.tellg();
      if(!stream.fail())
      {
         stream.seekg (0, std::ios::beg);
         if(!stream.fail())
         {
            AllocateRaw(length);

            // read data as a block
            stream.read ((OpenGPS::UnsignedBytePtr)m_ValidityBuffer,length);
            if(!stream.fail())
            {
               return TRUE;
            }
         }
      }
   }

   Reset();

   return FALSE;
}

OGPS_Boolean ValidBuffer::Write(std::ostream& stream)
{
   stream.write((const char*)m_ValidityBuffer, m_RawSize);
   return !stream.fail();
}

Int16ValidBuffer::Int16ValidBuffer(PointBuffer* const value)
: ValidBuffer(value)
{
   _ASSERT(value && value->GetType() == OGPS_Int16PointType);
}


Int16ValidBuffer::~Int16ValidBuffer()
{
}

void Int16ValidBuffer::SetValid(const unsigned int index, const OGPS_Boolean value) throw(...)
{
   ValidBuffer::SetValid(index, value);

   if(!value)
   {
      const OGPS_Int16 invalid = 0;
      GetPointBuffer()->Set(index, invalid);
   }
}

Int32ValidBuffer::Int32ValidBuffer(PointBuffer* const value)
: ValidBuffer(value)
{
   _ASSERT(value && value->GetType() == OGPS_Int32PointType);
}

Int32ValidBuffer::~Int32ValidBuffer()
{
}

void Int32ValidBuffer::SetValid(const unsigned int index, const OGPS_Boolean value) throw(...)
{
   ValidBuffer::SetValid(index, value);

   if(!value)
   {
      const OGPS_Int32 invalid = 0;
      GetPointBuffer()->Set(index, invalid);
   }
}
