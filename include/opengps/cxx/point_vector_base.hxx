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

#ifndef _OPENGPS_CXX_POINT_VECTOR_BASE_HXX
#define _OPENGPS_CXX_POINT_VECTOR_BASE_HXX

#ifndef _OPENGPS_CXX_OPENGPS_HXX
#  include <opengps/cxx/opengps.hxx>
#endif

namespace OpenGPS
{
   class DataPoint;

   class _OPENGPS_EXPORT PointVectorBase
   {
   public:      
      virtual ~PointVectorBase();

      virtual const DataPoint* GetX() const = 0;
      virtual const DataPoint* GetY() const = 0;
      virtual const DataPoint* GetZ() const = 0;

      virtual DataPoint* GetX() = 0;
      virtual DataPoint* GetY() = 0;
      virtual DataPoint* GetZ() = 0;

      virtual OGPS_Boolean Set(const PointVectorBase& value) = 0;
      virtual OGPS_Boolean Get(PointVectorBase& value) const = 0;

   protected:
      PointVectorBase();
   };
}

#endif /* _OPENGPS_CXX_POINT_VECTOR_BASE_HXX */
