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

#include "xml_point_vector_writer_context.hxx"
#include "point_vector_iostream.hxx"

#include "stdafx.hxx"

PointVectorWriterContext::PointVectorWriterContext()
{
}

PointVectorWriterContext::~PointVectorWriterContext()
{
}

XmlPointVectorWriterContext::XmlPointVectorWriterContext(StringList* const pointVectorList)
: PointVectorWriterContext()
{
   _ASSERT(pointVectorList);

   m_Stream = new PointVectorOutputStringStream();
   m_Stream->setf(std::ios::scientific);
   m_Stream->precision(15);
   m_NeedsSeparator = FALSE;
   m_PointVectorList = pointVectorList;
}

XmlPointVectorWriterContext::~XmlPointVectorWriterContext()
{
   if(m_Stream)
   {
   delete m_Stream;
   }
}

String XmlPointVectorWriterContext::Get() const
{
   if(m_Stream)
   {
   return m_Stream->str();
   }

   return _T("");
}

void XmlPointVectorWriterContext::Reset()
{
   if(m_Stream)
   {
   m_Stream->str(_T(""));
   }

   m_NeedsSeparator = FALSE;
}

OGPS_Boolean XmlPointVectorWriterContext::Write(const OGPS_Int16* const value)
{
   _ASSERT(value);

   if(m_Stream)
   {
      AppendSeparator();

      *m_Stream << *value;
      return IsGood();
   }

   return FALSE;
}

OGPS_Boolean XmlPointVectorWriterContext::Write(const OGPS_Int32* const value)
{
   _ASSERT(value);

   if(m_Stream)
   {
      AppendSeparator();

      *m_Stream << *value;
      return IsGood();
   }

   return FALSE;
}

OGPS_Boolean XmlPointVectorWriterContext::Write(const OGPS_Float* const value)
{
   _ASSERT(value);

   if(m_Stream)
   {
      AppendSeparator();

      *m_Stream << *value;
      return IsGood();
   }

   return FALSE;
}

OGPS_Boolean XmlPointVectorWriterContext::Write(const OGPS_Double* const value)
{
   _ASSERT(value);

   if(m_Stream)
   {
      AppendSeparator();

      *m_Stream << *value;
      return IsGood();
   }

   return FALSE;
}

OGPS_Boolean XmlPointVectorWriterContext::Skip()
{
   if(m_Stream)
   {
      return IsGood();
   }

   return FALSE;
}

OGPS_Boolean XmlPointVectorWriterContext::IsGood() const
{
   _ASSERT(m_Stream);

      const std::ios_base::io_state state = m_Stream->rdstate();
      return (state == std::ios_base::goodbit || state == std::ios_base::eofbit);
}

void XmlPointVectorWriterContext::AppendSeparator()
{
   _ASSERT(m_Stream);

   if(m_NeedsSeparator)
   {
      m_Stream->write(_T(";"), 1);
   }

   m_NeedsSeparator = TRUE;
}

OGPS_Boolean XmlPointVectorWriterContext::MoveNext()
{
   if(m_Stream && m_PointVectorList)
   {
      const OpenGPS::String vector = Get();
      Schemas::ISO5436_2::DataListType::Datum_type datum(vector);
      m_PointVectorList->push_back(datum);

      Reset();
      
      return TRUE;
   }

   return FALSE;
}
