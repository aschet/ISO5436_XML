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

#ifndef _OPENGPS_CXX_ISO5436_2_HXX
#  include <opengps/cxx/iso5436_2.hxx>
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

#include <opengps/cxx/iso5436_2_xsd.hxx>

#include <zip.h>

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
         const OpenGPS::String& file,
         const OpenGPS::String& temp);

      virtual ~ISO5436_2Container();

      /* Open. */
      virtual OGPS_Boolean Open(const OGPS_Boolean readOnly = TRUE);

      /* Create matrix. */
      virtual OGPS_Boolean Create(
         const xsd::Record1Type& record1,
         const xsd::Record2Type& record2,
         const xsd::MatrixDimensionType& matrixDimension,
         const OGPS_Boolean useBinaryData = TRUE);

      /* Create list. */
      virtual OGPS_Boolean Create(
         const xsd::Record1Type& record1,
         const xsd::Record2Type& record2,
         const unsigned long listDimension,
         const OGPS_Boolean useBinaryData = TRUE);

      virtual PointIteratorAutoPtr CreateNextPointIterator();
      virtual PointIteratorAutoPtr CreatePrevPointIterator();

      virtual OGPS_Boolean SetMatrixPoint(
         const unsigned long u,
         const unsigned long v,
         const unsigned long w,
         const PointVector* const vector);

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
         OGPS_Double* const x,
         OGPS_Double* const y,
         OGPS_Double* const z);

      virtual OGPS_Boolean IsMatrixCoordValid(
         unsigned long u,
         unsigned long v,
         unsigned long w);

      virtual OGPS_Boolean GetListCoord(
         const unsigned long index,
         OGPS_Double* const x,
         OGPS_Double* const y,
         OGPS_Double* const z);

      virtual const ISO5436_2TypeAutoPtr& GetDocument();

      virtual OGPS_Boolean Write(const int compressionLevel = -1);

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
      const OpenGPS::String& GetFilePath() const;
      OpenGPS::String GetFullFilePath() const;
      const OpenGPS::String& GetTempDir() const;

      OpenGPS::String GetContainerTempFilePath() const;

      /* Get current file names from XML or defaults. Empty if not set. */
      OpenGPS::String GetMainArchiveName() const;
      OpenGPS::String GetMainFileName() const;
      OpenGPS::String GetPointDataArchiveName() const;
      OpenGPS::String GetPointDataFileName();
      OpenGPS::String GetValidPointsArchiveName() const;
      OpenGPS::String GetValidPointsFileName();
      OpenGPS::String GetChecksumArchiveName() const;
      OpenGPS::String GetChecksumFileName() const;

      /* Decompress/pack the zip container. */
      OGPS_Boolean Decompress();
      OGPS_Boolean Decompress(const OpenGPS::String& src, const OpenGPS::String& dst) const;
      OGPS_Boolean DecompressMain() const;
      OGPS_Boolean DecompressChecksum() const;
      OGPS_Boolean DecompressDataBin();
      OGPS_Boolean Compress();

      OGPS_Boolean CreateDocument(
         const xsd::Record1Type* const record1,
         const xsd::Record2Type* const record2,
         const xsd::MatrixDimensionType* const matrixDimension,
         const unsigned long listDimension,
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
      OGPS_Boolean SaveChecksumFile(zipFile handle, const OpenGPS::UnsignedByte checksum[16]);

      virtual PointVectorReaderContext* CreatePointVectorReaderContext();
      virtual PointVectorWriterContext* CreatePointVectorWriterContext(zipFile handle) const;

      unsigned long GetPointCount() const;

      void Reset();

      OGPS_Boolean HasDocument() const;

      OGPS_Boolean HasVectorBuffer() const;

      OGPS_Boolean ReadXmlDocument();
      OGPS_Boolean SaveXmlDocument(zipFile handle);


   private:
      OpenGPS::String m_FilePath;
      OpenGPS::String m_TempPath;
      OpenGPS::String m_TempBasePath;
      OGPS_Boolean m_IsReadOnly;
      OpenGPS::String m_PointDataFileName;
      OpenGPS::String m_ValidPointsFileName;
      int m_CompressionLevel;

      ISO5436_2TypeAutoPtr m_Document;
      VectorBufferBuilderAutoPtr m_VectorBufferBuilder;

      PointVectorProxyContext m_ProxyContext;
      PointVectorAutoPtr m_PointVector;

      OGPS_Boolean CreateTempDir();
      OGPS_Boolean HasTempDir() const;

      OGPS_Boolean RemoveTempDir();

      VectorBuffer* GetVectorBuffer();

      OGPS_Boolean ConfigureNamespaceMap(xml_schema::namespace_infomap& map) const;
      OGPS_Boolean ConfigureNamespaceMap(xml_schema::properties& props) const;

      OGPS_Boolean VerifyChecksum(const OpenGPS::String& filePath, const OpenGPS::UnsignedBytePtr checksum, const size_t size) const;
      OGPS_Boolean VerifyMainChecksum() const;
      OGPS_Boolean VerifyDataBinChecksum();
      OGPS_Boolean VerifyValidBinChecksum();

      OGPS_Boolean ReadMd5FromFile(const OpenGPS::String& fileName, OpenGPS::UnsignedByte checksum[16]) const;

      OGPS_Boolean ConvertPointToCoord(
         const  PointVector& vector,
         OGPS_Double* const x,
         OGPS_Double* const y,
         OGPS_Double* const z);

      OGPS_Boolean IsIncrementalX() const;
      OGPS_Boolean IsIncrementalY() const;

      double GetIncrementX() const;
      double GetIncrementY() const;

      double GetOffsetX() const;
      double GetOffsetY() const;
      double GetOffsetZ() const;

      unsigned long GetMatrixColumnCount() const;
      unsigned long GetMatrixIndex(const unsigned long u,
         const unsigned long v,
         const unsigned long w) const;
   };
}

#endif /* _OPENGPS_ISO5436_2_CONTAINER_HXX */
