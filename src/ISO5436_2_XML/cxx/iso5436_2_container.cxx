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

#include "iso5436_2_container.hxx"

#ifndef _OPENGPS_POINT_ITERATOR_IMPL_HXX
#  include "point_iterator_impl.hxx"
#endif

#include <opengps/iso5436_2_xsd.hxx>
#include <opengps/point_vector.hxx>

#include "point_vector_parser_builder.hxx"
#include "point_vector_parser.hxx"

#include "xml_point_vector_reader_context.hxx"
#include "xml_point_vector_writer_context.hxx"

#include "binary_lsb_point_vector_reader_context.hxx"
#include "binary_msb_point_vector_reader_context.hxx"

#include "binary_lsb_point_vector_writer_context.hxx"
#include "binary_msb_point_vector_writer_context.hxx"

#include "vector_buffer_builder.hxx"
#include "vector_buffer.hxx"

#include "point_vector_proxy_context.hxx"

#include "environment.hxx"

#include "point_vector_iostream.hxx"

#include "zip_stream_buffer.hxx"

#include <iostream>
#include <fstream>

/* zlib/minizip header files */
#include <unzip.h>
#include <zip.h>

#define MAX_ZIP_CHUNK (256*1024)

#include "stdafx.hxx"

ISO5436_2Container::ISO5436_2Container(
                                       const String& file,
                                       const String& temp)
                                       : ISO5436_2()
{
   m_FilePath = file;
   m_IsReadOnly = FALSE;
   m_TempBasePath = temp;
}

ISO5436_2Container::~ISO5436_2Container()
{
   Environment::Reset();
}

OGPS_Boolean ISO5436_2Container::Open(const OGPS_Boolean readOnly)
{
   if(!HasDocument())
   {
      m_IsReadOnly = readOnly;

      if(!Decompress())
      {
         Reset();
         return FALSE;
      }

      return TRUE;
   }

   return FALSE;
}

OGPS_Boolean ISO5436_2Container::Decompress()
{
   if(CreateTempDir())
   {
   OGPS_Boolean unpacked = (DecompressMain() && ReadDocument() && DecompressDataBin() && CreatePointBuffer());

      if(RemoveTempDir())
      {
         return unpacked;
      }
   }
      return FALSE;
}

OGPS_Boolean ISO5436_2Container::Create(
                                   const xsd::Record1Type& record1,
                                   const xsd::Record2Type& record2,
                                   const xsd::MatrixDimensionType& matrix,
                                   const OGPS_Boolean useBinaryData)
{
   if(!HasDocument())
   {
      return CreateDocument(&record1, &record2, &matrix, useBinaryData);
   }
   return FALSE;
}

OGPS_Boolean ISO5436_2Container::Create(
                                   const xsd::Record1Type& record1,
                                   const xsd::Record2Type& record2,
                                   const OGPS_Boolean useBinaryData)
{
   if(!HasDocument())
   {
      return CreateDocument(&record1, &record2, NULL, useBinaryData);
   }
   return FALSE;
}

PointIteratorAutoPtr ISO5436_2Container::CreateNextPointIterator()
{
   return PointIteratorAutoPtr(new PointIteratorImpl(this, TRUE, IsMatrix()));
}

PointIteratorAutoPtr ISO5436_2Container::CreatePrevPointIterator()
{
   return PointIteratorAutoPtr(new PointIteratorImpl(this, FALSE, IsMatrix()));
}

OGPS_Boolean ISO5436_2Container::SetMatrixPoint(
   const unsigned long u,
   const unsigned long v,
   const unsigned long w,
   const PointVector* vector)
{
   _ASSERT(m_PointVector.get());

   m_ProxyContext.SetU(u);
   m_ProxyContext.SetV(v);
   m_ProxyContext.SetW(w);

   if(vector)
   {
      return m_PointVector->Set(*vector);
   }

   // TODO: Invalid point

   return FALSE;
}

OGPS_Boolean ISO5436_2Container::GetMatrixPoint(
   const unsigned long u,
   const unsigned long v,
   const unsigned long w,
   PointVector& vector)
{
   _ASSERT(m_PointVector.get());

   m_ProxyContext.SetU(u);
   m_ProxyContext.SetV(v);
   m_ProxyContext.SetW(w);

   return m_PointVector->Get(vector);
}

OGPS_Boolean ISO5436_2Container::SetListPoint(
   const unsigned long index,
   const PointVector& vector)
{
   _ASSERT(m_PointVector.get());

   m_ProxyContext.SetU(index);

   return m_PointVector->Set(vector);
}

OGPS_Boolean ISO5436_2Container::GetListPoint(
   const unsigned long index,
   PointVector& vector)
{
   _ASSERT(m_PointVector.get());

   m_ProxyContext.SetU(index);

   return m_PointVector->Get(vector);
}

OGPS_Boolean ISO5436_2Container::GetMatrixCoord(
   const unsigned long u,
   const unsigned long v,
   const unsigned long w,
   double* x,
   double* y,
   double* z)
{
   // TODO
   return FALSE;
}

OGPS_Boolean ISO5436_2Container::IsMatrixCoordValid(
   unsigned long u,
   unsigned long v,
   unsigned long w)
{
   // TODO
   return TRUE;
}

OGPS_Boolean ISO5436_2Container::GetListCoord(
   const unsigned long index,
   double* x,
   double* y,
   double* z)
{
   // TODO
   return FALSE;
}

const ISO5436_2TypeAutoPtr& ISO5436_2Container::GetDocument()
{
   return m_Document;
}

OGPS_Boolean ISO5436_2Container::Write()
{
   return Compress();
}

OGPS_Boolean ISO5436_2Container::Close()
{
   Reset();
   RemoveTempDir();

   return TRUE;
}

ISO5436_2Container& ISO5436_2Container::operator=(const ISO5436_2Container& src)
{
   // TODO
   return *this;
}

OGPS_Boolean ISO5436_2Container::IsMatrix() const
{
   _ASSERT(HasDocument());

   // is matrix present?
   if(m_Document->Record3().MatrixDimension().present())
   {
      // ensure there's either matrix or list
      _ASSERT(!m_Document->Record3().ListDimension().present());

      return TRUE;
   }

   // the point list should be present instead
   _ASSERT(m_Document->Record3().ListDimension().present());

   return FALSE;
}

String ISO5436_2Container::GetContainerTempFilePath() const
{
   const Environment* env = Environment::GetInstance();

   return env->ConcatPathes(GetTempDir(), env->GetUniqueName());
}

String ISO5436_2Container::GetFullFilePath() const
{
   // TODO
   return m_FilePath;
}

const String& ISO5436_2Container::GetFilePath() const
{
   return m_FilePath;
}

const String& ISO5436_2Container::GetTempDir() const
{
   _ASSERT(m_TempPath.length() > 0);

   return m_TempPath;
}

String ISO5436_2Container::GetMainArchiveName() const
{
   return _T("main.xml");
}

String ISO5436_2Container::GetMainFileName() const
{
   return Environment::GetInstance()->ConcatPathes(GetTempDir(), GetMainArchiveName());
}

String ISO5436_2Container::GetPointDataArchiveName() const
{
   _ASSERT(HasDocument() && IsBinary());

   return m_Document->Record3().DataLink()->PointDataLink();
}

String ISO5436_2Container::GetPointDataFileName()
{
   if(m_PointDataFileName.empty())
   {
      const Environment* env = Environment::GetInstance();
    m_PointDataFileName = env->ConcatPathes(GetTempDir(), env->GetUniqueName());
   }

   return m_PointDataFileName;
}

String ISO5436_2Container::GetValidPointsArchiveName() const
{
   _ASSERT(HasDocument() && IsBinary() && HasValidPointsLink());
   
   return m_Document->Record3().DataLink()->ValidPointsLink().get();
}

String ISO5436_2Container::GetValidPointsFileName()
{
   if(m_ValidPointsFileName.empty())
   {
      const Environment* env = Environment::GetInstance();
    m_ValidPointsFileName = env->ConcatPathes(GetTempDir(), env->GetUniqueName());
   }

   return m_ValidPointsFileName;
}

String ISO5436_2Container::GetChecksumArchiveName() const
{
   _ASSERT(HasDocument());

   return m_Document->Record4().ChecksumFile();
}

String ISO5436_2Container::GetChecksumFileName() const
{
   return Environment::GetInstance()->ConcatPathes(GetTempDir(), GetChecksumArchiveName());
}

OGPS_Boolean ISO5436_2Container::DecompressMain() const
{
   const String src = GetMainArchiveName();
   const String dst = GetMainFileName();

   if(!Decompress(src, dst))
   {
      if(Environment::GetInstance()->PathExists(dst))
      {
         _VERIFY(Environment::GetInstance()->RemoveFile(dst));
      }

      return FALSE;
   }

   return TRUE;
}

OGPS_Boolean ISO5436_2Container::DecompressDataBin()
{
   if(IsBinary())
   {
      if(Decompress(GetPointDataArchiveName(), GetPointDataFileName()))
      {
         if(HasValidPointsLink())
         {
            if(Decompress(GetValidPointsArchiveName(), GetValidPointsFileName()))
            {
               return TRUE;
            }

            return FALSE;
         }

         return TRUE;
      }

      return FALSE;
   }

   return TRUE;
}

OGPS_Boolean ISO5436_2Container::Decompress(const OpenGPS::String& src, const OpenGPS::String& dst) const
{
   OGPS_Boolean retval = FALSE;

   // Gets absolute path to X3P file.
   String filePath = GetFullFilePath();

      // Gets handler to zip archive or NULL.
      unzFile handle = unzOpen(filePath.ToChar());
      
      if(handle)
      {
         // Locate the central xml document in the archive.
         // The X3P format definition says it must be named main.xml,
         // but make a case-insensitive search - merly for
         // compatibility reasons on different architectures...
         // Set this as the current file.
         String srcbuf(src);
         if(unzLocateFile(handle, srcbuf.ToChar(), 2 /* case insensitive */) == UNZ_OK)
         {
            // Open the current file for reading.
            if(unzOpenCurrentFile(handle) == UNZ_OK)
            {
               // Need information about file size
               unz_file_info fileInfo;
               if(unzGetCurrentFileInfo(handle, &fileInfo, NULL, 0, NULL, 0, NULL, 0) == UNZ_OK)
               {
                  // Open binary target stream for uncompressed data
               PointVectorOutputBinaryFileStream binaryTarget(dst);
               
               if(!binaryTarget.fail())
               {
               unsigned long written = 0;
               unsigned long length = fileInfo.uncompressed_size;

               // Don't uncompress this file as a whole, but in loops
               // of a predefined maximum chunk size. Otherwise we
               // might get out of memory...
               while(written < length)
               {
                  int size = min(length - written, MAX_ZIP_CHUNK);                  

                  _ASSERT(size > 0);

                  voidp buffer = (voidp)malloc(size);
               
                  if(!buffer)
                  {
                     written = 0;
                     break;
                  }

                     int bytesCopied = unzReadCurrentFile(handle, buffer, size);

                     if(bytesCopied == size)
                     {
                     binaryTarget.write((OpenGPS::UnsignedBytePtr)buffer, bytesCopied);
                     }

                     free(buffer);

                     if(bytesCopied != size || binaryTarget.fail())
                     {
                        written = 0;
                        break;
                     }

                     written += size;
               }

               retval = (written == length);
               }

               binaryTarget.close();

               retval = retval && !binaryTarget.fail();
               }

               // Close the current file. Gives an error
               // when CRC cecksum did not validate.
               if(unzCloseCurrentFile(handle) == UNZ_OK)
               {
                  retval = retval && TRUE;
               }
               }
         }
         // Closes the file handler.
         if(unzClose(handle) == UNZ_OK)
         {
            retval = retval && TRUE;
         }
         }
        

   return retval;
}

OGPS_Boolean ISO5436_2Container::Compress()
{
   OGPS_Boolean retval = FALSE;

   if(CreateTempDir())
   {

   String targetZip = GetContainerTempFilePath();
   zipFile handle = zipOpen(targetZip.ToChar(), APPEND_STATUS_CREATE);

   if(handle)
   {
      retval = SavePointBuffer(handle) && SaveXmlDocument(handle);

      if(HasValidPointsLink())
      {
         retval = retval && SaveValidPointsLink(handle);
      }

      if(zipClose(handle, NULL) == ZIP_OK)
      {
         retval = retval && TRUE;
      }

      if(retval)
      {
         retval = Environment::GetInstance()->RenameFile(targetZip, GetFullFilePath());
      }
   }

   if(RemoveTempDir())
   {
      retval = retval && TRUE;
   }

   }

   return retval;
   }

   

/*
const FilePathList& ISO5436_2Container::GetFileList() const
{
}
*/
void ISO5436_2Container::AddFile(const String& filePath)
{
}

OGPS_Boolean ISO5436_2Container::CreateDocument(
   const xsd::Record1Type* record1,
   const xsd::Record2Type* record2,
   const xsd::MatrixDimensionType* matrix,
   const OGPS_Boolean useBinaryData)
{
   _ASSERT(!HasDocument());
/*
   if(record2)
   {
      xsd::Record3Type record3;
      record3.MatrixDimension(*matrix);
      if(useBinaryData)
      {
         DataLinkType dataLink(_T("bindata/data.bin"), _T(""));
         record3.DataLink(dataLink);
      }
      xsd::Record4Type record4(
         ISO5436_2TypeAutoPtr d = new xsd::ISO5436_2Type(*record1, record3, record4);
   d->Record2(*record2);
   */

   // TODO:

   return FALSE;
}

OGPS_Boolean ISO5436_2Container::ReadDocument()
{
   if(!HasDocument())
   {
      if(ReadXmlDocument())
      {
         _ASSERT(HasDocument());

                      return TRUE;
            }
   }

   // soemthing went terribly wrong,
   // so reset internal state
   Reset();

   return FALSE;
}

OGPS_Boolean ISO5436_2Container::ReadXmlDocument()
{
   OGPS_Boolean success = TRUE;

   String xmlFilePath = GetMainFileName();

   // TODO: throws exception when file not found, wrong file type, xsd not found
   try
   {
      m_Document = xsd::ISO5436_2(xmlFilePath);
   }
   catch(const xml_schema::exception& ex)
   {
      // TODO: bessere error behandlung
      std::wstringstream ws;
      ws << ex;
      String s = ws.str();

      std::cerr << s.ToChar() << std::endl;

      success = FALSE;
   }

   return success;
}

OGPS_Boolean ISO5436_2Container::CreatePointBuffer()
{
   _ASSERT(!HasVectorBuffer());

   // Build and setup internal point buffer
   VectorBufferBuilderAutoPtr v_builder(CreateVectorBufferBuilder());
   if(v_builder.get())
   {
      if(CreateVectorBuffer(*v_builder.get()))
      {
            m_VectorBufferBuilder = v_builder;
      }
   }

      if(HasDocument() && HasVectorBuffer())
      {
         VectorBuffer* const vectorBuffer = GetVectorBuffer();

                  // read valid points file
                  if(HasValidPointsLink())
                  {
                     PointVectorInputBinaryFileStream vstream(GetValidPointsFileName());
                     if(!vectorBuffer->GetValid()->Read(vstream, GetPointCount()))
                     {
                        return FALSE;
                     }
                  }

         // Create point parser for this document
         PointVectorParserBuilderAutoPtr p_builder(CreatePointVectorParserBuilder());
         if(p_builder.get())
         {
            if(CreatePointVectorParser(*p_builder.get()))
            {
               PointVectorParser* parser = p_builder->GetParser();
               PointVectorReaderContext* context = CreatePointVectorReaderContext();

               if(context)
               {
                  OGPS_Boolean success = TRUE;

                  PointVectorProxyContext proxy_context;
                  PointVectorAutoPtr vector(vectorBuffer->GetPointVectorProxy(proxy_context));

                  unsigned long index = 0;

                  while(context->MoveNext())
                  {
                     if(context->IsValid())
                     {
                        if(!parser->Read(*context, *vector))
                        {
                           success = FALSE;
                           break;
                        }
                     }
                     else
                     {
                        if(!vectorBuffer->GetValid()->IsAllocated())
                        {
                           if(!vectorBuffer->GetValid()->Allocate(GetPointCount()))
                              {
                           success = FALSE;
                           break;
                           }
                        }

                        if(!vector->SetNull() || !vectorBuffer->GetValid()->SetValid(index, FALSE))
                        {
                           success = FALSE;
                           break;
                        }
                     }

                     ++index;

                     proxy_context.SetU(index);
                     proxy_context.SetV(index);
                     proxy_context.SetW(index);

                  }
/*
                  PointVectorOutputBinaryFileStream test(_T("e:\\temp\\valid.bin"));
                  vectorBuffer->GetValid()->Write(test);
                  test.close();
*/
                  // TODO: besser auto_ptr
                  delete context;

                  // When the point buffer has been created,
                  // we can savely drop the original xml content
                  ResetXmlPointList();

                  // initialize global vector proxy
                  m_PointVector = GetVectorBuffer()->GetPointVectorProxy(m_ProxyContext);

                  return success;
               }
            }
         }
      }

   return FALSE;
}

void ISO5436_2Container::ResetXmlPointList()
{
   if(m_Document->Record3().DataList().present())
                  {
                     m_Document->Record3().DataList()->Datum().clear();
                  }
}

OGPS_Boolean ISO5436_2Container::SaveXmlDocument(zipFile handle)
{
   _ASSERT(handle);

   OGPS_Boolean retval = FALSE;

   // Creates new file in the zip container.
   String mainDocument(GetMainArchiveName());
   if(zipOpenNewFileInZip(handle,
                       mainDocument.ToChar(),
                       NULL,
                       NULL,
                       0,
                       NULL,
                       0,
                       NULL,
                       Z_DEFLATED,
                       Z_BEST_COMPRESSION) == ZIP_OK)
   {
      ZipStreamBuffer buffer(handle);
      ZipOutputStream zipOut(buffer);

      
      if (!zipOut.fail ())
      {
         xml_schema::namespace_infomap map;
         map[_T("")].name = _T("http://www.opengps.eu/xsd/");
         map[_T("")].schema = _T("../ISO5436_2.xsd"); // TODO: Defaultnamespace?

         // TODO: throws exception when file not found, wrong file type, xsd not found
         try
         {
            xsd::ISO5436_2(zipOut, *m_Document, map);
            retval = TRUE;
         }
         catch(const xml_schema::exception& ex)
         {
            // TODO: bessere error behandlung
            std::wstringstream ws;
            ws << ex;
            String s = ws.str();

            std::cerr << s.ToChar() << std::endl;
         }
      }

      if(zipCloseFileInZip(handle) == ZIP_OK)
      {
         retval = retval && TRUE;
      }
   }

   return retval;
}

OGPS_Boolean ISO5436_2Container::SaveValidPointsLink(zipFile handle)
{
   _ASSERT(HasValidPointsLink());

   OGPS_Boolean retval = FALSE;

   // Creates new file in the zip container.
   String section(GetValidPointsArchiveName());
   if(zipOpenNewFileInZip(handle,
                       section.ToChar(),
                       NULL,
                       NULL,
                       0,
                       NULL,
                       0,
                       NULL,
                       Z_DEFLATED,
                       Z_BEST_COMPRESSION) == ZIP_OK)
   {
   VectorBuffer* const vectorBuffer = GetVectorBuffer();

            ZipStreamBuffer vbuffer(handle);
            ZipOutputStream vstream(vbuffer);

            if(!vstream.fail())
            {
            retval = vectorBuffer->GetValid()->Write(vstream);
            }

            if(zipCloseFileInZip(handle) == ZIP_OK)
      {
         retval = retval && TRUE;
      }
   }

   return retval;
}

OGPS_Boolean ISO5436_2Container::SavePointBuffer(zipFile handle)
{
   _ASSERT(handle);

   OGPS_Boolean retval = FALSE;

   // Creates new file in the zip container.
   String section(GetPointDataArchiveName());
   if(zipOpenNewFileInZip(handle,
                       section.ToChar(),
                       NULL,
                       NULL,
                       0,
                       NULL,
                       0,
                       NULL,
                       Z_DEFLATED,
                       Z_BEST_COMPRESSION) == ZIP_OK)
   {
   // Before we start: reset changes made to the
   // point list xml tag. Points will be replaced
   // with those vlaues in the current vector buffer.
   ResetXmlPointList();

      if(HasDocument() && HasVectorBuffer())
      {
         // Create point parser for this document
         PointVectorParserBuilderAutoPtr p_builder(CreatePointVectorParserBuilder());
         if(p_builder.get())
         {
            if(CreatePointVectorParser(*p_builder.get()))
            {
               PointVectorParser* parser = p_builder->GetParser();
               PointVectorWriterContext* context = CreatePointVectorWriterContext(handle);

               if(context)
               {
                  OGPS_Boolean success = TRUE;

                  VectorBuffer* const vectorBuffer = GetVectorBuffer();

                  PointVectorProxyContext proxy_context;
                  PointVectorAutoPtr vector(vectorBuffer->GetPointVectorProxy(proxy_context));

                  unsigned long index = 0;
                  const unsigned long count = GetPointCount(); // TODO: besser irgendetwas mit buffer->Size - oder doch so ausreichend?

                  // TODO: besser "Context" verwenden
                  OGPS_Boolean isBinary = IsBinary();

                  while(index < count)
                  {
                     if(isBinary || vectorBuffer->GetValid()->IsValid(index))
                     {
                        if(!parser->Write(*context, *vector))
                        {
                           success = FALSE;
                           break;
                        }
                        }

                        if(!context->MoveNext())
                        {
                           if(index + 1 != count)
                           {
                              success = FALSE;
                              break;
                           }
                        }                     

                     ++index;

                     proxy_context.SetU(index);
                     proxy_context.SetV(index);
                     proxy_context.SetW(index);
                  }

                  // TODO: besser auto_ptr
                  delete context;

                  retval = success;
               }
            }
         }
      }

      if(zipCloseFileInZip(handle) == ZIP_OK)
      {
         retval = retval && TRUE;
      }
   }

   return retval;
}

PointVectorParserBuilder* ISO5436_2Container::CreatePointVectorParserBuilder() const
{
   return new PointVectorParserBuilder();
}

VectorBufferBuilder* ISO5436_2Container::CreateVectorBufferBuilder() const
{
   return new VectorBufferBuilder();
}

OGPS_Boolean ISO5436_2Container::CreatePointVectorParser(PointVectorParserBuilder& builder) const
{
   OGPS_DataPointType xType = GetXaxisDataType();
   OGPS_DataPointType yType = GetYaxisDataType();
   OGPS_DataPointType zType = GetZaxisDataType();

   return (builder.BuildParser() &&      
      builder.BuildX(xType) &&
      builder.BuildY(yType) &&
      builder.BuildZ(zType));
}

OGPS_Boolean ISO5436_2Container::CreateVectorBuffer(VectorBufferBuilder& builder) const
{
   _ASSERT(HasDocument());

   OGPS_DataPointType xType = GetXaxisDataType();
   OGPS_DataPointType yType = GetYaxisDataType();
   OGPS_DataPointType zType = GetZaxisDataType();

   const unsigned long size = GetPointCount();

   return (builder.BuildBuffer() &&
      builder.BuildX(xType, size) &&
      builder.BuildY(yType, size) &&
      builder.BuildZ(zType, size) &&
      builder.BuildValid(HasValidPointsLink() && !IsBinary() ? size : 0));
}

PointVectorReaderContext* ISO5436_2Container::CreatePointVectorReaderContext()
{
   // instantiate binary reader context
   if(IsBinary())
   {
      // path to the source binary file
      const String binaryFilePath = GetPointDataFileName();
      if(binaryFilePath.length() > 0)
      {
         // find out if we are on lsb or msb
         // hardware and create appropiate context
         if(Environment::GetInstance()->IsLittleEndian())
         {
            return new BinaryLSBPointVectorReaderContext(binaryFilePath);
         }

         return new BinaryMSBPointVectorReaderContext(binaryFilePath);
      }

      return NULL;
   }

   // instantiate xml String reader context...
   // find list tag in xml document and check whether it is present
   const xsd::Record3Type::DataList_optional& dataList = m_Document->Record3().DataList();
   if(dataList.present())
   {
      const xsd::DataListType::Datum_sequence& datum = dataList->Datum();
      return new XmlPointVectorReaderContext(&datum);
   }

   return NULL;
}

PointVectorWriterContext* ISO5436_2Container::CreatePointVectorWriterContext(zipFile handle) const
{
   _ASSERT(handle);

   // instantiate binary writer context
   if(IsBinary())
   {
         // find out if we are on lsb or msb
         // hardware and create appropiate context
         if(Environment::GetInstance()->IsLittleEndian())
         {
            return new BinaryLSBPointVectorWriterContext(handle);
         }

         return new BinaryMSBPointVectorWriterContext(handle);
   }

   // instantiate xml String reader context...
   // find list tag in xml document and check whether it is present
   xsd::Record3Type::DataList_optional& dataList = m_Document->Record3().DataList();
   if(dataList.present())
   {
      xsd::DataListType::Datum_sequence& datum = dataList->Datum();
      
      // should be empty at this point (and filled by the writer)
      if(datum.size() == 0)
      {
         return new XmlPointVectorWriterContext(&datum);
      }
   }

   return NULL;
}

OGPS_DataPointType ISO5436_2Container::GetXaxisDataType() const
{
   _ASSERT(HasDocument());

   return GetAxisDataType(m_Document->Record1().Axes().CX(), TRUE);
}

OGPS_DataPointType ISO5436_2Container::GetYaxisDataType() const
{
   _ASSERT(HasDocument());

   return GetAxisDataType(m_Document->Record1().Axes().CY(), TRUE);
}

OGPS_DataPointType ISO5436_2Container::GetZaxisDataType() const
{
   _ASSERT(HasDocument());

   return GetAxisDataType(m_Document->Record1().Axes().CZ(), FALSE);
}

OGPS_DataPointType ISO5436_2Container::GetAxisDataType(const xsd::AxisDescriptionType& axis, const OGPS_Boolean incremental) const
{
   // TODO: für sowas einen builder bauen?

   if(axis.DataType().present())
   {
      if(!incremental || axis.AxisType() != xsd::AxisType::I)
      {
         switch(axis.DataType().get())
         {
         case xsd::DataType::I:
            return Int16PointType;
         case xsd::DataType::L:
            return Int32PointType;
         case xsd::DataType::F:
            return FloatPointType;
         case xsd::DataType::D:
            return DoublePointType;
         default:
            _ASSERT(FALSE);
            break;
         }
      }
   }

   return MissingPointType;
}

unsigned long ISO5436_2Container::GetPointCount() const
{
   _ASSERT(HasDocument());

   // calculate point count of matrix type
   if(IsMatrix())
   {
      unsigned long xSize = m_Document->Record3().MatrixDimension()->SizeX();
      unsigned long ySize = m_Document->Record3().MatrixDimension()->SizeY();
      unsigned long zSize = m_Document->Record3().MatrixDimension()->SizeZ();

      // BUG: buffer overflow
      return (xSize * ySize * zSize);
   }
   
   // calculate point count of list type
   return m_Document->Record3().ListDimension().get();
}

unsigned long ISO5436_2Container::GetMaxU() const
{
   _ASSERT(HasDocument());

   if(IsMatrix())
   {
      return m_Document->Record3().MatrixDimension()->SizeX();
   }

   return m_Document->Record3().ListDimension().get();
}

unsigned long ISO5436_2Container::GetMaxV() const
{
   _ASSERT(HasDocument());

   if(IsMatrix())
   {
      return m_Document->Record3().MatrixDimension()->SizeY();
   }

   return m_Document->Record3().ListDimension().get();
}

unsigned long ISO5436_2Container::GetMaxW() const
{
   _ASSERT(HasDocument());

   if(IsMatrix())
   {
      return m_Document->Record3().MatrixDimension()->SizeZ();
   }

   return m_Document->Record3().ListDimension().get();
}

OGPS_Boolean ISO5436_2Container::IsBinary() const
{
   _ASSERT(HasDocument());

   const OGPS_Boolean hasDataLink = m_Document->Record3().DataLink().present();

   // ensure data ist available in binary or text format either
   _ASSERT((hasDataLink && !m_Document->Record3().DataList().present()) || (!hasDataLink && m_Document->Record3().DataList().present()));

   return hasDataLink;
}

OGPS_Boolean ISO5436_2Container::HasValidPointsLink() const
{
   _ASSERT(HasDocument());

   return (IsBinary() && m_Document->Record3().DataLink()->ValidPointsLink().present());
}

void ISO5436_2Container::Reset()
{
   m_Document.release();
   m_VectorBufferBuilder.release();
   m_PointVector.release();
   m_PointDataFileName.clear();
   m_ValidPointsFileName.clear();
}

OGPS_Boolean ISO5436_2Container::HasDocument() const
{
   return m_Document.get() != NULL;
}

OGPS_Boolean ISO5436_2Container::HasVectorBuffer() const
{
   OGPS_Boolean retval = m_VectorBufferBuilder.get() != NULL;
   _ASSERT(!retval || m_VectorBufferBuilder->GetBuffer());
   return retval;
}

OGPS_Boolean ISO5436_2Container::CreateTempDir()
{
   if(!HasTempDir())
   {
   const Environment* env = Environment::GetInstance();

   const String uniqueDirectory = env->GetUniqueName();

   String temp;
   OGPS_Boolean created = FALSE;
   if(m_TempBasePath.length() > 0 && env->PathExists(m_TempBasePath))
   {      
      temp = env->ConcatPathes(m_TempBasePath, uniqueDirectory);

      // TODO: try new random
      if(!env->PathExists(temp))
      {
         created = env->CreateDir(temp);
      }
   }

   if(!created)
   {
      const String sysTemp = env->GetTempDir();
      if(env->PathExists(sysTemp))
      {
         temp = env->ConcatPathes(sysTemp, uniqueDirectory);

         // TODO: try new random
         if(!env->PathExists(temp))
      {
         created = env->CreateDir(temp);
         }
      }
   }

   if(created)
   {
      m_TempPath = temp;
   }
   else
   {
      m_TempPath.clear();
   }

   return created;
   }

   return FALSE;
}

OGPS_Boolean ISO5436_2Container::HasTempDir() const
{
   return m_TempPath.length() > 0;
}

OGPS_Boolean ISO5436_2Container::RemoveTempDir()
{
   m_PointDataFileName.clear();
   m_ValidPointsFileName.clear();

   if(HasTempDir())
   {
      if(Environment::GetInstance()->RemoveDir(m_TempPath))
      {
         m_TempPath.clear();
         return TRUE;
      }

      return FALSE;
   }

   return TRUE;
}

VectorBuffer* const ISO5436_2Container::GetVectorBuffer() const
{
   _ASSERT(m_VectorBufferBuilder.get());
   return m_VectorBufferBuilder->GetBuffer();
}
