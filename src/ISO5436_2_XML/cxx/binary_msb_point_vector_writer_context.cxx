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

#include "binary_msb_point_vector_writer_context.hxx"
#include "point_vector_iostream.hxx"

#include "environment.hxx"

#include "stdafx.hxx"

BinaryMSBPointVectorWriterContext::BinaryMSBPointVectorWriterContext(zipFile handle)
: BinaryPointVectorWriterContext(handle)
{
}

BinaryMSBPointVectorWriterContext::~BinaryMSBPointVectorWriterContext()
{
   Environment::Reset();
}

OGPS_Boolean BinaryMSBPointVectorWriterContext::Write(const OGPS_Int16* const value)
{
   _ASSERT(value);

   if(HasStream())
   {
      _ASSERT(sizeof(*value) >= _OPENGPS_BINFORMAT_INT16_SIZE);
      _ASSERT(_OPENGPS_BINFORMAT_INT16_SIZE == 2);

      OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_INT16_SIZE];
      GetStream()->write((const char*)Environment::GetInstance()->ByteSwap16(value, &buffer[0]), _OPENGPS_BINFORMAT_INT16_SIZE);

      return IsGood();
   }

   return FALSE;
}

OGPS_Boolean BinaryMSBPointVectorWriterContext::Write(const OGPS_Int32* const value)
{
   _ASSERT(value);

   if(HasStream())
   {
      _ASSERT(sizeof(*value) >= _OPENGPS_BINFORMAT_INT32_SIZE);
      _ASSERT(_OPENGPS_BINFORMAT_INT32_SIZE == 4);

      OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_INT32_SIZE];
      GetStream()->write((const char*)Environment::GetInstance()->ByteSwap32(value, &buffer[0]), _OPENGPS_BINFORMAT_INT32_SIZE);

      return IsGood();
   }

   return FALSE;
}

OGPS_Boolean BinaryMSBPointVectorWriterContext::Write(const OGPS_Float* const value)
{
   _ASSERT(value);

   if(HasStream())
   {
      _ASSERT(sizeof(*value) >= _OPENGPS_BINFORMAT_FLOAT_SIZE);
      _ASSERT(_OPENGPS_BINFORMAT_FLOAT_SIZE == 4);

      // TODO: assumption (OpenGPS::Byte cut) valid with type float???
      OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_FLOAT_SIZE];
      GetStream()->write((const char*)Environment::GetInstance()->ByteSwap32(value, &buffer[0]), _OPENGPS_BINFORMAT_FLOAT_SIZE);

      return IsGood();
   }

   return FALSE;
}

OGPS_Boolean BinaryMSBPointVectorWriterContext::Write(const OGPS_Double* const value)
{
   _ASSERT(value);

   if(HasStream())
   {
      _ASSERT(sizeof(*value) >= _OPENGPS_BINFORMAT_DOUBLE_SIZE);
      _ASSERT(_OPENGPS_BINFORMAT_DOUBLE_SIZE == 8);

      // TODO: assumption (OpenGPS::Byte cut) valid with type double???
      OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_DOUBLE_SIZE];
      GetStream()->write((const char*)Environment::GetInstance()->ByteSwap64(value, &buffer[0]), _OPENGPS_BINFORMAT_DOUBLE_SIZE);

      return IsGood();
   }

   return FALSE;
}
