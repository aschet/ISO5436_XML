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
 * Global handling of error and warning messages.
 */

#ifndef _OPENGPS_MESSAGES_H
#define _OPENGPS_MESSAGES_H

#ifndef _OPENGPS_H
#  include <opengps/opengps.h>
#endif

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
    * Gets the last handled error message or NULL.
    *
    * @remarks This may return NULL even though an error occured. This is because
    * the description of a particular error condition may not be available.
    *
    * @see ::ogps_GetErrorId
    */
   _OPENGPS_EXPORT const OGPS_Character* ogps_GetErrorMessage();

   /*!
    * Gets the last handled detailed error message or NULL.
    *
    * @remarks This may return NULL even though an error occured. This is because
    * the description of a particular error condition may not be available.
    *
    * @see ::ogps_GetErrorId
    */
   _OPENGPS_EXPORT const OGPS_Character* ogps_GetErrorDescription();

   /*!
    * Gets the type of the last handled error condition.
    *
    * @see ::ogps_GetErrorMessage
    */
   _OPENGPS_EXPORT OGPS_ExceptionId ogps_GetErrorId();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* _OPENGPS_MESSAGES_H */
