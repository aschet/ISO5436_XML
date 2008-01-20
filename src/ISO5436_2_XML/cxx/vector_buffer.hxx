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

#ifndef _OPENGPS_VECTOR_BUFFER_HXX
#define _OPENGPS_VECTOR_BUFFER_HXX

#ifndef _OPENGPS_DATA_POINT_TYPE_H
#  include <opengps/data_point_type.h>
#endif

#ifndef _OPENGPS_CXX_POINT_VECTOR_BASE_HXX
#  include <opengps/cxx/point_vector_base.hxx>
#endif

#ifndef _OPENGPS_AUTO_PTR_TYPES_HXX
#  include "auto_ptr_types.hxx"
#endif

#ifndef _OPENGPS_VALID_BUFFER_HXX
#  include "valid_buffer.hxx"
#endif

namespace OpenGPS
{
   class PointBuffer;
   class PointVectorProxyContext;

   class VectorBuffer
   {
   public:
      VectorBuffer();
      ~VectorBuffer();

      virtual void SetX(PointBuffer* const value);
      virtual void SetY(PointBuffer* const value);
      virtual void SetZ(PointBuffer* const value);

      virtual void SetValidityProvider(PointValidityProvider* const value, ValidBuffer* const buffer);

      virtual PointBuffer* GetX();
      virtual PointBuffer* GetY();
      virtual PointBuffer* GetZ();

      virtual PointValidityProvider* GetValidityProvider();
      virtual ValidBuffer* GetValidityBuffer();

      virtual const PointBuffer* GetX() const;
      virtual const PointBuffer* GetY() const;
      virtual const PointBuffer* GetZ() const;

      virtual const PointValidityProvider* GetValidityProvider() const;
      virtual const ValidBuffer* GetValidityBuffer() const;

      OGPS_Boolean HasValidityBuffer() const;

      virtual PointVectorAutoPtr GetPointVectorProxy(const PointVectorProxyContext& context);

   private:
      PointBuffer* m_X;
      PointBuffer* m_Y;
      PointBuffer* m_Z;

      PointValidityProvider* m_ValidityProvider;
      ValidBuffer* m_ValidBuffer;
   };
}

#endif /* _OPENGPS_VECTOR_BUFFER_HXX */
