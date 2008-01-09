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

#ifndef _OPENGPS_XML_POINT_VECTOR_WRITER_CONTEXT_HXX
#define _OPENGPS_XML_POINT_VECTOR_WRITER_CONTEXT_HXX

#ifndef _OPENGPS_POINT_VECTOR_WRITER_CONTEXT_HXX
#  include "point_vector_writer_context.hxx"
#endif

#include <opengps/cxx/iso5436_2_xsd.hxx>

namespace OpenGPS
{
   class PointVectorOutputStringStream;

   class XmlPointVectorWriterContext : public PointVectorWriterContext {
   public:
      typedef xsd::DataListType::Datum_sequence StringList;

      XmlPointVectorWriterContext(StringList* const pointVectorList);
      virtual ~XmlPointVectorWriterContext();

      virtual OGPS_Boolean Write(const OGPS_Int16* const value);
      virtual OGPS_Boolean Write(const OGPS_Int32* const value);
      virtual OGPS_Boolean Write(const OGPS_Float* const value);
      virtual OGPS_Boolean Write(const OGPS_Double* const value);

      virtual OGPS_Boolean Skip();

      virtual OGPS_Boolean MoveNext();

   protected:
      virtual OGPS_Boolean IsGood() const;
      virtual void AppendSeparator();

   private:
      OpenGPS::String Get() const;
      void Reset();

      PointVectorOutputStringStream* m_Stream;
      OGPS_Boolean m_NeedsSeparator;
      StringList* m_PointVectorList;
   };
}

#endif /* _OPENGPS_XML_POINT_VECTOR_WRITER_CONTEXT_HXX */
