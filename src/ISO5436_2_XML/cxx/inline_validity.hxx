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
   class FloatInlineValidity : public PointValidityProvider
   {
   public:
      FloatInlineValidity(PointBuffer* const value);
      ~FloatInlineValidity();

      virtual OGPS_Boolean SetValid(const unsigned int index, const OGPS_Boolean value);
      virtual OGPS_Boolean IsValid(const unsigned int index) const;
   };

   class DoubleInlineValidity : public PointValidityProvider
   {
   public:
      DoubleInlineValidity(PointBuffer* const value);
      ~DoubleInlineValidity();

      virtual OGPS_Boolean SetValid(const unsigned int index, const OGPS_Boolean value);
      virtual OGPS_Boolean IsValid(const unsigned int index) const;
   };
}

#endif /* _OPENGPS_INLINE_VALIDITY_HXX */
