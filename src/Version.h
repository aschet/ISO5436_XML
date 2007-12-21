/***************************************************************************
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
 *   openGPS and the openGPS logo is a registered trademark of             *
 *   Physikalisch Technische Bundesanstalt (PTB)                           *
 *   http://www.ptb.de/                                                    *
 *                                                                         *
 *   More information about openGPS can be found at                        *
 *   http://www.opengps.eu/                                                *
 ***************************************************************************/

/// Version definition for the openGPS library
// TODO: extracted the build number and version from the SVN repository automatically
//
// (C) by Georg Wiora, NanoFocus AG
// 27-Mar-2007

/// Name of the program
#define OPENGPS_NAME "openGPS ISO 5436-2 XML"
/// Short description of the library
#define OPENGPS_DESCRIPTION "openGPS class library implementing an xml-version of ISO 5436-2 file format."


/// Major program version. This is increased manually in the release process
#define OPENGPS_VERSION 0
/// Minor program revision. This is increased manually in the release process
#define OPENGPS_MINVERSION 1
/// Build number, automatically extracted from SVN repository
#define OPENGPS_BUILD 1
/// openGPS revision (currently unused)
#define OPENGPS_REVISION 0


/// Build a version string from version numbers
// This macro is necessary to make a number to string conversion
#define MSTR(x) #x
#define OPENGPS_VERSIONSTRING_M(ver,mver,build,rev) MSTR(ver) "." MSTR(mver) "." MSTR(build) "." MSTR(rev)

/// Build a version string
#define OPENGPS_VERSIONSTRING OPENGPS_VERSIONSTRING_M(OPENGPS_VERSION, OPENGPS_MINVERSION, OPENGPS_BUILD, OPENGPS_REVISION)

/// Define ID string with programm name and Version
#define OPENGPS_ID OPENGPS_NAME " (V" OPENGPS_VERSIONSTRING ")"
