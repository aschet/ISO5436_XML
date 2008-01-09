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

#ifndef _OPENGPS_STDAFX_HXX
#define _OPENGPS_STDAFX_HXX

#include <tchar.h>

using namespace OpenGPS;

#define _OPENGPS_BINFORMAT_INT16_SIZE 2
#define _OPENGPS_BINFORMAT_INT32_SIZE 4
#define _OPENGPS_BINFORMAT_FLOAT_SIZE 4
#define _OPENGPS_BINFORMAT_DOUBLE_SIZE 8

#define _OPENGPS_XSD_ISO5436_NAMESPACE _T("http://www.opengps.eu/xsd/")
#define _OPENGPS_XSD_ISO5436_LOCATION _T("http://www.opengps.eu/xsd/ISO5436_2.xsd")

#define _OPENGPS_XSD_ISO5436_MAIN_PATH _T("main.xml")
#define _OPENGPS_XSD_ISO5436_MAIN_CHECKSUM_PATH _T("md5checksum.hex")

#define _OPENGPS_XSD_ISO5436_DATALINK_PATH _T("bindata/data.bin")
#define _OPENGPS_XSD_ISO5436_VALIDPOINTSLINK_PATH _T("bindata/valid.bin")

// TODO: Unix? / Mac?
#define _OPENGPS_ENV_ISO5436_LOCATION _T("OPENGPS_LOCATION")

#ifdef _DEBUG

#include <assert.h>

#ifndef _ASSERT
#  define _ASSERT(x) assert(x)
#endif

#ifndef _VERIFY
#  define _VERIFY(x) _ASSERT(x)
#endif

#else /* _DEBUG */

#ifndef _ASSERT
#  define _ASSERT(x) ((void)0)
#endif

#ifndef _VERIFY
#  define _VERIFY(x) x
#endif

#endif /* _DEBUG */

#endif /* _OPENGPS_STDAFX_HXX */
