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

#include "binary_msb_point_vector_reader_context.hxx"

#include "point_vector_iostream.hxx"
#include "environment.hxx"

#include "stdafx.hxx"

BinaryMSBPointVectorReaderContext::BinaryMSBPointVectorReaderContext(const OpenGPS::String& filePath)
: BinaryPointVectorReaderContext(filePath)
{
}

BinaryMSBPointVectorReaderContext::~BinaryMSBPointVectorReaderContext()
{
   Environment::Reset();
}

OGPS_Boolean BinaryMSBPointVectorReaderContext::Read(OGPS_Int16* const value)
{
   _ASSERT(value);

   if(HasStream())
   {
      OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_INT16_SIZE];
      GetStream()->read(&buffer[0], _OPENGPS_BINFORMAT_INT16_SIZE);

      if(IsGood())
      {
         _ASSERT(_OPENGPS_BINFORMAT_INT16_SIZE == 2); // 2 bytes
         Environment::GetInstance()->ByteSwap16(&buffer[0], value);
         return TRUE;
      }
   }

   return FALSE;
}

OGPS_Boolean BinaryMSBPointVectorReaderContext::Read(OGPS_Int32* const value)
{
   _ASSERT(value);

   if(HasStream())
   {
      OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_INT32_SIZE];
      GetStream()->read(&buffer[0], _OPENGPS_BINFORMAT_INT32_SIZE);

      if(IsGood())
      {
         _ASSERT(_OPENGPS_BINFORMAT_INT32_SIZE == 4); // 4 bytes
         Environment::GetInstance()->ByteSwap32(&buffer[0], value);
         return TRUE;
      }
   }

   return FALSE;
}

OGPS_Boolean BinaryMSBPointVectorReaderContext::Read(OGPS_Float* const value)
{
   _ASSERT(value);

   if(HasStream())
   {
      OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_FLOAT_SIZE];
      GetStream()->read(&buffer[0], _OPENGPS_BINFORMAT_FLOAT_SIZE);

      if(IsGood())
      {
         _ASSERT(_OPENGPS_BINFORMAT_FLOAT_SIZE == 4); // 4 bytes
         Environment::GetInstance()->ByteSwap32(&buffer[0], value);
         return TRUE;
      }
   }

   return FALSE;
}

OGPS_Boolean BinaryMSBPointVectorReaderContext::Read(OGPS_Double* const value)
{
   _ASSERT(value);

   if(HasStream())
   {
      OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_DOUBLE_SIZE];
      GetStream()->read(&buffer[0], _OPENGPS_BINFORMAT_DOUBLE_SIZE);

      if(IsGood())
      {
         _ASSERT(_OPENGPS_BINFORMAT_DOUBLE_SIZE == 8); // 8 bytes
         Environment::GetInstance()->ByteSwap64(&buffer[0], value);
         return TRUE;
      }
   }

   return FALSE;
}
