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

#include "vector_buffer.hxx"
#include "point_vector_proxy.hxx"

#include "stdafx.hxx"

VectorBuffer::VectorBuffer()
{
   m_X = NULL;
   m_Y = NULL;
   m_Z = NULL;
   m_Valid = NULL;
}

VectorBuffer::~VectorBuffer()
{
   // TODO: maybe we should overload following
   // delete operators, since memory is not
   // allocated within the current class scope?
   if(m_X)
   {
      delete m_X;
   }

   if(m_Y)
   {
      delete m_Y;
   }

   if(m_Z)
   {
      delete m_Z;
   }

   if(m_Valid)
   {
      delete m_Valid;
   }
}

void VectorBuffer::SetX(PointBuffer* const value)
{
   _ASSERT(!m_X);

   m_X = value;
}

void VectorBuffer::SetY(PointBuffer* const value)
{
   _ASSERT(!m_Y);

   m_Y = value;
}

void VectorBuffer::SetZ(PointBuffer* const value)
{
   _ASSERT(!m_Z);

   m_Z = value;
}

void VectorBuffer::SetValid(ValidBuffer* const value)
{
   _ASSERT(!m_Valid);

   m_Valid = value;
}

PointBuffer* VectorBuffer::GetX()
{
   return m_X;
}

PointBuffer* VectorBuffer::GetY()
{
   return m_Y;
}

PointBuffer* VectorBuffer::GetZ()
{
   return m_Z;
}

ValidBuffer* VectorBuffer::GetValid()
{
   return m_Valid;
}

const PointBuffer* VectorBuffer::GetX() const
{
   return m_X;
}

const PointBuffer* VectorBuffer::GetY() const
{
   return m_Y;
}

const PointBuffer* VectorBuffer::GetZ() const
{
   return m_Z;
}

const ValidBuffer* VectorBuffer::GetValid() const
{
   return m_Valid;
}

PointVectorAutoPtr VectorBuffer::GetPointVectorProxy(const PointVectorProxyContext& context)
{
   return PointVectorAutoPtr(new PointVectorProxy(&context, this));
}
