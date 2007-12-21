#ifndef _OPENGPS_ISO5436_2_HXX
#define _OPENGPS_ISO5436_2_HXX

#ifndef _OPENGPS_HXX
#	include <opengps/opengps.hxx>
#endif
/* TODO:
#ifndef _OPENGPS_AUTO_PTR_TYPES_HXX
#  include "auto_ptr_types.hxx"
#endif

.... */

#include <memory>

namespace xsd
 {
  class ISO5436_2Type;
 }

namespace OpenGPS
{
   class PointBuffer;
   class PointVectorBase;
   class PointIterator;
   class VectorBuffer;
   class VectorBufferBuilder;
   class PointVectorParserBuilder;

   typedef std::auto_ptr<PointBuffer> PointBufferAutoPtr;
   typedef std::auto_ptr<PointVectorBase> PointVectorAutoPtr;

   typedef std::auto_ptr<PointIterator> PointIteratorAutoPtr;
   typedef std::auto_ptr<xsd::ISO5436_2Type> ISO5436_2TypeAutoPtr;

   typedef std::auto_ptr<VectorBuffer> VectorBufferAutoPtr;
   typedef std::auto_ptr<VectorBufferBuilder> VectorBufferBuilderAutoPtr;
   typedef std::auto_ptr<PointVectorParserBuilder> PointVectorParserBuilderAutoPtr;
}

 namespace xsd
 {
  class Record1Type;
  class Record2Type;
  class MatrixDimensionType;
  class ISO5436_2Type;
 }

namespace OpenGPS
{
	class PointVector;

  class ISO5436_2
  {
  protected:
    ISO5436_2(ISO5436_2* instance = NULL);
    
  public:
    ISO5436_2(
            const String& file,
            const String& temp);

    ISO5436_2(
            const String& file);

    /* Open. */
    virtual OGPS_Boolean Open(const OGPS_Boolean readOnly = TRUE);

    /* Create matrix. */
    virtual OGPS_Boolean Create(
            const xsd::Record1Type& record1,
            const xsd::Record2Type& record2,
            const xsd::MatrixDimensionType& matrix,
            const OGPS_Boolean useBinaryData = TRUE);

    /* Create list. */
    virtual OGPS_Boolean Create(
            const xsd::Record1Type& record1,
            const xsd::Record2Type& record2,
            const OGPS_Boolean useBinaryData = TRUE);
    
    virtual ~ISO5436_2();

    virtual PointIteratorAutoPtr CreateNextPointIterator();
    virtual PointIteratorAutoPtr CreatePrevPointIterator();
    
    virtual OGPS_Boolean SetMatrixPoint(
        const unsigned long u,
        const unsigned long v,
        const unsigned long w,
        const PointVector* vector);

    virtual OGPS_Boolean GetMatrixPoint(
        const unsigned long u,
        const unsigned long v,
        const unsigned long w,
        PointVector& vector);

    virtual OGPS_Boolean SetListPoint(
        const unsigned long index,
        const PointVector& vector);

    virtual OGPS_Boolean GetListPoint(
        const unsigned long index,
        PointVector& vector);

    virtual OGPS_Boolean GetMatrixCoord(
        const unsigned long u,
        const unsigned long v,
        const unsigned long w,
        double* x,
        double* y,
        double* z);

    virtual OGPS_Boolean IsMatrixCoordValid(
       unsigned long u,
       unsigned long v,
       unsigned long w);

    virtual OGPS_Boolean GetListCoord(
        const unsigned long index,
        double* x,
        double* y,
        double* z);
    
    virtual const ISO5436_2TypeAutoPtr& GetDocument();
    
    virtual OGPS_Boolean Write();
    
    /* This is implicitly called on delete, but may savely be called
    from your code explicitly. */
    virtual OGPS_Boolean Close();

    ISO5436_2& operator=(const ISO5436_2& src);
    
  private:
    const OGPS_Boolean m_IsProtected;

    /* Internal instance. Either this pointer or ISO5436_2Container instance. */
    ISO5436_2* m_Instance;
  };
}


#endif /* _OPENGPS_ISO5436_2_HXX */
