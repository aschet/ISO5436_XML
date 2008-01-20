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

#ifndef _OPENGPS_POINT_ITERATOR_H
#define _OPENGPS_POINT_ITERATOR_H

#ifndef _OPENGPS_H
#	include <opengps/opengps.h>
#endif

#ifndef _OPENGPS_POINT_VECTOR_H
#	include <opengps/point_vector.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

   /* Point iterator interface. */
   typedef struct _OGPS_POINT_ITERATOR * OGPS_PointIteratorPtr;

   /* Returns TRUE, if there is another point available, FALSE otherwise. */
   _OPENGPS_EXPORT OGPS_Boolean ogps_HasNextPoint(const OGPS_PointIteratorPtr iterator);

   /* Returns TRUE, if there is a previous point available, FALSE otherwise. */
   _OPENGPS_EXPORT OGPS_Boolean ogps_HasPrevPoint(const OGPS_PointIteratorPtr iterator);

   /* Moves the inner counter forward. Returns TRUE on success, otherwise FALSE. */
   _OPENGPS_EXPORT OGPS_Boolean ogps_MoveNextPoint(OGPS_PointIteratorPtr const iterator);

   /* Moves the inner counter backward. Returns TRUE on success, otherwise FALSE. */
   _OPENGPS_EXPORT OGPS_Boolean ogps_MovePrevPoint(OGPS_PointIteratorPtr const iterator);

   /* Resets the inner index to the beginning. */
   _OPENGPS_EXPORT void ogps_ResetNextPointIterator(OGPS_PointIteratorPtr const iterator);

   /* Resets the inner index to the end. */
   _OPENGPS_EXPORT void ogps_ResetPrevPointIterator(OGPS_PointIteratorPtr const iterator);

   /* Reads the current point vector. Returns TRUE on success, FALSE otherwise. */
   _OPENGPS_EXPORT OGPS_Boolean ogps_GetCurrentPoint(
      const OGPS_PointIteratorPtr iterator,
      OGPS_PointVectorPtr const vector);

   _OPENGPS_EXPORT OGPS_Boolean ogps_SetCurrentPoint(
      const OGPS_PointIteratorPtr iterator,
      const OGPS_PointVectorPtr vector);

   /* Retrieves the current position of the iterator in matrix coordinates. */
   _OPENGPS_EXPORT OGPS_Boolean ogps_GetMatrixPosition(
      const OGPS_PointIteratorPtr iterator,
      unsigned long* u,
      unsigned long* v,
      unsigned long* w);

   _OPENGPS_EXPORT OGPS_Boolean ogps_GetListPosition(
      const OGPS_PointIteratorPtr iterator,
      unsigned long* index);

   /* Frees the point iterator. */
   _OPENGPS_EXPORT void ogps_FreePointIterator(OGPS_PointIteratorPtr * const iterator);

#ifdef __cplusplus
}
#endif

#endif /* _OPENGPS_POINT_ITERATOR_H */
