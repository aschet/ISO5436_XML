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
 * Communicate validity of point vectors through special IEEE754 values.
 */

#ifndef _OPENGPS_INLINE_VALIDITY_HXX
#define _OPENGPS_INLINE_VALIDITY_HXX

#ifndef _OPENGPS_CXX_OPENGPS_HXX
#  include <opengps/cxx/opengps.hxx>
#endif

#ifndef _OPENGPS_POINT_VALIDITY_PROVIDER_HXX
#  include "point_validity_provider.hxx"
#endif

namespace OpenGPS
{
   /*!
    * Implements OpenGPS::PointValidityProvider as a lookup
    * of a special IEEE754 value.
    *
    * If the value of the point data stored for the Z component
    * of a point vector equals the special IEEE754 value
    * of infinity, then the point measurement is identified as invalid.
    */
   class FloatInlineValidity : public PointValidityProvider
   {
   public:
      /*!
       * Creates a new instance.
       * @param value The point buffer of the Z axis.
       */
      FloatInlineValidity(PointBuffer* const value);

      /*! Destroys this instance. */
      ~FloatInlineValidity();

      virtual void SetValid(const unsigned int index, const OGPS_Boolean value) throw(...);
      virtual OGPS_Boolean IsValid(const unsigned int index) const throw(...);
   };

   /*!
    * Implements OpenGPS::PointValidityProvider as a lookup
    * of a special IEEE754 value.
    *
    * If the value of the point data stored for the Z component
    * of a point vector equals the special IEEE754 value
    * of infinity, then the point measurement is identified as invalid.
    */
   class DoubleInlineValidity : public PointValidityProvider
   {
   public:
      /*!
       * Creates a new instance.
       * @param value The point buffer of the Z axis.
       */
      DoubleInlineValidity(PointBuffer* const value);

      /*! Destroys this instance. */
      ~DoubleInlineValidity();

      virtual void SetValid(const unsigned int index, const OGPS_Boolean value) throw(...);
      virtual OGPS_Boolean IsValid(const unsigned int index) const throw(...);
   };
}

#endif /* _OPENGPS_INLINE_VALIDITY_HXX */
