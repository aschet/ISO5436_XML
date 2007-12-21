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

#include "point_vector_iostream.hxx"

#include "stdafx.hxx"

PointVectorWhitespaceFacet::PointVectorWhitespaceFacet(size_t refs)
: BaseType(refs)
{
}

PointVectorWhitespaceFacet::~PointVectorWhitespaceFacet()
{
}

bool PointVectorWhitespaceFacet::do_is(mask msk, OGPS_Character ch) const
{
   if(ch == _T(';') && msk == space)
   {
      return true;
   }

   return BaseType::do_is(msk, ch);
}

PointVectorInvariantLocale PointVectorInvariantLocale::m_Instance;

PointVectorInvariantLocale::PointVectorInvariantLocale()
: BaseType(std::locale::classic(), new PointVectorWhitespaceFacet())
{
}

PointVectorInvariantLocale::~PointVectorInvariantLocale()
{
}

const PointVectorInvariantLocale& PointVectorInvariantLocale::GetInstance()
{
   return m_Instance;
}

PointVectorInputStringStream::PointVectorInputStringStream()
: BaseType(std::ios_base::in)
{
   imbue(PointVectorInvariantLocale::GetInstance());
}

PointVectorInputStringStream::PointVectorInputStringStream(const OpenGPS::String& s)
: BaseType(std::ios_base::in)
{
   imbue(PointVectorInvariantLocale::GetInstance());
   str(s);
}

PointVectorInputStringStream::~PointVectorInputStringStream()
{
}

PointVectorOutputStringStream::PointVectorOutputStringStream()
: BaseType(std::ios_base::out)
{
   imbue(PointVectorInvariantLocale::GetInstance());
}

PointVectorOutputStringStream::~PointVectorOutputStringStream()
{
}

PointVectorInputBinaryFileStream::PointVectorInputBinaryFileStream(const String& filePath)
: BaseType()
{
   imbue(PointVectorInvariantLocale::GetInstance());

   String buf(filePath);
   open(buf.ToChar(), std::ios_base::in | std::ios_base::binary);
}

PointVectorInputBinaryFileStream::~PointVectorInputBinaryFileStream()
{
}

PointVectorOutputBinaryFileStream::PointVectorOutputBinaryFileStream(const String& filePath)
: BaseType()
{
   imbue(PointVectorInvariantLocale::GetInstance());

   String buf(filePath);
   open(buf.ToChar(), std::ios_base::out | std::ios_base::binary);
}

PointVectorOutputBinaryFileStream::~PointVectorOutputBinaryFileStream()
{
}
