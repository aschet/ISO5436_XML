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

#ifndef _OPENGPS_CXX_ISO5436_2_HANDLE_HXX
#define _OPENGPS_CXX_ISO5436_2_HANDLE_HXX

#ifndef _OPENGPS_CXX_OPENGPS_HXX
#  include <opengps/cxx/opengps.hxx>
#endif

#ifndef _OPENGPS_ISO5436_2_HANDLE_H
#  include <opengps/iso5436_2_handle.h>
#endif

namespace xsd
{
   class ISO5436_2Type;
   class Record1Type;
   class Record2Type;
   class MatrixDimensionType;
}

/* Create a new ISO5436-2 file. Optionally specify temporary path, where
unpacked data ogps_Gets stored. Must use ogps_CloseISO5436_2 on this handle. */
OGPS_ISO5436_2Handle ogps_CreateMatrixISO5436_2(
   const OGPS_Character* const file,
   const OGPS_Character* const temp,
   const xsd::Record1Type& record1,
   const xsd::Record2Type& record2,
   const xsd::MatrixDimensionType& matrixDimension,
   const OGPS_Boolean useBinaryData = TRUE);

OGPS_ISO5436_2Handle ogps_CreateListISO5436_2(
   const OGPS_Character* const file,
   const OGPS_Character* const temp,
   const xsd::Record1Type& record1,
   const xsd::Record2Type& record2,
   const unsigned long listDimension,
   const OGPS_Boolean useBinaryData = TRUE);

/* Gets pointer to XML-document with read/write access. Returns NULL on error. */
xsd::ISO5436_2Type * ogps_GetDocument(const OGPS_ISO5436_2Handle handle);

#endif /* _OPENGPS_CXX_ISO5436_2_HANDLE_HXX */
