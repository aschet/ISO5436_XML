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

#include "data_point_impl.hxx"

#include <opengps/point_vector.hxx>

#include "stdafx.hxx"

PointVectorBase::PointVectorBase()
{
}

PointVectorBase::~PointVectorBase()
{
}

PointVector::PointVector() : PointVectorBase()
	 {
	 m_X = new DataPointImpl();
	 m_Y = new DataPointImpl();
	 m_Z = new DataPointImpl();
	 }
PointVector::~PointVector()
	 {
	 delete m_X;
	 delete m_Y;
	 delete m_Z;
	 }
    
const DataPoint* PointVector::GetX() const
	 {
       _ASSERT(m_X);
	 return m_X;
	 }
    const DataPoint* PointVector::GetY() const
	 {
       _ASSERT(m_Y);
	 return m_Y;
	 }
    const DataPoint* PointVector::GetZ() const
	 {
       _ASSERT(m_Z);
	 return m_Z;
	 }

    DataPoint* const PointVector::GetX()
	 {
       _ASSERT(m_X);
	 return m_X;
	 }
    DataPoint* const PointVector::GetY()
	 {
       _ASSERT(m_Y);
	 return m_Y;
	 }
    DataPoint* const PointVector::GetZ()
	 {
       _ASSERT(m_Z);
	 return m_Z;
	 }

    OGPS_Boolean PointVector::GetX(short* const value) const
   {
      _ASSERT(value && m_X);

      return m_X->Get(value);
   }

   OGPS_Boolean PointVector::GetX(int* const value) const
   {
      _ASSERT(value && m_X);

      return m_X->Get(value);
   }

   OGPS_Boolean PointVector::GetX(float* const value) const
   {
      _ASSERT(value && m_X);

      return m_X->Get(value);
   }

   OGPS_Boolean PointVector::GetX(double* const value) const
   {
      _ASSERT(value && m_X);

      return m_X->Get(value);
   }
    
   OGPS_Boolean PointVector::GetY(short* const value) const
   {
      _ASSERT(value && m_Y);

      return m_Y->Get(value);
   }

   OGPS_Boolean PointVector::GetY(int* const value) const
   {
      _ASSERT(value && m_Y);

      return m_Y->Get(value);
   }

   OGPS_Boolean PointVector::GetY(float* const value) const
   {
      _ASSERT(value && m_Y);

      return m_Y->Get(value);
   }

   OGPS_Boolean PointVector::GetY(double* const value) const
   {
      _ASSERT(value && m_Y);

      return m_Y->Get(value);
   }

   OGPS_Boolean PointVector::GetZ(short* const value) const
   {
      _ASSERT(value && m_Z);

      return m_Z->Get(value);
   }

   OGPS_Boolean PointVector::GetZ(int* const value) const
   {
      _ASSERT(value && m_Z);

      return m_Z->Get(value);
   }

   OGPS_Boolean PointVector::GetZ(float* const value) const
   {
      _ASSERT(value && m_Z);

      return m_Z->Get(value);
   }

   OGPS_Boolean PointVector::GetZ(double* const value) const
   {
      _ASSERT(value && m_Z);

      return m_Z->Get(value);
   }

    void PointVector::SetX(const short value)
	 {
       _ASSERT(m_X);
	 m_X->Set(value);
	 }
    void PointVector::SetX(const int value)
	 {
       _ASSERT(m_X);
	 m_X->Set(value);
	 }
    void PointVector::SetX(const float value)
	 {
       _ASSERT(m_X);
	 m_X->Set(value);
	 }
    void PointVector::SetX(const double value)
	 {
       _ASSERT(m_X);
	 m_X->Set(value);
	 }

    void PointVector::SetY(const short value)
	 {
       _ASSERT(m_Y);
	 m_Y->Set(value);
	 }
    void PointVector::SetY(const int value)
	 {
       _ASSERT(m_Y);
	 m_Y->Set(value);
	 }
    void PointVector::SetY(const float value)
	 {
       _ASSERT(m_Y);
	 m_Y->Set(value);
	 }
    void PointVector::SetY(const double value)
	 {
       _ASSERT(m_Y);
	 m_Y->Set(value);
	 }

    void PointVector::SetZ(const short value)
	 {
       _ASSERT(m_Z);
	 m_Z->Set(value);
	 }
    void PointVector::SetZ(const int value)
	 {
       _ASSERT(m_Z);
	 m_Z->Set(value);
	 }
    void PointVector::SetZ(const float value)
	 {
       _ASSERT(m_Z);
	 m_Z->Set(value);
	 }
    void PointVector::SetZ(const double value)
	 {
       _ASSERT(m_Z);
	 m_Z->Set(value);
	 }
	 

    void PointVector::GetXYZ(
        double* x,
        double* y,
        double* z) const
	 {
	 if(x != NULL)
	 {
       _ASSERT(m_X);
		*x = m_X->Get();
		}
		
		if(y != NULL)
	 {
       _ASSERT(m_Y);
		*y = m_Y->Get();
		}
		
		if(z != NULL)
	 {
       _ASSERT(m_Z);
		*z = m_Z->Get();
		}
	 }
    
    OGPS_Boolean PointVector::IsValid() const
	 {
       _ASSERT(m_X && m_Y && m_Z);
	 return m_X->IsValid() && m_Y->IsValid() && m_Z->IsValid();
	 }

    PointVector& PointVector::operator=(const PointVector& src)
	 {
       _ASSERT(m_X && m_Y && m_Z);
       _ASSERT(src.m_X && src.m_Y && src.m_Z);
/*
		*m_X = *src.m_X;
		*m_Y = *src.m_Y;
		*m_Z = *src.m_Z;
*/
       Set(src);

      return *this;
	 }

    OGPS_Boolean PointVector::SetNull()
    {
       _ASSERT(m_X && m_Y && m_Z);

       return m_X->SetNull() & m_Y->SetNull() & m_Z->SetNull();
    }

    OGPS_Boolean PointVector::Set(const PointVectorBase& value)
    {
       _ASSERT(m_X && m_Y && m_Z);

       _ASSERT(value.GetX() && value.GetY() && value.GetZ());

       return (m_X->Set(*value.GetX()) &&
         m_Y->Set(*value.GetY()) &&
         m_Z->Set(*value.GetZ()));
    }

    OGPS_Boolean PointVector::Get(PointVectorBase& value) const
    {
       _ASSERT(m_X && m_Y && m_Z);

       _ASSERT(value.GetX() && value.GetY() && value.GetZ());

       return (value.GetX()->Set(*m_X) &&
         value.GetY()->Set(*m_Y) &&
         value.GetZ()->Set(*m_Z));
    }
