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

#include <opengps/cxx/exceptions.hxx>

#include "stdafx.hxx"

/*! Checks whether the underlying stream is valid. Throws an exception if this is not the case. */
#define _CHECK_STREAM_AND_THROW_EXCEPTION \
   if(!HasStream()) \
   { \
   throw OpenGPS::Exception( \
   OGPS_ExInvalidOperation, \
   _EX_T("No binary file stream available."), \
   _EX_T("The operation on the binary file stream failed, because the stream has been closed already."), \
   _EX_T("OpenGPS::BinaryMSBPointVectorReaderContext")); \
   }

/*! Checks whether the underlying stream is valid. Throws an exception if this is not the case. */
#define _CHECK_ISGOOD_AND_THROW_EXCEPTION \
   if(!IsGood()) \
   { \
   throw OpenGPS::Exception( \
   OGPS_ExInvalidOperation, \
   _EX_T("The underlying binary stream object became invalid."), \
   _EX_T("A read/write error occured."), \
   _EX_T("OpenGPS::BinaryMSBPointVectorReaderContext")); \
   }

BinaryMSBPointVectorReaderContext::BinaryMSBPointVectorReaderContext(const OpenGPS::String& filePath)
: BinaryPointVectorReaderContext(filePath)
{
}

BinaryMSBPointVectorReaderContext::~BinaryMSBPointVectorReaderContext()
{
}

void BinaryMSBPointVectorReaderContext::Read(OGPS_Int16* const value) throw(...)
{
   _ASSERT(value);

   _CHECK_STREAM_AND_THROW_EXCEPTION;

   OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_INT16_SIZE];
   GetStream()->read(&buffer[0], _OPENGPS_BINFORMAT_INT16_SIZE);

   _CHECK_ISGOOD_AND_THROW_EXCEPTION;

   _ASSERT(_OPENGPS_BINFORMAT_INT16_SIZE == 2); // 2 bytes
   Environment::GetInstance()->ByteSwap16(&buffer[0], value);
}

void BinaryMSBPointVectorReaderContext::Read(OGPS_Int32* const value) throw(...)
{
   _ASSERT(value);

   _CHECK_STREAM_AND_THROW_EXCEPTION;

   OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_INT32_SIZE];
   GetStream()->read(&buffer[0], _OPENGPS_BINFORMAT_INT32_SIZE);

   _CHECK_ISGOOD_AND_THROW_EXCEPTION;

   _ASSERT(_OPENGPS_BINFORMAT_INT32_SIZE == 4); // 4 bytes
   Environment::GetInstance()->ByteSwap32(&buffer[0], value);
}

void BinaryMSBPointVectorReaderContext::Read(OGPS_Float* const value) throw(...)
{
   _ASSERT(value);

   _CHECK_STREAM_AND_THROW_EXCEPTION;

   OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_FLOAT_SIZE];
   GetStream()->read(&buffer[0], _OPENGPS_BINFORMAT_FLOAT_SIZE);

   _CHECK_ISGOOD_AND_THROW_EXCEPTION;

   _ASSERT(_OPENGPS_BINFORMAT_FLOAT_SIZE == 4); // 4 bytes
   Environment::GetInstance()->ByteSwap32(&buffer[0], value);
}

void BinaryMSBPointVectorReaderContext::Read(OGPS_Double* const value) throw(...)
{
   _ASSERT(value);

   _CHECK_STREAM_AND_THROW_EXCEPTION;

   OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_DOUBLE_SIZE];
   GetStream()->read(&buffer[0], _OPENGPS_BINFORMAT_DOUBLE_SIZE);

   _CHECK_ISGOOD_AND_THROW_EXCEPTION;

   _ASSERT(_OPENGPS_BINFORMAT_DOUBLE_SIZE == 8); // 8 bytes
   Environment::GetInstance()->ByteSwap64(&buffer[0], value);
}
