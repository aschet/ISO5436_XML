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

#include "point_buffer.hxx"
#include "stdafx.hxx"

#include <stdlib.h>

PointBuffer::PointBuffer()
{
   m_Size = 0;
}

PointBuffer::~PointBuffer()
{
}

OGPS_Boolean PointBuffer::Set(const unsigned long index, const short value)
{
   return FALSE;
}

OGPS_Boolean PointBuffer::Set(const unsigned long index, const int value)
{
   return FALSE;
}

OGPS_Boolean PointBuffer::Set(const unsigned long index, const float value)
{
   return FALSE;
}

OGPS_Boolean PointBuffer::Set(const unsigned long index, const double value)
{
   return FALSE;
}

OGPS_Boolean PointBuffer::Get(const unsigned long index, short& value) const
{
   return FALSE;
}

OGPS_Boolean PointBuffer::Get(const unsigned long index, int& value) const
{
   return FALSE;
}

OGPS_Boolean PointBuffer::Get(const unsigned long index, float& value) const
{
   return FALSE;
}

OGPS_Boolean PointBuffer::Get(const unsigned long index, double& value) const
{
   return FALSE;
}

unsigned long PointBuffer::GetSize() const
{
   return m_Size;
}

OpenGPS::UnsignedBytePtr PointBuffer::Allocate(const unsigned long size, const size_t typeSize)
{
      _ASSERT(m_Size == 0);

      OpenGPS::UnsignedBytePtr buffer = (OpenGPS::UnsignedBytePtr)malloc(size * typeSize);
      if(buffer)
      {
         m_Size = size;
      }

      return buffer;
}

void PointBuffer::Free(OpenGPS::UnsignedBytePtr* value)
{
   if(*value)
   {
      free(*value);
      *value = NULL;
   }
}

OGPS_Boolean PointBuffer::Allocate(const unsigned long size)
{
   return FALSE;
}

OGPS_DataPointType PointBuffer::GetType() const
{
   return OGPS_MissingPointType;
}
