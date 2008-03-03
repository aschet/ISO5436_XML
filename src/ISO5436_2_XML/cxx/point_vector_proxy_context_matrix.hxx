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
 * Indexing of point data managed by OpenGPS::VectorBuffer.
 */

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
   /*!
    * Indexing of point data managed by OpenGPS::VectorBuffer.
    * The indexes are calculated for point measurements stored in
    * a matrix structure that saves extra topology information.
    */
   class PointVectorProxyContextMatrix : public PointVectorProxyContext
   {
   public:
      /*!
       * Creates a new instance.
       * @param maxU The maximum index possible in X direction of the topology mapping.
       * @param maxV The maximum index possible in Y direction of the topology mapping.
       * @param maxW The maximum index possible in Z direction of the topology mapping.
       */
      PointVectorProxyContextMatrix(
         const unsigned long maxU,
         const unsigned long maxV,
         const unsigned long maxW);

      /*! Destroys this instance. */
      virtual ~PointVectorProxyContextMatrix();

      /*!
       * Sets the current index.
       * @param u The new arbitrary index in X direction of the topology mapping.
       * @param v The new arbitrary index in Y direction of the topology mapping.
       * @param w The new arbitrary index in Z direction of the topology mapping.
       * @returns Returns TRUE on success, FALSE otherwise.
       */
      OGPS_Boolean SetIndex(
         const unsigned long u,
         const unsigned long v,
         const unsigned long w);

      virtual unsigned long GetIndex() const;
      virtual OGPS_Boolean IncrementIndex();
      virtual OGPS_Boolean IsMatrix() const;

   private:
      /*! Returns FALSE if the maximum possible index is reached, TRUE otherwise. */
      OGPS_Boolean HasNext() const;

      /*! The current index in X direction of the topology mapping. */
      unsigned long m_U;

      /*! The current index in Y direction of the topology mapping. */
      unsigned long m_V;

      /*! The current index in Z direction of the topology mapping. */
      unsigned long m_W;

      /*! The maximum index possible in X direction. */
      unsigned long m_MaxU;
      
      /*! The maximum index possible in Y direction. */
      unsigned long m_MaxV;

      /*! The maximum index possible in Z direction. */
      unsigned long m_MaxW;
   };
}

#endif /* _OPENGPS_POINT_VECTOR_PROXY_CONTEXT_MATRIX_HXX */
