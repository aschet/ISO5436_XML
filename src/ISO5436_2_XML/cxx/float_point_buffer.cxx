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

#include "float_point_buffer.hxx"
#include "stdafx.hxx"

FloatPointBuffer::FloatPointBuffer() : PointBuffer()
{
   m_Buffer = NULL;
}

FloatPointBuffer::~FloatPointBuffer()
{
      Free((OpenGPS::UnsignedBytePtr*)(&m_Buffer));
}

OGPS_Boolean FloatPointBuffer::Allocate(const unsigned long size)
{
   if(!m_Buffer)
   {
      m_Buffer = (OGPS_Float*)PointBuffer::Allocate(size, sizeof(OGPS_Float));
      
      if(m_Buffer)
      {
         return TRUE;
      }
   }

   return FALSE;
}

OGPS_Boolean FloatPointBuffer::Set(const unsigned long index, const OGPS_Float value)
{
   _ASSERT(index < GetSize() && m_Buffer);

   m_Buffer[index] = value;

   return TRUE;
}

OGPS_Boolean FloatPointBuffer::Get(const unsigned long index, OGPS_Float& value) const
{
   _ASSERT(index < GetSize() && m_Buffer);

   value = m_Buffer[index];

   return TRUE;
}

OGPS_DataPointType FloatPointBuffer::GetType() const
{
   return OGPS_FloatPointType;
}
