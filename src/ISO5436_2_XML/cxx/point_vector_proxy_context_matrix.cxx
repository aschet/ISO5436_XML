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

#include "point_vector_proxy_context_matrix.hxx"

#include "stdafx.hxx"

PointVectorProxyContextMatrix::PointVectorProxyContextMatrix(
         const unsigned long maxU,
         const unsigned long maxV,
         const unsigned long maxW)
         : PointVectorProxyContext()
{
   m_MaxU = maxU;
   m_MaxV = maxV;
   m_MaxW = maxW;

   m_U = 0;
   m_V = 0;
   m_W = 0;
}

PointVectorProxyContextMatrix::~PointVectorProxyContextMatrix()
{
}

OGPS_Boolean PointVectorProxyContextMatrix::SetIndex(
         const unsigned long u,
         const unsigned long v,
         const unsigned long w)
{
   if(u < m_MaxU && v < m_MaxV && w < m_MaxW)
   {
      m_U = u;
      m_V = v;
      m_W = w;

      return TRUE;
   }

   return FALSE;
}

unsigned long PointVectorProxyContextMatrix::GetIndex() const
{
   return ((m_U * m_MaxV * (m_W + 1)) + m_V);
}

OGPS_Boolean PointVectorProxyContextMatrix::IncrementIndex()
{
   if(HasNext())
   {
      /*if(m_IsReset)
      {
         _ASSERT(m_U == 0 && m_V == 0 && m_W == 0);

         m_IsReset = FALSE;

         return TRUE;
      }*/

      if(m_U + 1 < m_MaxU)
      {
         ++m_U;

         return TRUE;
      }

      if(m_V + 1 < m_MaxV)
      {
         ++m_V;
         m_U = 0;

         return TRUE;
      }

      if(m_W + 1 < m_MaxW)
      {
         ++m_W;
         m_U = 0;
         m_V = 0;

         return TRUE;
      }
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxyContextMatrix::HasNext() const
{

   //if(m_IsForward)
   {
      /*if(m_IsReset)
      {
         _ASSERT(m_U == 0 && m_V == 0 && m_W == 0);

         return m_Handle->GetMaxU() > 0;
      }*/

      return ((m_W + 1) * (m_V + 1) * (m_U + 1) < m_MaxU * m_MaxV * m_MaxW);
   }

   //return FALSE;
}
