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

#ifndef _OPENGPS_DATA_POINT_H
#define _OPENGPS_DATA_POINT_H

#ifndef _OPENGPS_H
#  include <opengps/opengps.h>
#endif

#ifndef _OPENGPS_DATA_POINT_TYPE_H
#  include <opengps/data_point_type.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

   /* Incomplete type of data point record. */
   typedef struct _OGPS_DATA_POINT * OGPS_DataPointPtr;
   /* HACK: with the typedef, the const qualifier is not reconised any more.
   #define OGPS_DataPointPtr struct _DATA_POINT * */

   /* Gets the current type stored in this DataPoint. */
   /* if(ogps_GetType(dataPoint)) { ... } checks if dataPoint has a valid
   * data point value, since MissingPointType == 0. */
   /* Returns MissingPointType if a null-pointer is passed. */
   _OPENGPS_EXPORT OGPS_DataPointType ogps_GetDataType(const OGPS_DataPointPtr dataPoint);

   /* Typesafe access functions to DataPoint properties. */
   /* If you access a missing data point or have anticipated the wrong data type
   * (even if types may be compatible) these functions return null (0/0.0). */
   _OPENGPS_EXPORT OGPS_Int16 ogps_GetInt16(const OGPS_DataPointPtr dataPoint);
   _OPENGPS_EXPORT OGPS_Int32 ogps_GetInt32(const OGPS_DataPointPtr dataPoint);
   _OPENGPS_EXPORT OGPS_Float ogps_GetFloat(const OGPS_DataPointPtr dataPoint);
   _OPENGPS_EXPORT OGPS_Double ogps_GetDouble(const OGPS_DataPointPtr dataPoint);

   /* Typesafe write access to DataPoint properties. */
   /* This functions return false if anything went wrong. This should in fact
   * never ever be the case - except when passing a null-pointer. */
   _OPENGPS_EXPORT void ogps_SetInt16(
      OGPS_DataPointPtr const dataPoint,
      const OGPS_Int16 value);
   _OPENGPS_EXPORT void ogps_SetInt32(
      OGPS_DataPointPtr const dataPoint,
      const OGPS_Int32 value);
   _OPENGPS_EXPORT void ogps_SetFloat(
      OGPS_DataPointPtr const dataPoint,
      const OGPS_Float value);
   _OPENGPS_EXPORT void ogps_SetDouble(
      OGPS_DataPointPtr const dataPoint,
      const OGPS_Double value);

#ifdef __cplusplus
}
#endif

#endif /* _OPENGPS_DATA_POINT_H */
