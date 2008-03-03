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
#include <opengps/cxx/exceptions.hxx>
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

OGPS_DataPointType DataPointImpl::GetType() const throw(...)
{
   return m_Type;
}

void DataPointImpl::Get(OGPS_Int16* const value) const throw(...)
{
   _ASSERT(value);

   if(m_Type != OGPS_Int16PointType)
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("Could not obtain the value of the data point."),
         _EX_T("An attempt to read a point value of type OGPS_Int16 is made, but the current value is of different type."),
         _EX_T("OpenGPS::DataPointImpl::Get"));
   }

   *value = m_Value.int16Value;   
}

void DataPointImpl::Get(OGPS_Int32* const value) const throw(...)
{
   _ASSERT(value);

   if(m_Type != OGPS_Int32PointType)
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("Could not obtain the value of the data point."),
         _EX_T("An attempt to read a point value of type OGPS_Int32 is made, but the current value is of different type."),
         _EX_T("OpenGPS::DataPointImpl::Get"));
   }

   *value = m_Value.int32Value;
}

void DataPointImpl::Get(OGPS_Float* const value) const throw(...)
{
   _ASSERT(value);

   if(m_Type != OGPS_FloatPointType)
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("Could not obtain the value of the data point."),
         _EX_T("An attempt to read a point value of type OGPS_Float is made, but the current value is of different type."),
         _EX_T("OpenGPS::DataPointImpl::Get"));
   }

   *value = m_Value.floatValue;
}

void DataPointImpl::Get(OGPS_Double* const value) const throw(...)
{
   _ASSERT(value);

   if(m_Type != OGPS_DoublePointType)
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("Could not obtain the value of the data point."),
         _EX_T("An attempt to read a point value of type OGPS_Double is made, but the current value is of different type."),
         _EX_T("OpenGPS::DataPointImpl::Get"));
   }

   *value = m_Value.doubleValue;   
}

OGPS_Double DataPointImpl::Get() const throw(...)
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
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("Could not obtain the value of the data point."),
         _EX_T("The value of the type property equals OGPS_MissingPointType. There is no point data available to be read."),
         _EX_T("OpenGPS::DataPointImpl::Get"));

   default:
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("Could not obtain the value of the data point."),
         _EX_T("The value of the type property of this instance is unknown or invalid. It should be one of the following: OGPS_Int16PointType, OGPS_Int32PointType, OGPS_FloatPointType, OGPS_DoublePointType."),
         _EX_T("OpenGPS::DataPointImpl::Get"));
   }
}

void DataPointImpl::Set(const OGPS_Int16 value) throw(...)
{
   m_Type = OGPS_Int16PointType;
   m_Value.int16Value = value;
}

void DataPointImpl::Set(const OGPS_Int32 value) throw(...)
{
   m_Type = OGPS_Int32PointType;
   m_Value.int32Value = value;
}

void DataPointImpl::Set(const OGPS_Float value) throw(...)
{
   m_Type = OGPS_FloatPointType;
   m_Value.floatValue = value;
}

void DataPointImpl::Set(const OGPS_Double value) throw(...)
{
   m_Type = OGPS_DoublePointType;
   m_Value.doubleValue = value;
}

OGPS_Boolean DataPointImpl::IsValid() const throw(...)
{
   return m_Type != OGPS_MissingPointType;
}

void DataPointImpl::Set(const DataPoint& src) throw(...)
{
   /* I do not use memcpy or the like here, since possibly there are different memory models
   implemented by different combinations of compiler/architecture concerning the union data type,
   allowing the compiler to allocate the size of unions dynamically. Therefore a simple but
   efficient memcpy might lead to unexpected results. */
   switch(src.GetType())
   {
   case OGPS_Int16PointType:
      OGPS_Int16 vs;
      src.Get(&vs);
      Set(vs);
      break;

   case OGPS_Int32PointType:
      OGPS_Int32 vl;
      src.Get(&vl);
      Set(vl);
      break;

   case OGPS_FloatPointType:
      OGPS_Float vf;
      src.Get(&vf);
      Set(vf);
      break;

   case OGPS_DoublePointType:
      OGPS_Double vd;
      src.Get(&vd);
      Set(vd);
      break;

   case OGPS_MissingPointType:
      Reset();
      break;

   default:
      throw OpenGPS::Exception(
         OGPS_ExInvalidArgument,
         _EX_T("Could not copy from the given instance of type data point."),
         _EX_T("The value of the type property of the argument given is unknown. It should be one of the following: OGPS_Int16PointType, OGPS_Int32PointType, OGPS_FloatPointType, OGPS_DoublePointType, OGPS_MissingPointType."),
         _EX_T("OpenGPS::DataPointImpl::Set"));
      break;
   }
}

void DataPointImpl::Reset() throw(...)
{
   m_Type = OGPS_MissingPointType;
   m_Value.doubleValue = 0.0;
}
