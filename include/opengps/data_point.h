#ifndef _OPENGPS_DATA_POINT_H
#define _OPENGPS_DATA_POINT_H

#ifndef _OPENGPS_H
#	include <opengps/opengps.h>
#endif

#ifndef _OPENGPS_DATA_POINT_TYPE_H
#	include <opengps/data_point_type.h>
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
OGPS_DataPointType ogps_GetDataType(const OGPS_DataPointPtr dataPoint);

/* Typesafe access functions to DataPoint properties. */
/* If you access a missing data point or have anticipated the wrong data type
 * (even if types may be compatible) these functions return null (0/0.0). */
short ogps_GetInt16(const OGPS_DataPointPtr dataPoint);
int ogps_GetInt32(const OGPS_DataPointPtr dataPoint);
float ogps_GetFloat(const OGPS_DataPointPtr dataPoint);
double ogps_GetDouble(const OGPS_DataPointPtr dataPoint);

/* Typesafe write access to DataPoint properties. */
/* This functions return false if anything went wrong. This should in fact
 * never ever be the case - except when passing a null-pointer. */
void ogps_SetInt16(
	OGPS_DataPointPtr const dataPoint,
	const short value);
void ogps_SetInt32(
	OGPS_DataPointPtr const dataPoint,
	const int value);
void ogps_SetFloat(
	OGPS_DataPointPtr const dataPoint,
	const float value);
void ogps_SetDouble(
	OGPS_DataPointPtr const dataPoint,
	const double value);

#ifdef __cplusplus
}
#endif
	
#endif /* _OPENGPS_DATA_POINT_H */
