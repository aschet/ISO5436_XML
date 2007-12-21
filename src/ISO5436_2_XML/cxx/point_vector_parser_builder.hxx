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

#ifndef _OPENGPS_POINT_VECTOR_PARSER_BUILDER_HXX
#define _OPENGPS_POINT_VECTOR_PARSER_BUILDER_HXX

#ifndef _OPENGPS_HXX
#  include <opengps/opengps.hxx>
#endif

#ifndef _OPENGPS_DATA_POINT_TYPE_H
#  include <opengps/data_point_type.h>
#endif

namespace OpenGPS
{
   class PointVectorParser;

/* Interface which handles the building process of a specialized parser
   * object for reading and writing typed point data of three axis. */
  class PointVectorParserBuilder {
  public:
     PointVectorParserBuilder();
    virtual ~PointVectorParserBuilder();
    
    /* Build the main parser object itself. */
    virtual OGPS_Boolean BuildParser();
    
    /* Build objects to parse one single data point. Take into account the
     * axis data type. Connect point parsers with the main parser object. */
    virtual OGPS_Boolean BuildX(const OGPS_DataPointType dataType);
    virtual OGPS_Boolean BuildY(const OGPS_DataPointType dataType);
    virtual OGPS_Boolean BuildZ(const OGPS_DataPointType dataType);
    
    /* Gets the assembled point vector parser (for three axis). */
    virtual PointVectorParser* GetParser();

  private:
      PointVectorParser* m_Parser;
  };
}

#endif /* _OPENGPS_POINT_VECTOR_PARSER_BUILDER_HXX */
