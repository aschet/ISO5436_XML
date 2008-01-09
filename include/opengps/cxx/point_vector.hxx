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

#ifndef _OPENGPS_CXX_POINT_VECTOR_HXX
#define _OPENGPS_CXX_POINT_VECTOR_HXX

#ifndef _OPENGPS_CXX_OPENGPS_HXX
#  include <opengps/cxx/opengps.hxx>
#endif

#ifndef _OPENGPS_CXX_POINT_VECTOR_BASE_HXX
#  include <opengps/cxx/point_vector_base.hxx>
#endif

namespace OpenGPS
{
   class DataPoint;

   class PointVector : public PointVectorBase
   {
   public:
      PointVector();
      ~PointVector();

      virtual const DataPoint* GetX() const;
      virtual const DataPoint* GetY() const;
      virtual const DataPoint* GetZ() const;

      virtual DataPoint* GetX();
      virtual DataPoint* GetY();
      virtual DataPoint* GetZ();

      virtual OGPS_Boolean Set(const PointVectorBase& value);
      virtual OGPS_Boolean Get(PointVectorBase& value) const;

      // TODO: really need return value here?
      OGPS_Boolean GetX(OGPS_Int16* const value) const;
      OGPS_Boolean GetX(OGPS_Int32* const value) const;
      OGPS_Boolean GetX(OGPS_Float* const value) const;
      OGPS_Boolean GetX(OGPS_Double* const value) const;

      OGPS_Boolean GetY(OGPS_Int16* const value) const;
      OGPS_Boolean GetY(OGPS_Int32* const value) const;
      OGPS_Boolean GetY(OGPS_Float* const value) const;
      OGPS_Boolean GetY(OGPS_Double* const value) const;

      OGPS_Boolean GetZ(OGPS_Int16* const value) const;
      OGPS_Boolean GetZ(OGPS_Int32* const value) const;
      OGPS_Boolean GetZ(OGPS_Float* const value) const;
      OGPS_Boolean GetZ(OGPS_Double* const value) const;

      void SetX(const OGPS_Int16 value);
      void SetX(const OGPS_Int32 value);
      void SetX(const OGPS_Float value);
      void SetX(const OGPS_Double value);

      void SetY(const OGPS_Int16 value);
      void SetY(const OGPS_Int32 value);
      void SetY(const OGPS_Float value);
      void SetY(const OGPS_Double value);

      void SetZ(const OGPS_Int16 value);
      void SetZ(const OGPS_Int32 value);
      void SetZ(const OGPS_Float value);
      void SetZ(const OGPS_Double value);

      void GetXYZ(
         OGPS_Double* const x,
         OGPS_Double* const y,
         OGPS_Double* const z) const;

      virtual OGPS_Boolean IsValid() const;

      // TODO: muss hier raus (aus dem Interface!)
      virtual OGPS_Boolean SetNull();

      PointVector& operator=(const PointVector& src);

   private:
      DataPoint* m_X;
      DataPoint* m_Y;
      DataPoint* m_Z;
   };
}

#endif /* _OPENGPS_CXX_POINT_VECTOR_HXX */
