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

#ifndef _OPENGPS_POINT_VECTOR_PROXY_HXX
#define _OPENGPS_POINT_VECTOR_PROXY_HXX

#ifndef _OPENGPS_POINT_VECTOR_BASE_HXX
#  include <opengps/point_vector_base.hxx>
#endif

#ifndef _OPENGPS_DATA_POINT_HXX
#  include <opengps/data_point.hxx>
#endif

namespace OpenGPS
{
   class PointVectorProxyContext;
   class VectorBuffer;
   class PointBuffer;

   class PointVectorProxy : public PointVectorBase
   {
   public:
      PointVectorProxy(
         const PointVectorProxyContext* context,
         const VectorBuffer* buffer);
      virtual ~PointVectorProxy();

      virtual const DataPoint* GetX() const;
      virtual const DataPoint* GetY() const;
      virtual const DataPoint* GetZ() const;

      virtual DataPoint* const GetX();
      virtual DataPoint* const GetY();
      virtual DataPoint* const GetZ();

      virtual OGPS_Boolean Set(const PointVectorBase& value);
      virtual OGPS_Boolean Get(PointVectorBase& value) const;

      virtual OGPS_Boolean SetNull();

   private:
      const PointVectorProxyContext* m_Context;
      const VectorBuffer *m_Buffer;
      
      DataPoint* m_X;
      DataPoint* m_Y;
      DataPoint* m_Z;

      class DataPointProxyContext;

      DataPointProxyContext* m_U;
      DataPointProxyContext* m_V;
      DataPointProxyContext* m_W;

      class DataPointProxyContext
   {
   public:      
      virtual ~DataPointProxyContext();

      virtual unsigned long GetIndex() const = 0;
      virtual PointBuffer* const GetBuffer() const = 0;
      virtual OGPS_Boolean IsValid() const;

   protected:
      DataPointProxyContext(PointVectorProxy* vector);

      PointVectorProxy* m_Vector;
   };

      class UDataPointProxyContext : public DataPointProxyContext
   {
   public:
      UDataPointProxyContext(PointVectorProxy* vector);
      virtual ~UDataPointProxyContext();

      virtual unsigned long GetIndex() const;
      virtual PointBuffer* const GetBuffer() const;
   };

   class VDataPointProxyContext : public DataPointProxyContext
   {
   public:
      VDataPointProxyContext(PointVectorProxy* vector);
      virtual ~VDataPointProxyContext();

      virtual unsigned long GetIndex() const;
      virtual PointBuffer* const GetBuffer() const;
   };

   class WDataPointProxyContext : public DataPointProxyContext
   {
   public:
      WDataPointProxyContext(PointVectorProxy* vector);
      virtual ~WDataPointProxyContext();

      virtual unsigned long GetIndex() const;
      virtual PointBuffer* const GetBuffer() const;
   };

   class DataPointProxy : public DataPoint
   {
   public:
      DataPointProxy(const DataPointProxyContext* context);
      virtual ~DataPointProxy();

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
      const DataPointProxyContext* m_Context;
   };
   };
}

#endif /* _OPENGPS_POINT_VECTOR_PROXY_HXX */
