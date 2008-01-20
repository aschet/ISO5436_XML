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

#ifndef _OPENGPS_XML_POINT_VECTOR_READER_CONTEXT_HXX
#define _OPENGPS_XML_POINT_VECTOR_READER_CONTEXT_HXX

#ifndef _OPENGPS_POINT_VECTOR_READER_CONTEXT_HXX
#  include "point_vector_reader_context.hxx"
#endif

#include <opengps/cxx/iso5436_2_xsd.hxx>

namespace OpenGPS
{
   class PointVectorInputStringStream;

   class XmlPointVectorReaderContext : public PointVectorReaderContext
   {
   public:
      typedef Schemas::ISO5436_2::DataListType::Datum_sequence StringList;

      XmlPointVectorReaderContext(const StringList* const pointVectorList);
      virtual ~XmlPointVectorReaderContext();

      virtual OGPS_Boolean Read(OGPS_Int16* const value);
      virtual OGPS_Boolean Read(OGPS_Int32* const value);
      virtual OGPS_Boolean Read(OGPS_Float* const value);
      virtual OGPS_Boolean Read(OGPS_Double* const value);

      virtual OGPS_Boolean Skip();

      virtual OGPS_Boolean MoveNext();
      virtual OGPS_Boolean IsValid() const;

   protected:
      virtual OGPS_Boolean IsGood() const;

   private:
      void Set(const OpenGPS::String& buf);
      void Reset();

      const StringList* m_PointVectorList;
      unsigned long m_Next;
      PointVectorInputStringStream* m_Stream;            
   };
}

#endif /* _OPENGPS_XML_POINT_VECTOR_READER_CONTEXT_HXX */
