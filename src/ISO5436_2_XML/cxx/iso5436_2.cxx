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

#include <opengps/cxx/iso5436_2.hxx>

#ifndef _OPENGPS_ISO5436_2_CONTAINER_HXX
#  include "iso5436_2_container.hxx"
#endif

#include "stdafx.hxx"

ISO5436_2::ISO5436_2(ISO5436_2 * instance)
: m_IsProtected(TRUE)
{
   m_Instance = instance ? instance : this;
}

/* Open. */
ISO5436_2::ISO5436_2(
                     const OpenGPS::String& file,
                     const OpenGPS::String& temp)
                     : m_IsProtected(FALSE)
{
   m_Instance = new ISO5436_2Container(file, temp);
}

ISO5436_2::ISO5436_2(const OpenGPS::String& file)
                     : m_IsProtected(FALSE)
{
   m_Instance = new ISO5436_2Container(file, _T("")); /* TODO: emptyOpenGPS::String? */
}

ISO5436_2::~ISO5436_2()
{
   _ASSERT(m_Instance);

   // Prevent ending in a stack overflow (if we call ourselves).
   if(m_Instance != this)
   {
      m_Instance->Close();
   }

   if(!m_IsProtected)
   {
      _ASSERT(m_Instance != this);

      delete m_Instance;
   }
}

OGPS_Boolean ISO5436_2::Open(const OGPS_Boolean readOnly)
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->Open(readOnly);
}

    /* Create matrix. */
OGPS_Boolean ISO5436_2::Create(
            const Schemas::ISO5436_2::Record1Type& record1,
            const Schemas::ISO5436_2::Record2Type& record2,
            const Schemas::ISO5436_2::MatrixDimensionType& matrixDimension,
            const OGPS_Boolean useBinaryData)
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->Create(record1, record2, matrixDimension, useBinaryData);
}

    /* Create list. */
OGPS_Boolean ISO5436_2::Create(
            const Schemas::ISO5436_2::Record1Type& record1,
            const Schemas::ISO5436_2::Record2Type& record2,
            const unsigned long listDimension,
            const OGPS_Boolean useBinaryData)
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->Create(record1, record2, listDimension, useBinaryData);
}

PointIteratorAutoPtr ISO5436_2::CreateNextPointIterator()
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->CreateNextPointIterator();
}

PointIteratorAutoPtr ISO5436_2::CreatePrevPointIterator()
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->CreatePrevPointIterator();
}

OGPS_Boolean ISO5436_2::SetMatrixPoint(
                                 const unsigned long u,
                                 const unsigned long v,
                                 const unsigned long w,
                                 const PointVector* const vector)
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->SetMatrixPoint(u, v, w, vector);
}

OGPS_Boolean ISO5436_2::GetMatrixPoint(
                                 const unsigned long u,
                                 const unsigned long v,
                                 const unsigned long w,
                                 PointVector& vector)
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->GetMatrixPoint(u, v, w, vector);
}

OGPS_Boolean ISO5436_2::SetListPoint(
                               const unsigned long index,
                               const PointVector& vector)
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->SetListPoint(index, vector);
}

OGPS_Boolean ISO5436_2::GetListPoint(
                               const unsigned long index,
                               PointVector& vector)
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->GetListPoint(index, vector);
}

OGPS_Boolean ISO5436_2::GetMatrixCoord(
                                 const unsigned long u,
                                 const unsigned long v,
                                 const unsigned long w,
                                 OGPS_Double* const x,
                                 OGPS_Double* const y,
                                 OGPS_Double* const z)
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->GetMatrixCoord(u, v, w, x, y, z);
}

OGPS_Boolean ISO5436_2::IsMatrixCoordValid(
       unsigned long u,
       unsigned long v,
       unsigned long w)
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->IsMatrixCoordValid(u, v, w);
}

OGPS_Boolean ISO5436_2::GetListCoord(
                               const unsigned long index,
                               OGPS_Double* const x,
                               OGPS_Double* const y,
                               OGPS_Double* const z)
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->GetListCoord(index, x, y, z);
}

ISO5436_2TypeAutoPtr& ISO5436_2::GetDocument()
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->GetDocument();
}

OGPS_Boolean ISO5436_2::Write(const int compressionLevel)
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->Write(compressionLevel);
}

OGPS_Boolean ISO5436_2::Close()
{
   _ASSERT(m_Instance && m_Instance != this);

   return m_Instance->Close();
}
