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

#ifndef _OPENGPS_CXX_DATA_POINT_HXX
#define _OPENGPS_CXX_DATA_POINT_HXX

#ifndef _OPENGPS_CXX_OPENGPS_HXX
#  include <opengps/cxx/opengps.hxx>
#endif

#ifndef _OPENGPS_DATA_POINT_TYPE_H
#  include <opengps/data_point_type.h>
#endif

namespace OpenGPS
{
   class DataPoint
   {
   protected:
      DataPoint() = 0;

   public:
      virtual ~DataPoint() = 0;

      virtual OGPS_DataPointType GetType() const = 0;

      virtual OGPS_Boolean Get(OGPS_Int16* const value) const = 0;
      virtual OGPS_Boolean Get(OGPS_Int32* const value) const = 0;
      virtual OGPS_Boolean Get(OGPS_Float* const value) const = 0;
      virtual OGPS_Boolean Get(OGPS_Double* const value) const = 0;

      virtual OGPS_Double Get() const = 0;

      virtual OGPS_Boolean IsValid() const = 0;

      virtual OGPS_Boolean Set(const OGPS_Int16 value) = 0;
      virtual OGPS_Boolean Set(const OGPS_Int32 value) = 0;
      virtual OGPS_Boolean Set(const OGPS_Float value) = 0;
      virtual OGPS_Boolean Set(const OGPS_Double value) = 0;

      virtual OGPS_Boolean SetNull() = 0;

      virtual OGPS_Boolean Set(const DataPoint& src) = 0;

   protected:
      virtual void Reset() = 0;
   };
}

#endif /* _OPENGPS_CXX_DATA_POINT_HXX */
