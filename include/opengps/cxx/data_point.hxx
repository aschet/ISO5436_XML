#ifndef _OPENGPS_DATA_POINT_HXX
#define _OPENGPS_DATA_POINT_HXX

#ifndef _OPENGPS_HXX
#	include <opengps/opengps.hxx>
#endif

#ifndef _OPENGPS_DATA_POINT_TYPE_H
#	include <opengps/data_point_type.h>
#endif

namespace OpenGPS
{
  class DataPoint
  {
  protected:
    DataPoint() = 0;
    
  public:
    virtual ~DataPoint() = 0;
    
    virtual OGPS_DataPointType GetType() const = 0;

    virtual OGPS_Boolean Get(short* const value) const = 0;
    virtual OGPS_Boolean Get(int* const value) const = 0;
    virtual OGPS_Boolean Get(float* const value) const = 0;
    virtual OGPS_Boolean Get(double* const value) const = 0;
    
    virtual double Get() const = 0;

    virtual OGPS_Boolean IsValid() const = 0;

    virtual OGPS_Boolean Set(const short value) = 0;
    virtual OGPS_Boolean Set(const int value) = 0;
    virtual OGPS_Boolean Set(const float value) = 0;
    virtual OGPS_Boolean Set(const double value) = 0;

    virtual OGPS_Boolean SetNull() = 0;

    virtual OGPS_Boolean Set(const DataPoint& src) = 0;

  protected:
     virtual void Reset() = 0;
  };
}

#endif /* _OPENGPS_DATA_POINT_HXX */
