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

#include <opengps/iso5436_2_handle.h>
#include <opengps/cxx/iso5436_2_handle.hxx>

#include "iso5436_2_handle.hxx"
#include "point_iterator.hxx"
#include "point_vector.hxx"

#include "../cxx/iso5436_2_container.hxx"
#include "../cxx/stdafx.hxx"

OGPS_ISO5436_2Handle ogps_OpenISO5436_2(
        const OGPS_Character* const file,
        const OGPS_Character* const temp,
        const OGPS_Boolean readOnly)
{
   _ASSERT(file);

   OGPS_ISO5436_2Handle h = new OGPS_ISO5436_2;

   h->instance = new OpenGPS::ISO5436_2Container(
      file,
      temp ? temp : _T(""));

   if(!h->instance->Open(readOnly))
   {
      delete h;
      return NULL;
   }

   return h;
}

OGPS_ISO5436_2Handle ogps_CreateMatrixISO5436_2(
        const OGPS_Character* const file,
        const OGPS_Character* const temp,
        const xsd::Record1Type& record1,
        const xsd::Record2Type& record2,
        const xsd::MatrixDimensionType& matrixDimension,
        const OGPS_Boolean useBinaryData)
{
   _ASSERT(file);

   OGPS_ISO5436_2Handle h = new OGPS_ISO5436_2;

   h->instance = new OpenGPS::ISO5436_2Container(
      file,
      temp ? temp : _T(""));

   if(!h->instance->Create(
      record1,
      record2,
      matrixDimension,
      useBinaryData))
   {
      delete h;
      return NULL;
   }

   return h;
}

OGPS_ISO5436_2Handle ogps_CreateListISO5436_2(
        const OGPS_Character* file,
        const OGPS_Character* temp,
        const xsd::Record1Type& record1,
        const xsd::Record2Type& record2,
        const unsigned long listDimension,
        const OGPS_Boolean useBinaryData)
{
   _ASSERT(file);

   OGPS_ISO5436_2Handle h = new OGPS_ISO5436_2;

   h->instance = new OpenGPS::ISO5436_2Container(
      file,
      temp ? temp : _T(""));

   if(!h->instance->Create(
      record1,
      record2,
      listDimension,
      useBinaryData))
   {
      delete h;
      return NULL;
   }

   return h;
}

xsd::ISO5436_2Type * ogps_GetDocument(const OGPS_ISO5436_2Handle handle)
{
   _ASSERT(handle && handle->instance);

   return handle->instance->GetDocument().get();
}

OGPS_Boolean ogps_WriteISO5436_2(const OGPS_ISO5436_2Handle handle, const int compressionLevel)
{
   _ASSERT(handle && handle->instance);

   return handle->instance->Write(compressionLevel);
}

OGPS_Boolean ogps_CloseISO5436_2(OGPS_ISO5436_2Handle* handle)
{
   _ASSERT(handle);

   OGPS_ISO5436_2Handle h = *handle;

   if(h)
   {
      _ASSERT(h->instance);

      OGPS_Boolean closed = h->instance->Close();

      delete h->instance;

      delete h;
      h = NULL;

      return closed;
   }

   return TRUE;
}

OGPS_PointIteratorPtr ogps_CreateNextPointIterator(const OGPS_ISO5436_2Handle handle)
{
   _ASSERT(handle && handle->instance);

   OGPS_PointIteratorPtr iter = new OGPS_PointIterator();
   iter->instance = handle->instance->CreateNextPointIterator().release();

   return iter;
}

OGPS_PointIteratorPtr ogps_CreatePrevPointIterator(const OGPS_ISO5436_2Handle handle)
{
   _ASSERT(handle && handle->instance);

   OGPS_PointIteratorPtr iter = new OGPS_PointIterator;
   iter->instance = handle->instance->CreatePrevPointIterator().release();

   return iter;
}

OGPS_Boolean ogps_SetMatrixPoint(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long u,
        const unsigned long v,
        const unsigned long w,
        const OGPS_PointVectorPtr vector)
{
   _ASSERT(handle && handle->instance);

   return handle->instance->SetMatrixPoint(u, v, w, vector ? &vector->instance : NULL);
}

OGPS_Boolean ogps_GetMatrixPoint(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long u,
        const unsigned long v,
        const unsigned long w,
        OGPS_PointVectorPtr const vector)
{
   _ASSERT(handle && handle->instance && vector);

   return handle->instance->GetMatrixPoint(u, v, w, vector->instance);
}

OGPS_Boolean ogps_SetListPoint(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long index,
        const OGPS_PointVectorPtr vector)
{
   _ASSERT(handle && handle->instance && vector);

   return handle->instance->SetListPoint(index, vector->instance);
}

OGPS_Boolean ogps_GetListPoint(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long index,
        OGPS_PointVectorPtr const vector)
{
   _ASSERT(handle && handle->instance && vector);

   return handle->instance->GetListPoint(index, vector->instance);
}

OGPS_Boolean ogps_GetMatrixCoord(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long u,
        const unsigned long v,
        const unsigned long w,
        OGPS_Double* const x,
        OGPS_Double* const y,
        OGPS_Double* const z)
{
   _ASSERT(handle && handle->instance);

   return handle->instance->GetMatrixCoord(u, v, w, x, y, z);
}

OGPS_Boolean ogps_IsMatrixCoordValid(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long u,
        const unsigned long v,
        const unsigned long w)
{
   _ASSERT(handle && handle->instance);

   return handle->instance->IsMatrixCoordValid(u, v, w);
}

OGPS_Boolean ogps_GetListCoord(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long index,
        OGPS_Double* const x,
        OGPS_Double* const y,
        OGPS_Double* const z)
{
   _ASSERT(handle && handle->instance);

   return handle->instance->GetListCoord(index, x, y, z);
}
