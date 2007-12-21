#ifndef _OPENGPS_POINT_ITERATOR_HXX
#define _OPENGPS_POINT_ITERATOR_HXX

#ifndef _OPENGPS_HXX
#	include <opengps/opengps.hxx>
#endif

namespace OpenGPS
{
	class ISO5436_2;
	class PointVector;
  
  class PointIterator
  {
  protected:
    PointIterator();
    
  public:
    virtual ~PointIterator();
    
    virtual OGPS_Boolean HasNext() const = 0;
    virtual OGPS_Boolean HasPrev() const = 0;

    virtual OGPS_Boolean MoveNext() = 0;
    virtual OGPS_Boolean MovePrev() = 0;

    virtual OGPS_Boolean CreateNext() = 0;

    virtual void ResetNext() = 0;
    virtual void ResetPrev() = 0;

    virtual OGPS_Boolean GetCurrent(PointVector& vector) = 0;

    virtual OGPS_Boolean SetCurrent(const PointVector* vector) = 0;
    
    virtual OGPS_Boolean GetPosition(unsigned long* const index) const = 0;
    virtual OGPS_Boolean GetPosition(
            unsigned long* const u,
            unsigned long* const v,
            unsigned long* const w) const = 0;    
  };
}

#endif /* _OPENGPS_POINT_ITERATOR_HXX */
