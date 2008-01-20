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

#ifndef _OPENGPS_POINT_VECTOR_PROXY_CONTEXT_MATRIX_HXX
#define _OPENGPS_POINT_VECTOR_PROXY_CONTEXT_MATRIX_HXX

#ifndef _OPENGPS_CXX_OPENGPS_HXX
#  include <opengps/cxx/opengps.hxx>
#endif

#ifndef _OPENGPS_POINT_VECTOR_PROXY_CONTEXT_HXX
#  include "point_vector_proxy_context.hxx"
#endif

namespace OpenGPS
{
   class PointVectorProxyContextMatrix : public PointVectorProxyContext
   {
   public:      
      PointVectorProxyContextMatrix(
         const unsigned long maxU,
         const unsigned long maxV,
         const unsigned long maxW);
      virtual ~PointVectorProxyContextMatrix();

      OGPS_Boolean SetIndex(
         const unsigned long u,
         const unsigned long v,
         const unsigned long w);

      virtual unsigned long GetIndex() const;

      virtual OGPS_Boolean IncrementIndex();

   private:
      OGPS_Boolean HasNext() const;

      unsigned long m_U;
      unsigned long m_V;
      unsigned long m_W;

      unsigned long m_MaxU;
      unsigned long m_MaxV;
      unsigned long m_MaxW;
   };
}

#endif /* _OPENGPS_POINT_VECTOR_PROXY_CONTEXT_MATRIX_HXX */
