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
OGPS_Boolean ogps_HasNextPoint(const OGPS_PointIteratorPtr iterator);

/* Returns TRUE, if there is a previous point available, FALSE otherwise. */
OGPS_Boolean ogps_HasPrevPoint(const OGPS_PointIteratorPtr iterator);

/* Moves the inner counter forward. Returns TRUE on success, otherwise FALSE. */
OGPS_Boolean ogps_MoveNextPoint(OGPS_PointIteratorPtr const iterator);

/* Moves the inner counter backward. Returns TRUE on success, otherwise FALSE. */
OGPS_Boolean ogps_MovePrevPoint(OGPS_PointIteratorPtr const iterator);

/* When moving forward and the inner index is set to the outermost index
 available plus one. This creates a new point buffer. Use ogps_SetCurrentPoint to
 set its value and add this point to the document. Returns TRUE if a new point
 buffer has been allocated, FALSE otherwise and this function
 behaves the same as ogps_MoveNextPoint. */
OGPS_Boolean ogps_CreateNextPoint(OGPS_PointIteratorPtr const iterator);

/* Resets the inner index to the beginning. */
void ogps_ResetNextPointIterator(OGPS_PointIteratorPtr const iterator);

/* Resets the inner index to the end. */
void ogps_ResetPrevPointIterator(OGPS_PointIteratorPtr const iterator);

/* Reads the current point vector. Returns TRUE on success, FALSE otherwise. */
OGPS_Boolean ogps_GetCurrentPoint(
        const OGPS_PointIteratorPtr iterator,
        OGPS_PointVectorPtr const vector);

/* Sets the value of the current point vector. Returns TRUE on success, FALSE
 otherwise. If the current point had previously been allocated by
 ogps_CreateNextPoint this buffer will be added to the document only if there
 the outermost index available has not changed. This function will fail
 otherwise and free the inner point buffer. */
OGPS_Boolean ogps_SetCurrentPoint(
        const OGPS_PointIteratorPtr iterator,
        const OGPS_PointVectorPtr vector);

/* Retrieves the current position of the iterator in matrix coordinates. */
OGPS_Boolean ogps_GetMatrixPosition(
        const OGPS_PointIteratorPtr iterator,
        unsigned long* u,
        unsigned long* v,
        unsigned long* w);

OGPS_Boolean ogps_GetListPosition(
        const OGPS_PointIteratorPtr iterator,
        unsigned long* index);

/* Frees the point iterator. */
void ogps_FreePointIterator(OGPS_PointIteratorPtr * iterator);

#ifdef __cplusplus
}
#endif

#endif /* _OPENGPS_POINT_ITERATOR_H */
