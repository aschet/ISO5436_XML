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
   m_Type = OGPS_MissingPointType;
}

DataPointImpl::~DataPointImpl()
{
}

OGPS_DataPointType DataPointImpl::GetType() const
{
   return m_Type;
}

OGPS_Boolean DataPointImpl::Get(OGPS_Int16* const value) const
{
   _ASSERT(value);

   if(m_Type == OGPS_Int16PointType)
   {
      *value = m_Value.int16Value;
      return TRUE;
   }

   return FALSE;
}

OGPS_Boolean DataPointImpl::Get(OGPS_Int32* const value) const
{
   _ASSERT(value);

   if(m_Type == OGPS_Int32PointType)
   {
      *value = m_Value.int32Value;
      return TRUE;
   }

   return FALSE;
}

OGPS_Boolean DataPointImpl::Get(OGPS_Float* const value) const
{
   _ASSERT(value);

   if(m_Type == OGPS_FloatPointType)
   {
      *value = m_Value.floatValue;
      return TRUE;
   }

   return FALSE;
}

OGPS_Boolean DataPointImpl::Get(OGPS_Double* const value) const
{
   _ASSERT(value);

   if(m_Type == OGPS_DoublePointType)
   {
      *value = m_Value.doubleValue;
      return TRUE;
   }

   return FALSE;
}

OGPS_Double DataPointImpl::Get() const
{
   switch(m_Type)
   {
   case OGPS_Int16PointType:
      return m_Value.int16Value;

   case OGPS_Int32PointType:
      return m_Value.int32Value;

   case OGPS_FloatPointType:
      return m_Value.floatValue;

   case OGPS_DoublePointType:
      return m_Value.doubleValue;

   case OGPS_MissingPointType:
      _ASSERT(FALSE);
      return 0.0;

   default:
      break;
   }

   _ASSERT(FALSE);
   return 0.0;
}

OGPS_Boolean DataPointImpl::Set(const OGPS_Int16 value)
{
   m_Type = OGPS_Int16PointType;
   m_Value.int16Value = value;
   return TRUE;
}
OGPS_Boolean DataPointImpl::Set(const OGPS_Int32 value)
{
   m_Type = OGPS_Int32PointType;
   m_Value.int32Value = value;
   return TRUE;
}
OGPS_Boolean DataPointImpl::Set(const OGPS_Float value)
{
   m_Type = OGPS_FloatPointType;
   m_Value.floatValue = value;
   return TRUE;
}
OGPS_Boolean DataPointImpl::Set(const OGPS_Double value)
{
   m_Type = OGPS_DoublePointType;
   m_Value.doubleValue = value;
   return TRUE;
}

OGPS_Boolean DataPointImpl::IsValid() const
{
   return m_Type != OGPS_MissingPointType;
}

OGPS_Boolean DataPointImpl::Set(const DataPoint& src)
{
   /* I do not use memcpy or the like here, since possibly there are different memory models
   implemented by different compiler/architecture combinations concerning the union data type,
   allowing the compiler to allocate the size of unions dynamically . Therefore a simple but
   efficient memcpy might lead to unexpected results - I guess. */
   switch(src.GetType())
   {
   case OGPS_Int16PointType:
      OGPS_Int16 vs;
      _VERIFY(src.Get(&vs));
      _VERIFY(Set(vs));
      break;

   case OGPS_Int32PointType:
      OGPS_Int32 vl;
      _VERIFY(src.Get(&vl));
      _VERIFY(Set(vl));
      break;

   case OGPS_FloatPointType:
      OGPS_Float vf;
      _VERIFY(src.Get(&vf));
      _VERIFY(Set(vf));
      break;

   case OGPS_DoublePointType:
      OGPS_Double vd;
      _VERIFY(src.Get(&vd));
      _VERIFY(Set(vd));
      break;

   case OGPS_MissingPointType:
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
   m_Type = OGPS_MissingPointType;
   m_Value.doubleValue = 0.0;
}
