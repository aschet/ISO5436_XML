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

#include "vector_buffer_builder.hxx"
#include "vector_buffer.hxx"

#include "int16_point_buffer.hxx"
#include "int32_point_buffer.hxx"
#include "float_point_buffer.hxx"
#include "double_point_buffer.hxx"

#include "stdafx.hxx"

VectorBufferBuilder::VectorBufferBuilder()
{
   m_Buffer = NULL;
}

VectorBufferBuilder::~VectorBufferBuilder()
{
   if(m_Buffer)
   {
      delete m_Buffer;
   }
}

OGPS_Boolean VectorBufferBuilder::BuildBuffer()
{
   _ASSERT(!m_Buffer);

   m_Buffer = new VectorBuffer();

   return TRUE;
}

OGPS_Boolean VectorBufferBuilder::BuildX(const OGPS_DataPointType dataType, const unsigned long size)
{
   _ASSERT(m_Buffer);

   OGPS_Boolean success = FALSE;
   m_Buffer->SetX(CreatePointBuffer(dataType, size, &success));
   return success;
}

OGPS_Boolean VectorBufferBuilder::BuildY(const OGPS_DataPointType dataType, const unsigned long size)
{
   _ASSERT(m_Buffer);

   OGPS_Boolean success = FALSE;
   m_Buffer->SetY(CreatePointBuffer(dataType, size, &success));
   return success;
}

OGPS_Boolean VectorBufferBuilder::BuildZ(const OGPS_DataPointType dataType, const unsigned long size)
{
   _ASSERT(m_Buffer);

   OGPS_Boolean success = FALSE;
   m_Buffer->SetZ(CreatePointBuffer(dataType, size, &success));
   return success;
}

OGPS_Boolean VectorBufferBuilder::BuildValid(const unsigned long size)
{
   _ASSERT(m_Buffer);

   ValidBuffer* valid = new ValidBuffer();
   if(valid->Allocate(size))
   {
   m_Buffer->SetValid(valid);
   return TRUE;
   }

   return FALSE;
}

VectorBuffer* VectorBufferBuilder::GetBuffer()
{
   return m_Buffer;
}

PointBuffer* VectorBufferBuilder::CreatePointBuffer(const OGPS_DataPointType dataType, const unsigned long size, OGPS_Boolean* const retval) const
{
   _ASSERT(retval);

   PointBuffer* point = NULL;

   switch(dataType)
   {
   case Int16PointType:
      point = new Int16PointBuffer();
      *retval = TRUE;
      break;
   case Int32PointType:
      point = new Int32PointBuffer();
      *retval = TRUE;
      break;
   case FloatPointType:
      point = new FloatPointBuffer();
      *retval = TRUE;
      break;
   case DoublePointType:
      point = new DoublePointBuffer();
      *retval = TRUE;
      break;
   case MissingPointType:
      *retval = TRUE;
      break;
   default:
      *retval = FALSE;
      _ASSERT(FALSE);
      break;
   }

   if(point)
   {
      if(!point->Allocate(size))
      {
         delete point;
         point = NULL;

         *retval = FALSE;
      }
   }

   return point;
}
