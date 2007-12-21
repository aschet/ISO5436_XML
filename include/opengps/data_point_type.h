#ifndef _OPENGPS_DATA_POINT_TYPE_H
#define _OPENGPS_DATA_POINT_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Possible data types (incorporating missing type information). */
typedef enum _OGPS_DATA_POINT_TYPE {
  MissingPointType, /* Data point where its value is marked as invalid. */
  Int16PointType,
  Int32PointType,
  FloatPointType,
  DoublePointType
} OGPS_DataPointType;

#ifdef __cplusplus
}
#endif

#endif /* _OPENGPS_DATA_POINT_TYPE_H */
