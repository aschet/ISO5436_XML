#ifndef _OPENGPS_ISO5436_2_HANDLE_H
#define _OPENGPS_ISO5436_2_HANDLE_H

#ifndef _OPENGPS_H
#	include <opengps/opengps.h>
#endif

#ifndef _OPENGPS_POINT_VECTOR_H
#include <opengps/point_vector.h>
#endif

#ifndef _OPENGPS_POINT_ITERATOR_H
#include <opengps/point_iterator.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _OGPS_ISO5436_2_HANDLE * OGPS_ISO5436_2Handle;

/* Open an existing ISO5436-2 file. Optionally specify temporary path, where
 unpacked data ogps_Gets stored. Must use ogps_CloseISO5436_2 on this handle. */
OGPS_ISO5436_2Handle ogps_OpenISO5436_2(
        const OGPS_Character* file,
        const OGPS_Character* temp,
        const OGPS_Boolean readOnly);

/* Call before ogps_CloseISO5436_2 to write content. */
OGPS_Boolean ogps_WriteISO5436_2(const OGPS_ISO5436_2Handle handle);

/* Close an ISO5436-2 file handle, either opened by openISO5436_2 or
 * createISO5436_2. This does not write any changed content to disk. */
OGPS_Boolean ogps_CloseISO5436_2(OGPS_ISO5436_2Handle* handle);

/* Returns an iterator or NULL for point sequences of the given handle.
 Must be freed with ogps_FreePointIterator. */
OGPS_PointIteratorPtr ogps_CreateNextPointIterator(const OGPS_ISO5436_2Handle handle);
OGPS_PointIteratorPtr ogps_CreatePrevPointIterator(const OGPS_ISO5436_2Handle handle);

/* Sets data points at position (x_pos, y_pos, z_pos)^T to given values. */
/* To be used for topologically accessable data only (matrix type). */
/* Returns Success on success otherwise indicates what went wrong. */
OGPS_Boolean ogps_SetMatrixPoint(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long u,
        const unsigned long v,
        const unsigned long w,
        const OGPS_PointVectorPtr vector);

/* Gets current values of data points at position (x_pos, y_pos, z_pos)^T. */
/* To be used for topologically accessable data only (matrix type). */
/* Returns Success on success otherwise indicates what went wrong. */
OGPS_Boolean ogps_GetMatrixPoint(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long u,
        const unsigned long v,
        const unsigned long w,
        OGPS_PointVectorPtr const vector);

/* Sets data points at position index. To be used for one-dimensional data. */
/* Returns Success on success otherwise indicates what went wrong. */
OGPS_Boolean ogps_SetListPoint(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long index,
        const OGPS_PointVectorPtr vector);

/* Gets data points at position index. To be used for one-dimensional data. */
/* Returns Success on success otherwise indicates what went wrong. */
OGPS_Boolean ogps_GetListPoint(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long index,
        OGPS_PointVectorPtr const vector);

OGPS_Boolean ogps_GetMatrixCoord(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long u,
        const unsigned long v,
        const unsigned long w,
        double* x,
        double* y,
        double* z);

OGPS_Boolean ogps_IsMatrixCoordValid(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long u,
        const unsigned long v,
        const unsigned long w);

OGPS_Boolean ogps_GetListCoord(
        const OGPS_ISO5436_2Handle handle,
        const unsigned long index,
        double* x,
        double* y,
        double* z);

#ifdef __cplusplus
}
#endif

#endif /* _OPENGPS_ISO5436_2_HANDLE_H */
