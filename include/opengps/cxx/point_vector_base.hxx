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

/*! @file
 * Very fundamental representation of three-dimensional point measurement data.
 * A point vector holds all three components of 3D point data stored in an ISO 5436-2 X3P file
 * and provides methods for typesafe access.
 */

#ifndef _OPENGPS_CXX_POINT_VECTOR_BASE_HXX
#define _OPENGPS_CXX_POINT_VECTOR_BASE_HXX

#ifndef _OPENGPS_CXX_OPENGPS_HXX
#  include <opengps/cxx/opengps.hxx>
#endif

namespace OpenGPS
{
   class DataPoint;

   /*!
    * Typesafe and very fundamental representation of three-dimensional point measurement data.
    *
    * @see OpenGPS::PointVector
    */
   class _OPENGPS_EXPORT PointVectorBase
   {
   public:
      /*! Destructs this object. */
      virtual ~PointVectorBase();

      /*!
       * Gets typesafe direct read-only access to the x component.
       */
      virtual const DataPoint* GetX() const = 0;

      /*!
       * Gets typesafe direct read-only access to the y component.
       */
      virtual const DataPoint* GetY() const = 0;

      /*!
       * Gets typesafe direct read-only access to the z component.
       */
      virtual const DataPoint* GetZ() const = 0;

      /*!
       * Gets typesafe direct access to the x component.
       */
      virtual DataPoint* GetX() = 0;

      /*!
       * Gets typesafe direct access to the y component.
       */
      virtual DataPoint* GetY() = 0;

      /*!
       * Gets typesafe direct access to the z component.
       */
      virtual DataPoint* GetZ() = 0;

      /*!
       * Copies values from another OpenGPS::PointVector instance.
       *
       * @param value OpenGPS::PointVectorBase object to copy from.
       * @returns Returns TRUE on success, FALSE otherwise.
       */
      virtual OGPS_Boolean Set(const PointVectorBase& value) = 0;

      /*!
       * Copies values from another OpenGPS::PointVectorBase instance.
       *
       * @param value Gets values from the current instance as a copy.
       * @returns Returns TRUE on success, FALSE otherwise.
       */
      virtual OGPS_Boolean Get(PointVectorBase& value) const = 0;

   protected:
      /*! Creates a new instance. */
      PointVectorBase();
   };
}

#endif /* _OPENGPS_CXX_POINT_VECTOR_BASE_HXX */
