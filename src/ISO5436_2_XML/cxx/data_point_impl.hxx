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

#ifndef _OPENGPS_DATA_POINT_IMPL_HXX
#define _OPENGPS_DATA_POINT_IMPL_HXX

#include <opengps/data_point.hxx>

namespace OpenGPS
{
   class DataPointImpl : public DataPoint
   {
   public:
      DataPointImpl();
      virtual ~DataPointImpl();

      virtual OGPS_DataPointType GetType() const;

      virtual OGPS_Boolean Get(short* const value) const;
    virtual OGPS_Boolean Get(int* const value) const;
    virtual OGPS_Boolean Get(float* const value) const;
    virtual OGPS_Boolean Get(double* const value) const;

      virtual double Get() const;

      virtual OGPS_Boolean IsValid() const;

      virtual OGPS_Boolean Set(const short value);
      virtual OGPS_Boolean Set(const int value);
      virtual OGPS_Boolean Set(const float value);
      virtual OGPS_Boolean Set(const double value);

      virtual OGPS_Boolean SetNull();

      virtual OGPS_Boolean Set(const DataPoint& src);

   protected:
      virtual void Reset();

   private:
      /* Tag: defines which value type is valid. */
      OGPS_DataPointType m_Type;

      /* Possible values by data type. */
      typedef union _OGPS_DATA_POINT_VALUE
      {
         short int16Value;
         int int32Value;
         float floatValue;
         double doubleValue;
      } OGPS_DataPointValue;

      OGPS_DataPointValue m_Value;
   };
}

#endif /* _OPENGPS_DATA_POINT_IMPL_HXX */
