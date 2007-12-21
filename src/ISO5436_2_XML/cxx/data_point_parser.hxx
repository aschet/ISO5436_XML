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

#ifndef _OPENGPS_DATA_POINT_PARSER_HXX
#define _OPENGPS_DATA_POINT_PARSER_HXX

#ifndef _OPENGPS_HXX
#  include <opengps/opengps.hxx>
#endif

namespace OpenGPS
{
   class PointVectorReaderContext;
   class PointVectorWriterContext;
   class DataPoint;

   // TODO: create a template, instantiate int16*,int32*,float*,double* and remove current implementations?
   class DataPointParser {
  public:    
    virtual ~DataPointParser();
    
    virtual OGPS_Boolean Read(PointVectorReaderContext& context, DataPoint& value) = 0;
    virtual OGPS_Boolean Write(PointVectorWriterContext& context, const DataPoint& value) = 0;

   protected:
      DataPointParser();
  };
}

#endif /* _OPENGPS_DATA_POINT_PARSER_HXX */
