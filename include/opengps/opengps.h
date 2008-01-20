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
#define _OPENGPS_H

#ifdef _WIN32
#  ifdef BUILD_ISO5436_2_XML_DLL
#     define _OPENGPS_EXPORT __declspec(dllexport)
#  else
#     ifdef BUILD_ISO5436_2_XML
#        define _OPENGPS_EXPORT
#    else
#        define _OPENGPS_EXPORT __declspec(dllimport)
#     endif /* BUILD_ISO5436_2_XML */
#  endif /* BUILD_ISO5436_2_XML_DLL */
#else
#  define _OPENGPS_EXPORT
#endif /* _WIN32 */

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

typedef enum _OPENGPS_EXCEPTION_ID
{
   OGPS_ExNone,
   OGPS_ExGeneral,
   OGPS_ExOverflow
} OGPS_ExceptionId;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

_OPENGPS_EXPORT size_t ogps_get_name_info(OGPS_Character* const text, const size_t size);
_OPENGPS_EXPORT size_t ogps_get_software_info(OGPS_Character* const text, const size_t size);
_OPENGPS_EXPORT size_t ogps_get_version_info(OGPS_Character* const text, const size_t size);

_OPENGPS_EXPORT void ogps_print_version_info();

_OPENGPS_EXPORT size_t ogps_get_copyright_info(OGPS_Character* const text, const size_t size);
_OPENGPS_EXPORT void ogps_print_copyright_info();

_OPENGPS_EXPORT size_t ogps_get_license_info(OGPS_Character* const text, const size_t size);
_OPENGPS_EXPORT void ogps_print_license_info();

_OPENGPS_EXPORT const OGPS_Character* ogps_get_error_message();
_OPENGPS_EXPORT OGPS_ExceptionId ogps_get_error_id();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* _OPENGPS_H */
