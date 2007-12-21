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
#include "stdafx.hxx"

DataPoint::DataPoint()
{
}

DataPoint::~DataPoint()
{
}

DataPointImpl::DataPointImpl()
: DataPoint()
{
   m_Type = MissingPointType;
}

DataPointImpl::~DataPointImpl()
{
}

OGPS_DataPointType DataPointImpl::GetType() const
{
   return m_Type;
}

OGPS_Boolean DataPointImpl::Get(short* const value) const
{
   _ASSERT(value);

   if(m_Type == Int16PointType)
   {
      *value = m_Value.int16Value;
      return TRUE;
   }

   return FALSE;
}

OGPS_Boolean DataPointImpl::Get(int* const value) const
{
   _ASSERT(value);

   if(m_Type == Int32PointType)
   {
      *value = m_Value.int32Value;
      return TRUE;
   }

   return FALSE;
}

OGPS_Boolean DataPointImpl::Get(float* const value) const
{
   _ASSERT(value);

   if(m_Type == FloatPointType)
   {
      *value = m_Value.floatValue;
      return TRUE;
   }

   return FALSE;
}

OGPS_Boolean DataPointImpl::Get(double* const value) const
{
   _ASSERT(value);

   if(m_Type == DoublePointType)
   {
      *value = m_Value.doubleValue;
      return TRUE;
   }

   return FALSE;
}

double DataPointImpl::Get() const
{
   switch(m_Type)
   {
   case Int16PointType:
      return m_Value.int16Value;

   case Int32PointType:
      return m_Value.int32Value;

   case FloatPointType:
      return m_Value.floatValue;

   case DoublePointType:
      return m_Value.doubleValue;

   case MissingPointType:
      _ASSERT(FALSE);
      return 0.0;

   default:
      break;
   }

   _ASSERT(FALSE);
   return 0.0;
}

OGPS_Boolean DataPointImpl::Set(const short value)
{
   m_Type = Int16PointType;
   m_Value.int16Value = value;
   return TRUE;
}
OGPS_Boolean DataPointImpl::Set(const int value)
{
   m_Type = Int32PointType;
   m_Value.int32Value = value;
   return TRUE;
}
OGPS_Boolean DataPointImpl::Set(const float value)
{
   m_Type = FloatPointType;
   m_Value.floatValue = value;
   return TRUE;
}
OGPS_Boolean DataPointImpl::Set(const double value)
{
   m_Type = DoublePointType;
   m_Value.doubleValue = value;
   return TRUE;
}

OGPS_Boolean DataPointImpl::SetNull()
{
   switch(m_Type)
   {
   case Int16PointType:
      m_Value.int16Value = 0;
      break;
   case Int32PointType:
      m_Value.int32Value = 0;
      break;
   case FloatPointType:
      m_Value.floatValue = 0.0F;
      break;
   case DoublePointType:
      m_Value.doubleValue = 0.0;
      break;
   case MissingPointType:
      return FALSE;

   default:
      return FALSE;
   }

   return TRUE;
}

OGPS_Boolean DataPointImpl::IsValid() const
{
   return m_Type != MissingPointType;
}

OGPS_Boolean DataPointImpl::Set(const DataPoint& src)
{
   /* I do not use memcpy or the like here, since possibly there are different memory models
   implemented by different compiler/architecture combinations concerning the union data type,
   allowing the compiler to allocate the size of unions dynamically . Therefore a simple but
   efficient memcpy might lead to unexpected results - I guess. */
   switch(src.GetType())
   {
   case Int16PointType:
      short vs;
      _VERIFY(src.Get(&vs));
      _VERIFY(Set(vs));
      break;

   case Int32PointType:
      int vl;
      _VERIFY(src.Get(&vl));
      _VERIFY(Set(vl));
      break;

   case FloatPointType:
      float vf;
      _VERIFY(src.Get(&vf));
      _VERIFY(Set(vf));
      break;

   case DoublePointType:
      double vd;
      _VERIFY(src.Get(&vd));
      _VERIFY(Set(vd));
      break;

   case MissingPointType:
      Reset();
      break;

   default:
      _ASSERT(FALSE);
      Reset();
      return FALSE;
   }

   return TRUE;
}

void DataPointImpl::Reset()
{
   m_Type = MissingPointType;
}
