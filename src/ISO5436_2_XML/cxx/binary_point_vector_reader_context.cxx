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

#include "binary_point_vector_reader_context.hxx"
#include "point_vector_iostream.hxx"

#include "stdafx.hxx"

BinaryPointVectorReaderContext::BinaryPointVectorReaderContext(const OpenGPS::String& filePath)
: PointVectorReaderContext()
{
   m_Stream = new InputBinaryFileStream(filePath);
}

BinaryPointVectorReaderContext::~BinaryPointVectorReaderContext()
{
   Close();
}

OGPS_Boolean BinaryPointVectorReaderContext::Close()
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

OGPS_Boolean BinaryPointVectorReaderContext::MoveNext()
{
   if(m_Stream)
   {
      m_Stream->peek();
      return !m_Stream->eof();
   }

   return FALSE;
}

OGPS_Boolean BinaryPointVectorReaderContext::IsValid() const
{
   return TRUE; // TODO: parse invalid file here!
}

OGPS_Boolean BinaryPointVectorReaderContext::Skip()
{
   if(m_Stream)
   {
      return IsGood();
   }

   return FALSE;
}

OGPS_Boolean BinaryPointVectorReaderContext::IsGood() const
{
   _ASSERT(m_Stream);

   const std::ios_base::io_state state = m_Stream->rdstate();
   return (state == std::ios_base::goodbit || state == std::ios_base::eofbit);
}

OGPS_Boolean BinaryPointVectorReaderContext::HasStream() const
{
   return m_Stream != NULL;
}

InputBinaryFileStream* BinaryPointVectorReaderContext::GetStream()
{
   return m_Stream;
}
