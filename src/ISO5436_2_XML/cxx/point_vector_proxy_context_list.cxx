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

#include "point_vector_proxy_context_list.hxx"

#include "stdafx.hxx"

PointVectorProxyContextList::PointVectorProxyContextList(const unsigned long maxIndex)
: PointVectorProxyContext()
{
   m_MaxIndex = maxIndex;
   m_Index = 0;
}

PointVectorProxyContextList::~PointVectorProxyContextList()
{
}

OGPS_Boolean PointVectorProxyContextList::SetIndex(const unsigned long index)
{
   if(index < m_MaxIndex)
   {
      m_Index = index;

      return TRUE;
   }

   return FALSE;
}

unsigned long PointVectorProxyContextList::GetIndex() const
{
   return m_Index;
}

OGPS_Boolean PointVectorProxyContextList::IncrementIndex()
{
   if(m_Index < m_MaxIndex)
   {
      ++m_Index;

      return TRUE;
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxyContextList::IsMatrix() const
{
   return FALSE;
}
