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

#include "data_point_impl.hxx"

#include <opengps/cxx/point_vector.hxx>

#include "stdafx.hxx"

PointVectorBase::PointVectorBase()
{
}

PointVectorBase::~PointVectorBase()
{
}

PointVector::PointVector() : PointVectorBase()
{
   m_X = new DataPointImpl();
   m_Y = new DataPointImpl();
   m_Z = new DataPointImpl();
}
PointVector::~PointVector()
{
   delete m_X;
   delete m_Y;
   delete m_Z;
}

const DataPoint* PointVector::GetX() const
{
   _ASSERT(m_X);
   return m_X;
}
const DataPoint* PointVector::GetY() const
{
   _ASSERT(m_Y);
   return m_Y;
}
const DataPoint* PointVector::GetZ() const
{
   _ASSERT(m_Z);
   return m_Z;
}

DataPoint* PointVector::GetX()
{
   _ASSERT(m_X);
   return m_X;
}
DataPoint* PointVector::GetY()
{
   _ASSERT(m_Y);
   return m_Y;
}
DataPoint* PointVector::GetZ()
{
   _ASSERT(m_Z);
   return m_Z;
}

OGPS_Boolean PointVector::GetX(OGPS_Int16* const value) const
{
   _ASSERT(value && m_X);

   return m_X->Get(value);
}

OGPS_Boolean PointVector::GetX(OGPS_Int32* const value) const
{
   _ASSERT(value && m_X);

   return m_X->Get(value);
}

OGPS_Boolean PointVector::GetX(OGPS_Float* const value) const
{
   _ASSERT(value && m_X);

   return m_X->Get(value);
}

OGPS_Boolean PointVector::GetX(OGPS_Double* const value) const
{
   _ASSERT(value && m_X);

   return m_X->Get(value);
}

OGPS_Boolean PointVector::GetY(OGPS_Int16* const value) const
{
   _ASSERT(value && m_Y);

   return m_Y->Get(value);
}

OGPS_Boolean PointVector::GetY(OGPS_Int32* const value) const
{
   _ASSERT(value && m_Y);

   return m_Y->Get(value);
}

OGPS_Boolean PointVector::GetY(OGPS_Float* const value) const
{
   _ASSERT(value && m_Y);

   return m_Y->Get(value);
}

OGPS_Boolean PointVector::GetY(OGPS_Double* const value) const
{
   _ASSERT(value && m_Y);

   return m_Y->Get(value);
}

OGPS_Boolean PointVector::GetZ(OGPS_Int16* const value) const
{
   _ASSERT(value && m_Z);

   return m_Z->Get(value);
}

OGPS_Boolean PointVector::GetZ(OGPS_Int32* const value) const
{
   _ASSERT(value && m_Z);

   return m_Z->Get(value);
}

OGPS_Boolean PointVector::GetZ(OGPS_Float* const value) const
{
   _ASSERT(value && m_Z);

   return m_Z->Get(value);
}

OGPS_Boolean PointVector::GetZ(OGPS_Double* const value) const
{
   _ASSERT(value && m_Z);

   return m_Z->Get(value);
}

void PointVector::SetX(const OGPS_Int16 value)
{
   _ASSERT(m_X);
   m_X->Set(value);
}
void PointVector::SetX(const OGPS_Int32 value)
{
   _ASSERT(m_X);
   m_X->Set(value);
}
void PointVector::SetX(const OGPS_Float value)
{
   _ASSERT(m_X);
   m_X->Set(value);
}
void PointVector::SetX(const OGPS_Double value)
{
   _ASSERT(m_X);
   m_X->Set(value);
}

void PointVector::SetY(const OGPS_Int16 value)
{
   _ASSERT(m_Y);
   m_Y->Set(value);
}
void PointVector::SetY(const OGPS_Int32 value)
{
   _ASSERT(m_Y);
   m_Y->Set(value);
}
void PointVector::SetY(const OGPS_Float value)
{
   _ASSERT(m_Y);
   m_Y->Set(value);
}
void PointVector::SetY(const OGPS_Double value)
{
   _ASSERT(m_Y);
   m_Y->Set(value);
}

void PointVector::SetZ(const OGPS_Int16 value)
{
   _ASSERT(m_Z);
   m_Z->Set(value);
}
void PointVector::SetZ(const OGPS_Int32 value)
{
   _ASSERT(m_Z);
   m_Z->Set(value);
}
void PointVector::SetZ(const OGPS_Float value)
{
   _ASSERT(m_Z);
   m_Z->Set(value);
}
void PointVector::SetZ(const OGPS_Double value)
{
   _ASSERT(m_Z);
   m_Z->Set(value);
}


void PointVector::GetXYZ(
                         OGPS_Double* const x,
                         OGPS_Double* const y,
                         OGPS_Double* const z) const
{
   if(x != NULL)
   {
      _ASSERT(m_X);
      *x = m_X->Get();
   }

   if(y != NULL)
   {
      _ASSERT(m_Y);
      *y = m_Y->Get();
   }

   if(z != NULL)
   {
      _ASSERT(m_Z);
      *z = m_Z->Get();
   }
}

OGPS_Boolean PointVector::IsValid() const
{
   _ASSERT(m_X && m_Y && m_Z);
   return /*m_X->IsValid() && m_Y->IsValid() && */m_Z->IsValid();
}

PointVector& PointVector::operator=(const PointVector& src)
{
   _ASSERT(m_X && m_Y && m_Z);
   _ASSERT(src.m_X && src.m_Y && src.m_Z);

   Set(src);

   return *this;
}

OGPS_Boolean PointVector::SetNull()
{
   _ASSERT(m_X && m_Y && m_Z);

   return m_X->SetNull() & m_Y->SetNull() & m_Z->SetNull();
}

OGPS_Boolean PointVector::Set(const PointVectorBase& value)
{
   _ASSERT(m_X && m_Y && m_Z);

   _ASSERT(value.GetX() && value.GetY() && value.GetZ());

   return (m_X->Set(*value.GetX()) &&
      m_Y->Set(*value.GetY()) &&
      m_Z->Set(*value.GetZ()));
}

OGPS_Boolean PointVector::Get(PointVectorBase& value) const
{
   _ASSERT(m_X && m_Y && m_Z);

   _ASSERT(value.GetX() && value.GetY() && value.GetZ());

   return (value.GetX()->Set(*m_X) &&
      value.GetY()->Set(*m_Y) &&
      value.GetZ()->Set(*m_Z));
}
