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

#ifndef _OPENGPS_H
#define	_OPENGPS_H

#ifndef NULL
# define NULL 0
#endif

#ifndef FALSE
# ifdef __cplusplus
#   define FALSE false
# else
#   define FALSE 0
# endif /* __cplusplus */
#endif /* FALSE */

#ifndef TRUE
# ifdef __cplusplus
#   define TRUE true
# else
#   define TRUE 1
# endif /* __cplusplus */
#endif /* TRUE */

#ifndef OGPS_Boolean
# ifdef __cplusplus
typedef bool OGPS_Boolean;
# else
typedef int OGPS_Boolean;
# endif /* __cplusplus */
#endif /* OGPS_Boolean */

#ifdef _UNICODE
typedef wchar_t OGPS_Character;
#else
typedef char OGPS_Character;
#endif /* _UNICODE */

typedef short OGPS_Int16;
typedef int OGPS_Int32;
typedef float OGPS_Float;
typedef double OGPS_Double;

#endif	/* _OPENGPS_H */
