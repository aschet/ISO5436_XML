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

#include "point_vector_proxy.hxx"
#include "point_vector_proxy_context.hxx"

#include "vector_buffer.hxx"
#include "point_buffer.hxx"

#include "stdafx.hxx"

PointVectorProxy::PointVectorProxy(const PointVectorProxyContext* context, const VectorBuffer* buffer)
: PointVectorBase(), m_Context(context), m_Buffer(buffer)
{
   _ASSERT(context && buffer);

   m_U = new UDataPointProxyContext(this);
   m_V = new VDataPointProxyContext(this);
   m_W = new WDataPointProxyContext(this);

   m_X = new DataPointProxy(m_U);
   m_Y = new DataPointProxy(m_V);
   m_Z = new DataPointProxy(m_W);
}

PointVectorProxy::~PointVectorProxy()
{
      delete m_X;
      delete m_Y;
      delete m_Z;

      delete m_U;
      delete m_V;
      delete m_W;
}

const DataPoint* PointVectorProxy::GetX() const
{
   _ASSERT(m_X);

   return m_X;
}

const DataPoint* PointVectorProxy::GetY() const
{
   _ASSERT(m_Y);

   return m_Y;
}

const DataPoint* PointVectorProxy::GetZ() const
{
   _ASSERT(m_Z);

   return m_Z;
}

DataPoint* const PointVectorProxy::GetX()
{
   _ASSERT(m_X);

   return m_X;
}

DataPoint* const PointVectorProxy::GetY()
{
   _ASSERT(m_Y);

   return m_Y;
}

DataPoint* const PointVectorProxy::GetZ()
{
   _ASSERT(m_Z);

   return m_Z;
}

OGPS_Boolean PointVectorProxy::Set(const PointVectorBase& value)
{
   _ASSERT(m_X && m_Y && m_Z);

   return (m_X->Set(*value.GetX()) && m_Y->Set(*value.GetY()) && m_Z->Set(*value.GetZ()));
}

OGPS_Boolean PointVectorProxy::Get(PointVectorBase& value) const
{
   _ASSERT(m_X && m_Y && m_Z);

   DataPoint *x = value.GetX();
   DataPoint *y = value.GetY();
   DataPoint *z = value.GetZ();

   _ASSERT(x && y && z);

   return (x->Set(*m_X) && y->Set(*m_Y) && z->Set(*m_Z));
}

OGPS_Boolean PointVectorProxy::SetNull()
{
   _ASSERT(m_X && m_Y && m_Z);

   m_X->SetNull();
   m_Y->SetNull();
   m_Z->SetNull();

   return TRUE;
}

PointVectorProxy::DataPointProxyContext::DataPointProxyContext(PointVectorProxy* vector)
: m_Vector(vector)
{
   _ASSERT(vector);
}

PointVectorProxy::DataPointProxyContext::~DataPointProxyContext()
{
}

OGPS_Boolean PointVectorProxy::DataPointProxyContext::IsValid() const
{
   return GetBuffer() != NULL;
}

PointVectorProxy::UDataPointProxyContext::UDataPointProxyContext(PointVectorProxy* vector)
: PointVectorProxy::DataPointProxyContext(vector)
{
}
      
PointVectorProxy::UDataPointProxyContext::~UDataPointProxyContext()
{
}

unsigned long PointVectorProxy::UDataPointProxyContext::GetIndex() const
{
   _ASSERT(m_Vector && m_Vector->m_Context);

   return m_Vector->m_Context->GetU();
}

PointBuffer* const PointVectorProxy::UDataPointProxyContext::GetBuffer() const
{
   _ASSERT(m_Vector && m_Vector->m_Buffer);

   return m_Vector->m_Buffer->GetX();
}

PointVectorProxy::VDataPointProxyContext::VDataPointProxyContext(PointVectorProxy* vector)
   : PointVectorProxy::DataPointProxyContext(vector)
{
}
      
PointVectorProxy::VDataPointProxyContext::~VDataPointProxyContext()
{
}

unsigned long PointVectorProxy::VDataPointProxyContext::GetIndex() const
{
   _ASSERT(m_Vector && m_Vector->m_Context);

   return m_Vector->m_Context->GetV();
}

PointBuffer* const PointVectorProxy::VDataPointProxyContext::GetBuffer() const
{
   _ASSERT(m_Vector && m_Vector->m_Buffer);

   return m_Vector->m_Buffer->GetY();
}

PointVectorProxy::WDataPointProxyContext::WDataPointProxyContext(PointVectorProxy* vector)
   : PointVectorProxy::DataPointProxyContext(vector)
{
}
      
PointVectorProxy::WDataPointProxyContext::~WDataPointProxyContext()
{
}

unsigned long PointVectorProxy::WDataPointProxyContext::GetIndex() const
{
   _ASSERT(m_Vector && m_Vector->m_Context);

   return m_Vector->m_Context->GetW();
}

PointBuffer* const PointVectorProxy::WDataPointProxyContext::GetBuffer() const
{
   _ASSERT(m_Vector && m_Vector->m_Buffer);

   return m_Vector->m_Buffer->GetZ();
}






PointVectorProxy::DataPointProxy::DataPointProxy(const DataPointProxyContext* context)
: DataPoint(), m_Context(context)
{
   _ASSERT(context);
}

PointVectorProxy::DataPointProxy::~DataPointProxy()
{
}

OGPS_DataPointType PointVectorProxy::DataPointProxy::GetType() const
{
   _ASSERT(m_Context);

   if(m_Context->IsValid())
   {
      PointBuffer* buffer = m_Context->GetBuffer();
   
      if(buffer)
      {
         return buffer->GetType();
      }
   }

   return MissingPointType;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Get(short* const value) const
{
   _ASSERT(m_Context && value);

   PointBuffer* buffer = m_Context->GetBuffer();
   if(buffer)
   {
      return buffer->Get(m_Context->GetIndex(), *value);
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Get(int* const value) const
{
   _ASSERT(m_Context && value);

   PointBuffer* buffer = m_Context->GetBuffer();
   if(buffer)
   {
      return buffer->Get(m_Context->GetIndex(), *value);
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Get(float* const value) const
{
   _ASSERT(m_Context && value);

   PointBuffer* buffer = m_Context->GetBuffer();
   if(buffer)
   {
      return buffer->Get(m_Context->GetIndex(), *value);
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Get(double* const value) const
{
   _ASSERT(m_Context && value);

   PointBuffer* buffer = m_Context->GetBuffer();
   if(buffer)
   {
      return buffer->Get(m_Context->GetIndex(), *value);
   }

   return FALSE;
}
    
double PointVectorProxy::DataPointProxy::Get() const
{
   _ASSERT(FALSE);

   return 0.0;
   // TODO: remove from interface???
}

OGPS_Boolean PointVectorProxy::DataPointProxy::IsValid() const
{
   _ASSERT(m_Context);

   return m_Context->IsValid();
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Set(const short value)
{
   _ASSERT(m_Context);

   PointBuffer* buffer = m_Context->GetBuffer();
   if(buffer)
   {
      if(buffer->Set(m_Context->GetIndex(), value))
      {
         return TRUE;
     }
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Set(const int value)
{
   _ASSERT(m_Context);

   PointBuffer* buffer = m_Context->GetBuffer();
   if(buffer)
   {
      if(buffer->Set(m_Context->GetIndex(), value))
      {
         return TRUE;
     }
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Set(const float value)
{
   _ASSERT(m_Context);

   PointBuffer* buffer = m_Context->GetBuffer();
   if(buffer)
   {
      if(buffer->Set(m_Context->GetIndex(), value))
      {
         return TRUE;
     }
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Set(const double value)
{
   _ASSERT(m_Context);

   PointBuffer* buffer = m_Context->GetBuffer();
   if(buffer)
   {
      if(buffer->Set(m_Context->GetIndex(), value))
      {
         return TRUE;
     }
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::SetNull()
{
   _ASSERT(m_Context);

   PointBuffer* buffer = m_Context->GetBuffer();
   if(buffer)
   {
      if(buffer->SetNull(m_Context->GetIndex()))
      {
         return TRUE;
     }
   }

   return FALSE;
}

void PointVectorProxy::DataPointProxy::Reset()
{
   _ASSERT(FALSE);

   // TODO: m_Context->ResetTarget()
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Set(const DataPoint& src)
{
   _ASSERT(m_Context);

   OGPS_Boolean success = FALSE;

   const OGPS_DataPointType srcType = src.GetType();
   const OGPS_DataPointType thisType = GetType();

   if(srcType == thisType)
   {
      switch(srcType)
      {
      case Int16PointType:
         short vs;
         if(src.Get(&vs))
         {
            success = Set(vs);
         }
         break;
         case Int32PointType:
         int vl;
         if(src.Get(&vl))
         {
            success = Set(vl);
         }
         break;
         case FloatPointType:
         short vf;
         if(src.Get(&vf))
         {
            success = Set(vf);
         }
         break;
         case DoublePointType:
         short vd;
         if(src.Get(&vd))
         {
            success = Set(vd);
         }
         break;
         case MissingPointType:
            break;
         default:
            _ASSERT(FALSE);
            break;
      }
   }

   if(!success)
   {
      Reset();
   }

   return success;
}
