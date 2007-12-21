#ifndef _OPENGPS_POINT_VECTOR_HXX
#define _OPENGPS_POINT_VECTOR_HXX

#ifndef _OPENGPS_HXX
#	include <opengps/opengps.hxx>
#endif

#ifndef _OPENGPS_POINT_VECTOR_BASE_HXX
#  include <opengps/point_vector_base.hxx>
#endif

namespace OpenGPS
{
   class DataPoint;

   class PointVector : public PointVectorBase
   {
   public:
      PointVector();
      ~PointVector();

      virtual const DataPoint* GetX() const;
      virtual const DataPoint* GetY() const;
      virtual const DataPoint* GetZ() const;

      virtual DataPoint* const GetX();
      virtual DataPoint* const GetY();
      virtual DataPoint* const GetZ();

      virtual OGPS_Boolean Set(const PointVectorBase& value);
      virtual OGPS_Boolean Get(PointVectorBase& value) const;

      // TODO: really need return value here?
      OGPS_Boolean GetX(short* value) const;
      OGPS_Boolean GetX(int* value) const;
      OGPS_Boolean GetX(float* value) const;
      OGPS_Boolean GetX(double* value) const;

      OGPS_Boolean GetY(short* value) const;
      OGPS_Boolean GetY(int* value) const;
      OGPS_Boolean GetY(float* value) const;
      OGPS_Boolean GetY(double* value) const;

      OGPS_Boolean GetZ(short* value) const;
      OGPS_Boolean GetZ(int* value) const;
      OGPS_Boolean GetZ(float* value) const;
      OGPS_Boolean GetZ(double* value) const;

      void SetX(const short value);
      void SetX(const int value);
      void SetX(const float value);
      void SetX(const double value);

      void SetY(const short value);
      void SetY(const int value);
      void SetY(const float value);
      void SetY(const double value);

      void SetZ(const short value);
      void SetZ(const int value);
      void SetZ(const float value);
      void SetZ(const double value);

      void GetXYZ(
         double* x,
         double* y,
         double* z) const;

      virtual OGPS_Boolean IsValid() const;

      virtual OGPS_Boolean SetNull();

      PointVector& operator=(const PointVector& src);

   private:
      DataPoint* m_X;
      DataPoint* m_Y;
      DataPoint* m_Z;
   };
}

#endif /* _OPENGPS_POINT_VECTOR_HXX */
