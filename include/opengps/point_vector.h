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
OGPS_PointVectorPtr ogps_CreatePointVector(void);

/* Frees allocated memory and ogps_Sets given pointer to NULL to be on the safe side.
 * Also it is safe to pass an unallocated null-pointer here. This function will
 * perform an noop then. */
void ogps_FreePointVector(OGPS_PointVectorPtr * vector);

/* Access functions to PointVector properties. */
/* Returns the DataPoint stored in PointVector or NULL on failure. */
void ogps_SetInt16X(
	OGPS_PointVectorPtr const vector,
	const short value);
void ogps_SetInt32X(
	OGPS_PointVectorPtr const vector,
	const int value);
void ogps_SetFloatX(
	OGPS_PointVectorPtr const vector,
	const float value);
void ogps_SetDoubleX(
	OGPS_PointVectorPtr const vector,
	const double value);

void ogps_SetInt16Y(
	OGPS_PointVectorPtr const vector,
	const short value);
void ogps_SetInt32Y(
	OGPS_PointVectorPtr const vector,
	const int value);
void ogps_SetFloatY(
	OGPS_PointVectorPtr const vector,
	const float value);
void ogps_SetDoubleY(
	OGPS_PointVectorPtr const vector,
	const double value);

void ogps_SetInt16Z(
	OGPS_PointVectorPtr const vector,
	const short value);
void ogps_SetInt32Z(
	OGPS_PointVectorPtr const vector,
	const int value);
void ogps_SetFloatZ(
	OGPS_PointVectorPtr const vector,
	const float value);
void ogps_SetDoubleZ(
	OGPS_PointVectorPtr const vector,
	const double value);

void ogps_GetXYZ(
        const OGPS_PointVectorPtr vector,
        double* x,
        double* y,
        double* z);
		  
OGPS_DataPointPtr const ogps_GetX(OGPS_PointVectorPtr const vector);
OGPS_DataPointPtr const ogps_GetY(OGPS_PointVectorPtr const vector);
OGPS_DataPointPtr const ogps_GetZ(OGPS_PointVectorPtr const vector);

short ogps_GetInt16X(const OGPS_PointVectorPtr vector);
int ogps_GetInt32X(const OGPS_PointVectorPtr vector);
float ogps_GetFloatX(const OGPS_PointVectorPtr vector);
double ogps_GetDoubleX(const OGPS_PointVectorPtr vector);

short ogps_GetInt16Y(const OGPS_PointVectorPtr vector);
int ogps_GetInt32Y(const OGPS_PointVectorPtr vector);
float ogps_GetFloatY(const OGPS_PointVectorPtr vector);
double ogps_GetDoubleY(const OGPS_PointVectorPtr vector);

short ogps_GetInt16Z(const OGPS_PointVectorPtr vector);
int ogps_GetInt32Z(const OGPS_PointVectorPtr vector);
float ogps_GetFloatZ(const OGPS_PointVectorPtr vector);
double ogps_GetDoubleZ(const OGPS_PointVectorPtr vector);

/* Returns true for valid data point, false otherwise. */
OGPS_Boolean ogps_IsValidPoint(const OGPS_PointVectorPtr vector);

#ifdef __cplusplus
}
#endif

#endif /* _OPENGPS_POINT_VECTOR_H */
