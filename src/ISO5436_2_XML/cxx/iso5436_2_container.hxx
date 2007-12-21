/***************************************************************************
 *   Copyright by Johannes Herwig (NanoFocus AG) 2007                      *
 *   Copyright by Georg Wiora (NanoFocus AG) 2007                          *
 *                                                                         *
 *   This file is part of the openGPS (R)[TM] software library.            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License (LGPL)    *
 *   as published by the Free Software Foundation; either version 3 of     *
 *   the License, or (at your option) any later version.                   *
 *   for detail see the files "licence_LGPL-3.0.txt" and                   *
 *   "licence_GPL-3.0.txt".                                                *
 *                                                                         *
 *   openGPS is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 *   The name "openGPS" and the logo are registered as                     *
 *   European trade mark No. 006178354 for                                 *
 *   Physikalisch Technische Bundesanstalt (PTB)                           *
 *   http://www.ptb.de/                                                    *
 *                                                                         *
 *   More information about openGPS can be found at                        *
 *   http://www.opengps.eu/                                                *
 ***************************************************************************/

#ifndef _OPENGPS_ISO5436_2_CONTAINER_HXX
#define _OPENGPS_ISO5436_2_CONTAINER_HXX

#ifndef _OPENGPS_ISO5436_2_HXX
#  include <opengps/iso5436_2.hxx>
#endif

#ifndef _OPENGPS_DATA_POINT_TYPE_H
#  include <opengps/data_point_type.h>
#endif

#ifndef _OPENGPS_AUTO_PTR_TYPES_HXX
#  include "auto_ptr_types.hxx"
#endif

#ifndef _OPENGPS_POINT_VECTOR_PROXY_CONTEXT_HXX
#  include "point_vector_proxy_context.hxx"
#endif

#include <zip.h>

namespace xsd
{
   class AxisDescriptionType;
}

namespace OpenGPS
{
   class PointVectorParser;
   class PointVectorParserBuilder;
   class VectorBufferBuilder;
   class PointVectorReaderContext;
   class PointVectorWriterContext;

   class ISO5436_2Container : public ISO5436_2
   {
  public:    
    ISO5436_2Container(
            const String& file,
            const String& temp);
    
    virtual ~ISO5436_2Container();

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

    /////////////////////////
    //// NON-ISO5436_2 interface starts here.
    /////////////////////////

    ISO5436_2Container& operator=(const ISO5436_2Container& src);

    /* Says whether points are stored in a matrix or list. */
    OGPS_Boolean IsMatrix() const;

    /* Check whether we store points as binary or xml. */
    OGPS_Boolean IsBinary() const;

    OGPS_Boolean HasValidPointsLink() const;

    unsigned long GetMaxU() const;
    unsigned long GetMaxV() const;
    unsigned long GetMaxW() const;

   protected:
      /* Gets file pathes. */
    const String& GetFilePath() const;
    String GetFullFilePath() const;
    const String& GetTempDir() const;

    String GetContainerTempFilePath() const;

    /* Get current file names from XML or defaults. Empty if not set. */
    String GetMainArchiveName() const;
      String GetMainFileName() const;
      String GetPointDataArchiveName() const;
      String GetPointDataFileName();
      String GetValidPointsArchiveName() const;
      String GetValidPointsFileName();
      String GetChecksumArchiveName() const;
      String GetChecksumFileName() const;

    /* Decompress/pack the zip container. */
      OGPS_Boolean Decompress();
    OGPS_Boolean Decompress(const String& src, const String& dst) const;
    OGPS_Boolean DecompressMain() const;
    OGPS_Boolean DecompressDataBin();
    OGPS_Boolean Compress();

    /* Returns file pathes found within the container. */
    //typedef std::vector<String> FilePathList;
    //const FilePathList& GetFileList() const;
    
    /* Adds a file to the container. */
    void AddFile(const String& filePath);

    OGPS_Boolean CreateDocument(
       const xsd::Record1Type* record1,
       const xsd::Record2Type* record2,
       const xsd::MatrixDimensionType* matrix,
       const OGPS_Boolean useBinaryData);

    OGPS_Boolean ReadDocument();

    /* What do we parse: xml or binary? */
    virtual PointVectorParserBuilder* CreatePointVectorParserBuilder() const;
    /* Set the parser's properties; e.g. every axis data type. */
    virtual OGPS_Boolean CreatePointVectorParser(PointVectorParserBuilder& builder) const;

    OGPS_DataPointType GetXaxisDataType() const;
    OGPS_DataPointType GetYaxisDataType() const;
    OGPS_DataPointType GetZaxisDataType() const;
    OGPS_DataPointType GetAxisDataType(const xsd::AxisDescriptionType& axis, const OGPS_Boolean incremental) const;

    /* Buffer builder/setup */
    virtual VectorBufferBuilder* CreateVectorBufferBuilder() const;
    virtual OGPS_Boolean CreateVectorBuffer(VectorBufferBuilder& builder) const;

    OGPS_Boolean CreatePointBuffer();
    OGPS_Boolean SavePointBuffer(zipFile handle);
    void ResetXmlPointList();
    OGPS_Boolean SaveValidPointsLink(zipFile handle);

    virtual PointVectorReaderContext* CreatePointVectorReaderContext();
    virtual PointVectorWriterContext* CreatePointVectorWriterContext(zipFile handle) const;

    unsigned long GetPointCount() const;

    void Reset();

    OGPS_Boolean HasDocument() const;

      OGPS_Boolean HasVectorBuffer() const;

      OGPS_Boolean ReadXmlDocument();
      OGPS_Boolean SaveXmlDocument(zipFile handle);


    private:
       String m_FilePath;
       String m_TempPath;
       String m_TempBasePath;
       OGPS_Boolean m_IsReadOnly;
       String m_PointDataFileName;
       String m_ValidPointsFileName;
       
       ISO5436_2TypeAutoPtr m_Document;
       VectorBufferBuilderAutoPtr m_VectorBufferBuilder;

       PointVectorProxyContext m_ProxyContext;
       PointVectorAutoPtr m_PointVector;

       OGPS_Boolean CreateTempDir();
       OGPS_Boolean HasTempDir() const;

       OGPS_Boolean RemoveTempDir();

       VectorBuffer* const GetVectorBuffer() const;
    };
}

#endif /* _OPENGPS_ISO5436_2_CONTAINER_HXX */
