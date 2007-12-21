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
#include "binary_lsb_point_vector_reader_context.hxx"

#include "point_vector_iostream.hxx"
#include "environment.hxx"

#include "stdafx.hxx"

BinaryMSBPointVectorReaderContext::BinaryMSBPointVectorReaderContext(const String& filePath)
   : BinaryPointVectorReaderContext()
     {
        m_Stream = new PointVectorInputBinaryFileStream(filePath);
     }

     BinaryMSBPointVectorReaderContext::~BinaryMSBPointVectorReaderContext()
     {
        Close();

        Environment::Reset();
     }

OGPS_Boolean BinaryMSBPointVectorReaderContext::Read(short* value)
{
   _ASSERT(value);

   if(m_Stream)
   {
      OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_INT16_SIZE];
      m_Stream->read(&buffer[0], _OPENGPS_BINFORMAT_INT16_SIZE);
      
      if(IsGood())
      {
         _ASSERT(_OPENGPS_BINFORMAT_INT16_SIZE == 2); // 2 bytes
         Environment::GetInstance()->ByteSwap16(&buffer[0], value);
         return TRUE;
      }
   }

   return FALSE;
}

OGPS_Boolean BinaryMSBPointVectorReaderContext::Read(int* value)
{
   _ASSERT(value);

   if(m_Stream)
   {
      OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_INT32_SIZE];
      m_Stream->read(&buffer[0], _OPENGPS_BINFORMAT_INT32_SIZE);
      
      if(IsGood())
      {
         _ASSERT(_OPENGPS_BINFORMAT_INT32_SIZE == 4); // 4 bytes
         Environment::GetInstance()->ByteSwap32(&buffer[0], value);
         return TRUE;
      }
   }

   return FALSE;
}

OGPS_Boolean BinaryMSBPointVectorReaderContext::Read(float* value)
{
   _ASSERT(value);

   if(m_Stream)
   {
      OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_FLOAT_SIZE];
      m_Stream->read(&buffer[0], _OPENGPS_BINFORMAT_FLOAT_SIZE);
      
      if(IsGood())
      {
         _ASSERT(_OPENGPS_BINFORMAT_FLOAT_SIZE == 4); // 4 bytes
         Environment::GetInstance()->ByteSwap32(&buffer[0], value);
         return TRUE;
      }
   }

   return FALSE;
}

OGPS_Boolean BinaryMSBPointVectorReaderContext::Read(double* value)
{
   _ASSERT(value);

   if(m_Stream)
   {
      OpenGPS::UnsignedByte buffer[_OPENGPS_BINFORMAT_DOUBLE_SIZE];
      m_Stream->read(&buffer[0], _OPENGPS_BINFORMAT_DOUBLE_SIZE);
      
      if(IsGood())
      {
         _ASSERT(_OPENGPS_BINFORMAT_DOUBLE_SIZE == 8); // 8 bytes
         Environment::GetInstance()->ByteSwap64(&buffer[0], value);
         return TRUE;
      }
   }

   return FALSE;
}

 OGPS_Boolean BinaryMSBPointVectorReaderContext::Skip()
 {
    if(m_Stream)
    {
      return IsGood();
    }

    return FALSE;
 }

 OGPS_Boolean BinaryMSBPointVectorReaderContext::IsGood() const
{
   _ASSERT(m_Stream);

   const std::ios_base::io_state state = m_Stream->rdstate();
   return (state == std::ios_base::goodbit || state == std::ios_base::eofbit);
}

 OGPS_Boolean BinaryMSBPointVectorReaderContext::Close()
 {
    if(m_Stream)
    {
       m_Stream->close();
       delete m_Stream;
       m_Stream = NULL;

       return TRUE;
    }

    return FALSE;
 }

 OGPS_Boolean BinaryMSBPointVectorReaderContext::MoveNext()
{
   if(m_Stream)
   {
      return !m_Stream->eof();
   }

   return FALSE;
}

 OGPS_Boolean BinaryMSBPointVectorReaderContext::IsValid() const
{
   return TRUE; // TODO: parse invalid file here!
}
