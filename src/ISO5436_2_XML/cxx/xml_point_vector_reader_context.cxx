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

#include "xml_point_vector_reader_context.hxx"
#include "point_vector_iostream.hxx"

#include "stdafx.hxx"

#include <opengps/cxx/string.hxx>

PointVectorReaderContext::PointVectorReaderContext()
{
}

PointVectorReaderContext::~PointVectorReaderContext()
{
}

XmlPointVectorReaderContext::XmlPointVectorReaderContext(const StringList* const pointVectorList)
: PointVectorReaderContext(), m_PointVectorList(pointVectorList)
{
   _ASSERT(pointVectorList);

   m_Next = 0;
   m_Stream = NULL;   
}

XmlPointVectorReaderContext::~XmlPointVectorReaderContext()
{
   Reset();
}

void XmlPointVectorReaderContext::Set(const OpenGPS::String& buf)
{
   _ASSERT(m_Stream);

   m_Stream->clear(); // reset current state
   m_Stream->str(buf); // set new buffer
}

void XmlPointVectorReaderContext::Reset()
{
   if(m_Stream)
   {
      delete m_Stream;
      m_Stream = NULL;
   }

   m_Next = 0;
}

OGPS_Boolean XmlPointVectorReaderContext::Read(OGPS_Int16* const value)
{
   _ASSERT(value);

   if(m_Stream)
   {
      *m_Stream >> *value;
      return IsGood();
   }

   return FALSE;
}

OGPS_Boolean XmlPointVectorReaderContext::Read(OGPS_Int32* const value)
{
   _ASSERT(value);

   if(m_Stream)
   {
      *m_Stream >> *value;
      return IsGood();
   }

   return FALSE;
}

OGPS_Boolean XmlPointVectorReaderContext::Read(OGPS_Float* const value)
{
   _ASSERT(value);

   if(m_Stream)
   {
      *m_Stream >> *value;
      return IsGood();
   }

   return FALSE;
}

OGPS_Boolean XmlPointVectorReaderContext::Read(OGPS_Double* const value)
{
   _ASSERT(value);

   if(m_Stream)
   {
      *m_Stream >> *value;      
      return IsGood();
   }

   return FALSE;
}

 OGPS_Boolean XmlPointVectorReaderContext::Skip()
 {
    if(m_Stream)
    {
      return IsGood();
    }

    return FALSE;
 }

OGPS_Boolean XmlPointVectorReaderContext::IsGood() const
{
   _ASSERT(m_Stream);

      const std::ios_base::io_state state = m_Stream->rdstate();
      return (state == std::ios_base::goodbit || state == std::ios_base::eofbit);
}

OGPS_Boolean XmlPointVectorReaderContext::MoveNext()
{
   _ASSERT(m_PointVectorList);

   if(m_Next > 0)
   {
      if(m_Next < m_PointVectorList->size())
      {
         Set(m_PointVectorList->at(m_Next++));
         return TRUE;
      }

      Reset();
      return FALSE;
   }

   // non-empty vector list
   if(m_PointVectorList->size() > 0)
   {
   _ASSERT(!m_Stream);

   m_Stream = new PointVectorInputStringStream(m_PointVectorList->at(m_Next++));

   return TRUE;
   }

   return FALSE;
}

OGPS_Boolean XmlPointVectorReaderContext::IsValid() const
{
   return (m_Stream && m_Stream->str().length() > 0);
}
