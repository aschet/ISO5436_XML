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

/*! @file
 * Standard includes and defines.
 */

#ifndef _OPENGPS_STDAFX_HXX
#define _OPENGPS_STDAFX_HXX

#include <tchar.h>

using namespace OpenGPS;

#define _OPENGPS_BINFORMAT_INT16_SIZE 2
#define _OPENGPS_BINFORMAT_INT32_SIZE 4
#define _OPENGPS_BINFORMAT_FLOAT_SIZE 4
#define _OPENGPS_BINFORMAT_DOUBLE_SIZE 8

#define _OPENGPS_XSD_ISO5436_NAMESPACE _T("http://www.opengps.eu/2008/ISO5436_2")
#define _OPENGPS_XSD_ISO5436_LOCATION _T("http://www.opengps.eu/2008/ISO5436_2/ISO5436_2.xsd")

#define _OPENGPS_XSD_ISO5436_MAIN_PATH _T("main.xml")
#define _OPENGPS_XSD_ISO5436_MAIN_CHECKSUM_PATH _T("md5checksum.hex")

#define _OPENGPS_XSD_ISO5436_DATALINK_PATH _T("bindata/data.bin")
#define _OPENGPS_XSD_ISO5436_VALIDPOINTSLINK_PATH _T("bindata/valid.bin")

// TODO: Unix? / Mac?
#define _OPENGPS_ENV_OPENGPS_LOCATION _T("OPENGPS_LOCATION")
#define _OPENGPS_ISO5436_LOCATION _T("iso5436_2.xsd")

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

#define _OPENGPS_EXCEPTION_MESG(x) x

/// Version definition for the openGPS library
// TODO: extracted the build number and version from the SVN repository automatically

/// Name of the program
#define _OPENGPS_NAME _T("openGPS ISO 5436-2 XML")
/// Short description of the library
#define _OPENGPS_DESCRIPTION _T("openGPS class library implementing an xml-version of ISO 5436-2 file format.")


/// Major program version. This is increased manually in the release process
#define _OPENGPS_VERSION 0
/// Minor program revision. This is increased manually in the release process
#define _OPENGPS_MINVERSION 1
/// Build number, automatically extracted from SVN repository
#define _OPENGPS_BUILD 1
/// openGPS revision (currently unused)
#define _OPENGPS_REVISION 0


/// Build a version string from version numbers
// This macro is necessary to make a number to string conversion
#ifdef _UNICODE
#  define _OPENGPS_MSTR(x) L#x
#else
#  define _OPENGPS_MSTR(x) #x
#endif /* _UNICODE */

#define _OPENGPS_VERSIONSTRING_M(ver,mver,build,rev) _OPENGPS_MSTR(ver) _T(".") _OPENGPS_MSTR(mver) _T(".") _OPENGPS_MSTR(build) _T(".") _OPENGPS_MSTR(rev)

/// Build a version string
#define _OPENGPS_VERSIONSTRING _OPENGPS_VERSIONSTRING_M(_OPENGPS_VERSION, _OPENGPS_MINVERSION, _OPENGPS_BUILD, _OPENGPS_REVISION)

/// Define ID string with programm name and Version
#define _OPENGPS_ID _OPENGPS_NAME _T(" (V") _OPENGPS_VERSIONSTRING _T(")")

#define _OPENGPS_DELETE(x) if((x) != NULL) { delete (x); (x) = NULL; }
#define _OPENGPS_DELETE_ARRAY(x) if((x) != NULL) { delete[] (x); (x) = NULL; }
#define _OPENGPS_FREE(x) if((x) != NULL) { free(x); (x) = NULL; }

#endif /* _OPENGPS_STDAFX_HXX */
