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

#include "point_buffer.hxx"

#include "stdafx.hxx"

PointVectorProxy::DataPointProxy::DataPointProxy(
   const PointVectorProxyContext* const context,
   PointBuffer* const buffer)
: DataPoint(), m_Context(context), m_Buffer(buffer)
{
   _ASSERT(context);
}

PointVectorProxy::DataPointProxy::~DataPointProxy()
{
}

OGPS_DataPointType PointVectorProxy::DataPointProxy::GetType() const
{
   if(m_Buffer)
   {
      return m_Buffer->GetType();
   }

   return OGPS_MissingPointType;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Get(OGPS_Int16* const value) const
{
   _ASSERT(m_Context && value);

   if(m_Buffer)
   {
      return m_Buffer->Get(m_Context->GetIndex(), *value);
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Get(OGPS_Int32* const value) const
{
   _ASSERT(m_Context && value);

   if(m_Buffer)
   {
      return m_Buffer->Get(m_Context->GetIndex(), *value);
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Get(OGPS_Float* const value) const
{
   _ASSERT(m_Context && value);

   if(m_Buffer)
   {
      return m_Buffer->Get(m_Context->GetIndex(), *value);
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Get(OGPS_Double* const value) const
{
   _ASSERT(m_Context && value);

   if(m_Buffer)
   {
      return m_Buffer->Get(m_Context->GetIndex(), *value);
   }

   return FALSE;
}

OGPS_Double PointVectorProxy::DataPointProxy::Get() const
{
   _ASSERT(FALSE);
   return 0.0;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::IsValid() const
{
   return m_Buffer != NULL;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Set(const OGPS_Int16 value)
{
   _ASSERT(m_Context);

   if(m_Buffer)
   {
      return m_Buffer->Set(m_Context->GetIndex(), value);
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Set(const OGPS_Int32 value)
{
   _ASSERT(m_Context);

   if(m_Buffer)
   {
      return m_Buffer->Set(m_Context->GetIndex(), value);
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Set(const OGPS_Float value)
{
   _ASSERT(m_Context);

   if(m_Buffer)
   {
      return m_Buffer->Set(m_Context->GetIndex(), value);
   }

   return FALSE;
}

OGPS_Boolean PointVectorProxy::DataPointProxy::Set(const OGPS_Double value)
{
   _ASSERT(m_Context);

   if(m_Buffer)
   {
      return m_Buffer->Set(m_Context->GetIndex(), value);
   }

   return FALSE;
}

void PointVectorProxy::DataPointProxy::Reset()
{
   // Shouldn't have a buffer, because mainly for an instance of the current
   // data point proxy implementation a reset usually means that a value is skiped
   // when reading. But this is exacly the case, when no explicit point data needs
   // to be read, so that this instance should not contain that buffer either.
   _ASSERT(!m_Buffer);
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

   /* TODO:
   if(!success)
   {
      Reset();
   }
   */

   return success;
}
