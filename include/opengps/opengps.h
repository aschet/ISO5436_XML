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
 * Common define's and typedef's, copyright and license information.
 * @see cxx/opengps.hxx
 */

#ifndef _OPENGPS_H
#define _OPENGPS_H

#ifdef _WIN32
#  ifdef BUILD_ISO5436_2_XML_DLL
/*!
 * Manages export and import of symbols when used as shared library.
 * @remarks If you use openGPS as a shared library within you project
 * you must set the _USE_OPENGPS_LIBRARY flag when compiling your source code!
 */
#     define _OPENGPS_EXPORT __declspec(dllexport)
#  else
#     ifdef BUILD_ISO5436_2_XML
/*!
 * Manages export and import of symbols when used as shared library.
 * @remarks If you use openGPS as a shared library within you project
 * you must set the _USE_OPENGPS_LIBRARY flag when compiling your source code!
 */
#        define _OPENGPS_EXPORT
#    else
/* _USE_OPENGPS_LIBRARY must be specified by the
user of a shared library version of openGPS.
Otherwise no symbols will be imported. This is
needed to distinguish between includes for
static and shared library usage scenarios. */
#        ifdef _USE_OPENGPS_LIBRARY
/*!
 * Manages export and import of symbols when used as shared library.
 * @remarks If you use openGPS as a shared library within you project
 * you must set the _USE_OPENGPS_LIBRARY flag when compiling your source code!
 */
#          define _OPENGPS_EXPORT __declspec(dllimport)
#        else
/*!
 * Manages export and import of symbols when used as shared library.
 * @remarks If you use openGPS as a shared library within you project
 * you must set the _USE_OPENGPS_LIBRARY flag when compiling your source code!
 */
#          define _OPENGPS_EXPORT
#        endif /* _USE_OPENGPS_LIBRARY */
#     endif /* BUILD_ISO5436_2_XML */
#  endif /* BUILD_ISO5436_2_XML_DLL */
#else
/*!
 * Manages export and import of symbols when used as shared library.
 * @remarks If you use openGPS as a shared library within you project
 * you must set the _USE_OPENGPS_LIBRARY flag when compiling your source code!
 */
#  define _OPENGPS_EXPORT
#endif /* _WIN32 */

#ifndef NULL
/*! Initial value for pointers when they do not point to any memory location. */
# define NULL 0
#endif

#ifndef FALSE
# ifdef __cplusplus
/*! The FALSE boolean value. @see OGPS_Boolean */
#   define FALSE false
# else
/*! The FALSE boolean value. @see OGPS_Boolean */
#   define FALSE 0
# endif /* __cplusplus */
#endif /* FALSE */

#ifndef TRUE
# ifdef __cplusplus
/*! The TRUE boolean value. @see OGPS_Boolean */
#   define TRUE true
# else
/*! The TRUE boolean value. @see OGPS_Boolean */
#   define TRUE 1
# endif /* __cplusplus */
#endif /* TRUE */

#ifndef OGPS_Boolean
# ifdef __cplusplus
/*! Holds a boolean value. @see FALSE, TRUE */
typedef bool OGPS_Boolean;
# else
/*! Holds a boolean value. @see FALSE, TRUE */
typedef int OGPS_Boolean;
# endif /* __cplusplus */
#endif /* OGPS_Boolean */

#ifdef _UNICODE
/*! The current type of characters. This is either unicode (wchar_t) or char. */
typedef wchar_t OGPS_Character;
#else
/*! The current type of characters. This is either unicode (wchar_t) or char. */
typedef char OGPS_Character;
#endif /* _UNICODE */

/*! Represents measurement data of type short. */
typedef short OGPS_Int16;
/*! Represents measurement data of type int. */
typedef int OGPS_Int32;
/*! Represents measurement data of type float. */
typedef float OGPS_Float;
/*! Represents measurement data of type double. */
typedef double OGPS_Double;

/*!
 * Possible failure conditions.
 */
typedef enum _OPENGPS_EXCEPTION_ID
{
   /*! No failure condition trapped. */
   OGPS_ExNone,
   /*! A failure condition occured, but it has not been specified in detail. */
   OGPS_ExGeneral,
   /*! An overflow occured. */
   OGPS_ExOverflow
} OGPS_ExceptionId; /*! Possible failure conditions. */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   /*!
    * Gets a short name identifying this software library.
    *
    * @param text Allocated string buffer. Will contain the resulting message on success.
    * @param size The size of the allocated buffer in characters.
    * @returns On success returns the number of characters written, otherwise returns the
    * required size in characters of an allocated text buffer to store the name including
    * the terminating null character.
    */
   _OPENGPS_EXPORT size_t ogps_GetNameInfo(OGPS_Character* const text, const size_t size);
   
   /*!
    * Gets a short message describing the purpose of this software library.
    *
    * @param text Allocated string buffer. Will contain the resulting message on success.
    * @param size The size of the allocated buffer in characters.
    * @returns On success returns the number of characters written, otherwise returns the
    * required size  in characters of an allocated text buffer to store the about message
    * including the terminating null character.
    */
   _OPENGPS_EXPORT size_t ogps_GetAboutInfo(OGPS_Character* const text, const size_t size);
   
   /*!
    * Gets the version identifier of this software library.
    *
    * @param text Allocated string buffer. Will contain the resulting message on success.
    * @param size The size of the allocated buffer in characters.
    * @returns On success returns the number of characters written, otherwise returns the
    * required size  in characters of an allocated text buffer to store the version
    * identifier including the terminating null character.
    */
   _OPENGPS_EXPORT size_t ogps_GetVersionInfo(OGPS_Character* const text, const size_t size);

   /*!
    * Gets the copyright information of this software library.
    *
    * @param text Allocated string buffer. Will contain the resulting message on success.
    * @param size The size of the allocated buffer in characters.
    * @returns On success returns the number of characters written, otherwise returns the
    * required size  in characters of an allocated text buffer to store the copyright
    * information including the terminating null character.
    */
   _OPENGPS_EXPORT size_t ogps_GetCopyrightInfo(OGPS_Character* const text, const size_t size);

   /*!
    * Gets the license information of this software library.
    *
    * @param text Allocated string buffer. Will contain the resulting message on success.
    * @param size The size of the allocated buffer in characters.
    * @returns On success returns the number of characters written, otherwise returns the
    * required size  in characters of an allocated text buffer to store the license
    * information including the terminating null character.
    */
   _OPENGPS_EXPORT size_t ogps_GetLicenseInfo(OGPS_Character* const text, const size_t size);

   /*!
    * Prints the version identifier of this software library to standard output.
    */
   _OPENGPS_EXPORT void ogps_PrintVersionInfo();


   /*!
    * Prints the copyright information of this software library to standard output.
    */
   _OPENGPS_EXPORT void ogps_PrintCopyrightInfo();

   /*!
    * Prints the license information of this software library to standard output.
    */
   _OPENGPS_EXPORT void ogps_PrintLicenseInfo();

   /*!
    * Gets the last handled error message or NULL.
    *
    * @remarks This may return NULL even though an error occured. This is because
    * the description of a particular error condition may not be available.
    *
    * @see ::ogps_GetErrorId
    */
   _OPENGPS_EXPORT const OGPS_Character* ogps_GetErrorMessage();

   /*!
    * Gets the type of the last handled error condition.
    *
    * @see ::ogps_GetErrorMessage
    */
   _OPENGPS_EXPORT OGPS_ExceptionId ogps_GetErrorId();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* _OPENGPS_H */
