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

#include <limits>

#include "inline_validity.hxx"
#include "point_buffer.hxx"
#include "stdafx.hxx"

FloatInlineValidity::FloatInlineValidity(PointBuffer* const value)
: PointValidityProvider(value)
{
   _ASSERT(value && value->GetType() == OGPS_FloatPointType);
}

FloatInlineValidity::~FloatInlineValidity()
{
}

OGPS_Boolean FloatInlineValidity::SetValid(const unsigned int index, const OGPS_Boolean value)
{
   _ASSERT(std::numeric_limits<OGPS_Float>::has_infinity);

   if(!value)
   {
      return GetPointBuffer()->Set(index, std::numeric_limits<OGPS_Float>::infinity());
   }

   return value;
}

OGPS_Boolean FloatInlineValidity::IsValid(const unsigned int index) const
{
   _ASSERT(std::numeric_limits<OGPS_Float>::has_infinity);

   OGPS_Float value;
   if(GetPointBuffer()->Get(index, value))
   {
      return value != std::numeric_limits<OGPS_Float>::infinity();
   }

   return FALSE;
}

DoubleInlineValidity::DoubleInlineValidity(PointBuffer* const value)
: PointValidityProvider(value)
{
   _ASSERT(value && value->GetType() == OGPS_DoublePointType);
}

DoubleInlineValidity::~DoubleInlineValidity()
{
}

OGPS_Boolean DoubleInlineValidity::SetValid(const unsigned int index, const OGPS_Boolean value)
{
   _ASSERT(std::numeric_limits<OGPS_Double>::has_infinity);

   if(!value)
   {
      return GetPointBuffer()->Set(index, std::numeric_limits<OGPS_Double>::infinity());
   }

   return value;
}

OGPS_Boolean DoubleInlineValidity::IsValid(const unsigned int index) const
{
   _ASSERT(std::numeric_limits<OGPS_Double>::has_infinity);

   OGPS_Double value;
   if(GetPointBuffer()->Get(index, value))
   {
      return value != std::numeric_limits<OGPS_Double>::infinity();
   }

   return FALSE;
}
