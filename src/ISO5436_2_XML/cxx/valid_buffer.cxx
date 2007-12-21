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
#include "stdafx.hxx"

ValidBuffer::ValidBuffer()
{
   m_Valid = NULL;
   m_Size = 0;
   m_RawSize = 0;
}

ValidBuffer::~ValidBuffer()
{
   Reset();
}

OGPS_Boolean ValidBuffer::Allocate(const unsigned int size)
{
   _ASSERT(!m_Valid && m_Size == 0 && m_RawSize == 0);

   if(size == 0)
   {
      return TRUE;
   }

   unsigned int rawSize = size / 8;
   
   if(rawSize * 8 < size)
   {
      ++rawSize;
   }
  
   if(AllocateRaw(rawSize))
   {
      m_Size = size;

      return TRUE;
   }

   return FALSE;
}

OGPS_Boolean ValidBuffer::AllocateRaw(const unsigned int rawSize)
{
   _ASSERT(!m_Valid);

   m_Valid = (OpenGPS::UnsignedBytePtr)malloc(rawSize);
   
   if(m_Valid)
   {
      _VERIFY(memset(m_Valid, 255, rawSize));

      m_RawSize = rawSize;

      return TRUE;
   }

   return FALSE;
}

void ValidBuffer::Reset()
{
   if(m_Valid)
   {
      free(m_Valid);
   }

   m_Valid = NULL;
   m_Size = 0;
   m_RawSize = 0;
}

OGPS_Boolean ValidBuffer::IsAllocated() const
{
   return (m_Valid != NULL);
}

OGPS_Boolean ValidBuffer::SetValid(const unsigned int index, const OGPS_Boolean value)
{
   if(m_Valid)
   {
   _ASSERT(index < m_Size);

   unsigned int bytePosition = index / 8;
   unsigned int bitPosition = index % 8;

   OpenGPS::UnsignedByte bitValue = (1) << bitPosition;

   OpenGPS::UnsignedBytePtr rawByte = &m_Valid[bytePosition];

   if(value)
   {
      *rawByte |= bitValue;
   }
   else
   {
      *rawByte &= ~bitValue;
   }

   return TRUE;
   }

   return FALSE;
}

OGPS_Boolean ValidBuffer::IsValid(const unsigned int index) const
{
   if(!m_Valid)
   {
      return TRUE;
   }

   _ASSERT(index < m_Size);

   unsigned int bytePosition = index / 8;
   unsigned int bitPosition = index % 8;

   OpenGPS::UnsignedByte bitValue = (1) << bitPosition;

   OpenGPS::UnsignedBytePtr rawByte = &m_Valid[bytePosition];

   return ((*rawByte & bitValue) != 0);
}

OGPS_Boolean ValidBuffer::Read(std::basic_istream<OpenGPS::UnsignedByte>& stream, const unsigned int pointCount)
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
  if(AllocateRaw(length))
  {
  // read data as a block:
     stream.read ((OpenGPS::UnsignedBytePtr)m_Valid,length);
if(!stream.fail())
{
   // set logical size
   m_Size = pointCount;
      return TRUE;
}
  }
   }
  }
   }

   Reset();

   return FALSE;
}

OGPS_Boolean ValidBuffer::Write(std::ostream& stream)
{
   // TODO: LSB only!
   stream.write((const char*)m_Valid, m_RawSize);
      return !stream.fail();

   return FALSE;
}
