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

PointVectorProxy::PointVectorProxy(const PointVectorProxyContext* const context, VectorBuffer* const buffer)
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

DataPoint* PointVectorProxy::GetX()
{
   _ASSERT(m_X);

   return m_X;
}

DataPoint* PointVectorProxy::GetY()
{
   _ASSERT(m_Y);

   return m_Y;
}

DataPoint* PointVectorProxy::GetZ()
{
   _ASSERT(m_Z);

   return m_Z;
}

OGPS_Boolean PointVectorProxy::Set(const PointVectorBase& value)
{
   _ASSERT(m_X && m_Y && m_Z);

   return ((!m_X->IsValid() || m_X->Set(*value.GetX())) && (!m_Y->IsValid() || m_Y->Set(*value.GetY())) && m_Z->Set(*value.GetZ()));
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

PointVectorProxy::DataPointProxyContext::DataPointProxyContext(PointVectorProxy* const vector)
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

PointVectorProxy::UDataPointProxyContext::UDataPointProxyContext(PointVectorProxy* const vector)
: PointVectorProxy::DataPointProxyContext(vector)
{
}

PointVectorProxy::UDataPointProxyContext::~UDataPointProxyContext()
{
}

unsigned long PointVectorProxy::UDataPointProxyContext::GetIndex() const
{
   _ASSERT(m_Vector && m_Vector->m_Context);

   return m_Vector->m_Context->GetIndex();
}

PointBuffer* PointVectorProxy::UDataPointProxyContext::GetBuffer()
{
   _ASSERT(m_Vector && m_Vector->m_Buffer);

   return m_Vector->m_Buffer->GetX();
}

const PointBuffer* PointVectorProxy::UDataPointProxyContext::GetBuffer() const
{
   _ASSERT(m_Vector && m_Vector->m_Buffer);

   return m_Vector->m_Buffer->GetX();
}

PointVectorProxy::VDataPointProxyContext::VDataPointProxyContext(PointVectorProxy* const vector)
: PointVectorProxy::DataPointProxyContext(vector)
{
}

PointVectorProxy::VDataPointProxyContext::~VDataPointProxyContext()
{
}

unsigned long PointVectorProxy::VDataPointProxyContext::GetIndex() const
{
   _ASSERT(m_Vector && m_Vector->m_Context);

   return m_Vector->m_Context->GetIndex();
}

PointBuffer* PointVectorProxy::VDataPointProxyContext::GetBuffer()
{
   _ASSERT(m_Vector && m_Vector->m_Buffer);

   return m_Vector->m_Buffer->GetY();
}

const PointBuffer* PointVectorProxy::VDataPointProxyContext::GetBuffer() const
{
   _ASSERT(m_Vector && m_Vector->m_Buffer);

   return m_Vector->m_Buffer->GetY();
}

PointVectorProxy::WDataPointProxyContext::WDataPointProxyContext(PointVectorProxy* const vector)
: PointVectorProxy::DataPointProxyContext(vector)
{
}

PointVectorProxy::WDataPointProxyContext::~WDataPointProxyContext()
{
}

unsigned long PointVectorProxy::WDataPointProxyContext::GetIndex() const
{
   _ASSERT(m_Vector && m_Vector->m_Context);

   return m_Vector->m_Context->GetIndex();
}

PointBuffer* PointVectorProxy::WDataPointProxyContext::GetBuffer()
{
   _ASSERT(m_Vector && m_Vector->m_Buffer);

   return m_Vector->m_Buffer->GetZ();
}

const PointBuffer* PointVectorProxy::WDataPointProxyContext::GetBuffer() const
{
   _ASSERT(m_Vector && m_Vector->m_Buffer);

   return m_Vector->m_Buffer->GetZ();
}




PointVectorProxy::DataPointProxy::DataPointProxy(DataPointProxyContext* const context)
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
      const PointBuffer* const buffer = m_Context->GetBuffer();

      if(buffer)
      {
         return buffer->GetType();
      }
   }

   return OGPS_MissingPointType;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Get(OGPS_Int16* const value) const
{
   _ASSERT(m_Context && value);

   const PointBuffer* const buffer = m_Context->GetBuffer();
   if(buffer)
   {
      return buffer->Get(m_Context->GetIndex(), *value);
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Get(OGPS_Int32* const value) const
{
   _ASSERT(m_Context && value);

   const PointBuffer* const buffer = m_Context->GetBuffer();
   if(buffer)
   {
      return buffer->Get(m_Context->GetIndex(), *value);
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Get(OGPS_Float* const value) const
{
   _ASSERT(m_Context && value);

   const PointBuffer* const buffer = m_Context->GetBuffer();
   if(buffer)
   {
      return buffer->Get(m_Context->GetIndex(), *value);
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Get(OGPS_Double* const value) const
{
   _ASSERT(m_Context && value);

   const PointBuffer* const buffer = m_Context->GetBuffer();
   if(buffer)
   {
      return buffer->Get(m_Context->GetIndex(), *value);
   }

   return FALSE;
}

OGPS_Double PointVectorProxy::DataPointProxy::Get() const
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

OGPS_Boolean PointVectorProxy::DataPointProxy::Set(const OGPS_Int16 value)
{
   _ASSERT(m_Context);

   PointBuffer* const buffer = m_Context->GetBuffer();
   if(buffer)
   {
      if(buffer->Set(m_Context->GetIndex(), value))
      {
         return TRUE;
      }
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Set(const OGPS_Int32 value)
{
   _ASSERT(m_Context);

   PointBuffer* const buffer = m_Context->GetBuffer();
   if(buffer)
   {
      if(buffer->Set(m_Context->GetIndex(), value))
      {
         return TRUE;
      }
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Set(const OGPS_Float value)
{
   _ASSERT(m_Context);

   PointBuffer* const buffer = m_Context->GetBuffer();
   if(buffer)
   {
      if(buffer->Set(m_Context->GetIndex(), value))
      {
         return TRUE;
      }
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Set(const OGPS_Double value)
{
   _ASSERT(m_Context);

   PointBuffer* const buffer = m_Context->GetBuffer();
   if(buffer)
   {
      if(buffer->Set(m_Context->GetIndex(), value))
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
      case OGPS_Int16PointType:
         OGPS_Int16 vs;
         if(src.Get(&vs))
         {
            success = Set(vs);
         }
         break;
      case OGPS_Int32PointType:
         OGPS_Int32 vl;
         if(src.Get(&vl))
         {
            success = Set(vl);
         }
         break;
      case OGPS_FloatPointType:
         OGPS_Float vf;
         if(src.Get(&vf))
         {
            success = Set(vf);
         }
         break;
      case OGPS_DoublePointType:
         OGPS_Double vd;
         if(src.Get(&vd))
         {
            success = Set(vd);
         }
         break;
      case OGPS_MissingPointType:
         success = TRUE;
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
