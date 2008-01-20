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

#ifndef _OPENGPS_POINT_VECTOR_H
#define _OPENGPS_POINT_VECTOR_H

#ifndef _OPENGPS_H
#	include <opengps/opengps.h>
#endif

#ifndef _OPENGPS_DATA_POINT_H
#	include <opengps/data_point.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

   /* Set of three data points corresponding to the current axis definition. */
   typedef struct _OGPS_POINT_VECTOR * OGPS_PointVectorPtr;
   /* HACK: with the typedef, the const qualifier is not reconised any more.
   #define OGPS_PointVectorPtr struct _OGPS_POINT_VECTOR * */

   /* create* functions. */
   /* If instance points to NULL new pre-initalized memory will be allocated
   * and true is returned. Otherwise nothing will happen and false is returned.
   Must be freed with ogps_FreePointVector. */
   _OPENGPS_EXPORT OGPS_PointVectorPtr ogps_CreatePointVector(void);

   /* Frees allocated memory and ogps_Sets given pointer to NULL to be on the safe side.
   * Also it is safe to pass an unallocated null-pointer here. This function will
   * perform an noop then. */
   _OPENGPS_EXPORT void ogps_FreePointVector(OGPS_PointVectorPtr * const vector);

   /* Access functions to PointVector properties. */
   /* Returns the DataPoint stored in PointVector or NULL on failure. */
   _OPENGPS_EXPORT void ogps_SetInt16X(
      OGPS_PointVectorPtr const vector,
      const OGPS_Int16 value);
   _OPENGPS_EXPORT void ogps_SetInt32X(
      OGPS_PointVectorPtr const vector,
      const OGPS_Int32 value);
   _OPENGPS_EXPORT void ogps_SetFloatX(
      OGPS_PointVectorPtr const vector,
      const OGPS_Float value);
   _OPENGPS_EXPORT void ogps_SetDoubleX(
      OGPS_PointVectorPtr const vector,
      const OGPS_Double value);

   _OPENGPS_EXPORT void ogps_SetInt16Y(
      OGPS_PointVectorPtr const vector,
      const OGPS_Int16 value);
   _OPENGPS_EXPORT void ogps_SetInt32Y(
      OGPS_PointVectorPtr const vector,
      const OGPS_Int32 value);
   _OPENGPS_EXPORT void ogps_SetFloatY(
      OGPS_PointVectorPtr const vector,
      const OGPS_Float value);
   _OPENGPS_EXPORT void ogps_SetDoubleY(
      OGPS_PointVectorPtr const vector,
      const OGPS_Double value);

   _OPENGPS_EXPORT void ogps_SetInt16Z(
      OGPS_PointVectorPtr const vector,
      const OGPS_Int16 value);
   _OPENGPS_EXPORT void ogps_SetInt32Z(
      OGPS_PointVectorPtr const vector,
      const OGPS_Int32 value);
   _OPENGPS_EXPORT void ogps_SetFloatZ(
      OGPS_PointVectorPtr const vector,
      const OGPS_Float value);
   _OPENGPS_EXPORT void ogps_SetDoubleZ(
      OGPS_PointVectorPtr const vector,
      const OGPS_Double value);

   _OPENGPS_EXPORT void ogps_GetXYZ(
      const OGPS_PointVectorPtr vector,
      OGPS_Double* const x,
      OGPS_Double* const y,
      OGPS_Double* const z);

   _OPENGPS_EXPORT OGPS_DataPointPtr const ogps_GetX(OGPS_PointVectorPtr const vector);
   _OPENGPS_EXPORT OGPS_DataPointPtr const ogps_GetY(OGPS_PointVectorPtr const vector);
   _OPENGPS_EXPORT OGPS_DataPointPtr const ogps_GetZ(OGPS_PointVectorPtr const vector);

   _OPENGPS_EXPORT OGPS_Int16 ogps_GetInt16X(const OGPS_PointVectorPtr vector);
   _OPENGPS_EXPORT OGPS_Int32 ogps_GetInt32X(const OGPS_PointVectorPtr vector);
   _OPENGPS_EXPORT OGPS_Float ogps_GetFloatX(const OGPS_PointVectorPtr vector);
   _OPENGPS_EXPORT OGPS_Double ogps_GetDoubleX(const OGPS_PointVectorPtr vector);

   _OPENGPS_EXPORT OGPS_Int16 ogps_GetInt16Y(const OGPS_PointVectorPtr vector);
   _OPENGPS_EXPORT OGPS_Int32 ogps_GetInt32Y(const OGPS_PointVectorPtr vector);
   _OPENGPS_EXPORT OGPS_Float ogps_GetFloatY(const OGPS_PointVectorPtr vector);
   _OPENGPS_EXPORT OGPS_Double ogps_GetDoubleY(const OGPS_PointVectorPtr vector);

   _OPENGPS_EXPORT OGPS_Int16 ogps_GetInt16Z(const OGPS_PointVectorPtr vector);
   _OPENGPS_EXPORT OGPS_Int32 ogps_GetInt32Z(const OGPS_PointVectorPtr vector);
   _OPENGPS_EXPORT OGPS_Float ogps_GetFloatZ(const OGPS_PointVectorPtr vector);
   _OPENGPS_EXPORT OGPS_Double ogps_GetDoubleZ(const OGPS_PointVectorPtr vector);

   /* Returns true for valid data point, false otherwise. */
   _OPENGPS_EXPORT OGPS_Boolean ogps_IsValidPoint(const OGPS_PointVectorPtr vector);

#ifdef __cplusplus
}
#endif

#endif /* _OPENGPS_POINT_VECTOR_H */
