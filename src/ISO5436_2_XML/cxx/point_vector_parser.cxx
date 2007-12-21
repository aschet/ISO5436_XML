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

#include "point_vector_parser.hxx"

#include "int16_data_point_parser.hxx"
#include "int32_data_point_parser.hxx"
#include "float_data_point_parser.hxx"
#include "double_data_point_parser.hxx"
#include "missing_data_point_parser.hxx"

#include "point_vector_reader_context.hxx"
#include "point_vector_writer_context.hxx"

#include <opengps/point_vector_base.hxx>

#include "stdafx.hxx"

PointVectorParser::PointVectorParser()
{
   m_X = NULL;
   m_Y = NULL;
   m_Z = NULL;
}

PointVectorParser::~PointVectorParser()
{
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
}

void PointVectorParser::SetX(DataPointParser* value)
{
   _ASSERT(value);

   if(m_X)
   {
      delete m_X;
   }

   m_X = value;
}

void PointVectorParser::SetY(DataPointParser* value)
{
   _ASSERT(value);

   if(m_Y)
   {
      delete m_Y;
   }

   m_Y = value;
}

void PointVectorParser::SetZ(DataPointParser* value)
{
   _ASSERT(value);

   if(m_Z)
   {
      delete m_Z;
   }

   m_Z = value;
}

OGPS_Boolean PointVectorParser::Read(PointVectorReaderContext& context, PointVectorBase& value)
{
   _ASSERT(m_X && m_Y && m_Z);

   return (m_X->Read(context, *value.GetX()) && // parse x component
      m_Y->Read(context, *value.GetY()) && // parse y component
   m_Z->Read(context, *value.GetZ())); // parse z component
}

OGPS_Boolean PointVectorParser::Write(PointVectorWriterContext& context, const PointVectorBase& value)
{
   _ASSERT(m_X && m_Y && m_Z);

   return (m_X->Write(context, *value.GetX()) && // parse x component
      m_Y->Write(context, *value.GetY()) && // parse y component
   m_Z->Write(context, *value.GetZ()));  // parse z component
}

DataPointParser* PointVectorParser::CreateDataPointParser(const OGPS_DataPointType dataType) const
{
   switch(dataType)
   {
   case Int16PointType:
      return new Int16DataPointParser();
   case Int32PointType:
      return new Int32DataPointParser();
   case FloatPointType:
      return new FloatDataPointParser();
   case DoublePointType:
      return new DoubleDataPointParser();
   case MissingPointType:
      return new MissingDataPointParser();
   default:
      _ASSERT(FALSE);
      break;
   }

   return NULL;
}
