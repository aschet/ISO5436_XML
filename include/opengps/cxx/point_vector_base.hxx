#ifndef _OPENGPS_POINT_VECTOR_BASE_HXX
#define _OPENGPS_POINT_VECTOR_BASE_HXX

#ifndef _OPENGPS_HXX
#	include <opengps/opengps.hxx>
#endif

namespace OpenGPS
{
   class DataPoint;

   class PointVectorBase
   {
   public:      
      virtual ~PointVectorBase();

      virtual const DataPoint* GetX() const = 0;
      virtual const DataPoint* GetY() const = 0;
      virtual const DataPoint* GetZ() const = 0;

      virtual DataPoint* const GetX() = 0;
      virtual DataPoint* const GetY() = 0;
      virtual DataPoint* const GetZ() = 0;

      virtual OGPS_Boolean SetNull() = 0;

      virtual OGPS_Boolean Set(const PointVectorBase& value) = 0;
      virtual OGPS_Boolean Get(PointVectorBase& value) const = 0;

   protected:
      PointVectorBase();
   };
}

#endif /* _OPENGPS_POINT_VECTOR_BASE_HXX */
