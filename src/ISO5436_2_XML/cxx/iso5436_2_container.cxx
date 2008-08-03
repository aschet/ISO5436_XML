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

#include <opengps/cxx/point_vector.hxx>
#include <opengps/cxx/data_point.hxx>

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

#include "point_vector_proxy_context_matrix.hxx"
#include "point_vector_proxy_context_list.hxx"

#include "environment.hxx"

#include "point_vector_iostream.hxx"

#include "zip_stream_buffer.hxx"

#include <limits>
#include <iostream>
#include <fstream>
#include <memory>
#include <iomanip>
#include <sstream>

/* zlib/minizip header files */
#include <unzip.h>
#include <zip.h>

/* md5 */
#include "../xyssl/md5.h"

#include "stdafx.hxx"

#define _OPENGPS_ZIP_CHUNK_MAX (256*1024)
#define _OPENGPS_FILE_URI_PREF _T("file:///")

#ifdef max
#  undef max
#endif

typedef std::auto_ptr<PointVectorReaderContext> PointVectorReaderContextAutoPtr;
typedef std::auto_ptr<PointVectorWriterContext> PointVectorWriterContextAutoPtr;

ISO5436_2Container::ISO5436_2Container(
                                       const OpenGPS::String& file,
                                       const OpenGPS::String& temp)
                                       : ISO5436_2()
{
   m_Document = NULL;
   m_FilePath = file;
   m_IsReadOnly = FALSE;
   m_TempBasePath = temp;
   m_CompressionLevel = Z_DEFAULT_COMPRESSION;

   m_MainChecksum = TRUE;
   m_DataBinChecksum = TRUE;
   m_ValidBinChecksum = TRUE;
   m_IsCreating = FALSE;
}

ISO5436_2Container::~ISO5436_2Container()
{
   _OPENGPS_DELETE(m_Document);

   Environment::Reset();
}

void ISO5436_2Container::Open(const OGPS_Boolean readOnly) throw(...)
{
   if(HasDocument())
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("The document already exists."),
         _EX_T("Close the existing document before you open another."),
         _EX_T("OpenGPS::ISO5436_2Container::Open"));
   }

   try
   {
      CreateTempDir();

      try
      {
         Decompress();
         CreatePointBuffer();         
      }
      catch(...)
      {
         Reset();
         RemoveTempDir();
         throw;
      }

      RemoveTempDir();
   }
   catch(...)
   {
      Reset();
      throw;
   }

   m_IsReadOnly = readOnly;
   m_IsCreating = FALSE;

   TestChecksums();
}

void ISO5436_2Container::Decompress() throw(...)
{
   _ASSERT(HasTempDir());
   
   DecompressMain();
   ReadDocument();
   DecompressChecksum();
   DecompressDataBin();
}

OGPS_Boolean ISO5436_2Container::VerifyChecksum(const OpenGPS::String& filePath, const OpenGPS::UnsignedBytePtr checksum, const size_t size) const
{
   _ASSERT(filePath.size() > 0);

   if(!checksum || size != 16)
   {
      return FALSE;
   }

   OpenGPS::String filePathBuffer(filePath);
   OpenGPS::UnsignedByte md5[16];

   if(!md5_file(filePathBuffer.ToChar(), md5))
   {
      for(int n = 0; n < 16; ++n)
      {
         if(checksum[n] != md5[n])
         {
            return FALSE;
         }
      }

      return TRUE;
   }

   return FALSE;
}

void ISO5436_2Container::VerifyMainChecksum()
{
   _ASSERT(HasDocument());

   const OpenGPS::String file(GetMainFileName());
   const OpenGPS::String chksFileName(GetChecksumFileName());

   OpenGPS::UnsignedByte checksum[16];
   if(ReadMd5FromFile(chksFileName, checksum))
   {
      m_MainChecksum = VerifyChecksum(file, &checksum[0], 16);

      return;
   }

   m_MainChecksum = FALSE;
}

void ISO5436_2Container::VerifyDataBinChecksum()
{
   _ASSERT(HasDocument() && IsBinary());

   const OpenGPS::String file(GetPointDataFileName());

   if(m_Document->Record3().DataLink().present())
   {
      const Schemas::ISO5436_2::DataLinkType::MD5ChecksumPointData_type& md5 = m_Document->Record3().DataLink()->MD5ChecksumPointData();
      m_DataBinChecksum = VerifyChecksum(file, (const OpenGPS::UnsignedBytePtr)md5.data(), md5.size());

      return;
   }

   m_DataBinChecksum = FALSE;
}

void ISO5436_2Container::VerifyValidBinChecksum()
{
   _ASSERT(HasDocument() && IsBinary() && HasValidPointsLink());

   const OpenGPS::String file(GetValidPointsFileName());

   if(m_Document->Record3().DataLink().present())
   {
      const Schemas::ISO5436_2::DataLinkType::MD5ChecksumValidPoints_optional& md5 = m_Document->Record3().DataLink()->MD5ChecksumValidPoints();
      if(md5.present())
      {
         m_ValidBinChecksum = VerifyChecksum(file, (const OpenGPS::UnsignedBytePtr)md5.get().data(), md5.get().size());

         return;
      }
   }

   m_ValidBinChecksum = FALSE;
}

OGPS_Boolean ISO5436_2Container::ReadMd5FromFile(const OpenGPS::String& fileName, OpenGPS::UnsignedByte checksum[16]) const
{
#ifdef _UNICODE
   std::wifstream file(fileName.c_str());
#else
   std::ifstream file(fileName.c_str());
#endif /* _UNICODE */

   OpenGPS::String md5;
   file >> md5;
   file.close();

   return md5.ConvertToMd5(checksum);
}

void ISO5436_2Container::Create(
                                        const Schemas::ISO5436_2::Record1Type& record1,
                                        const Schemas::ISO5436_2::Record2Type* record2,
                                        const Schemas::ISO5436_2::MatrixDimensionType& matrixDimension,
                                        const OGPS_Boolean useBinaryData) throw(...)
{
   if(HasDocument())
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("The document already exists."),
         _EX_T("Close the existing document before you create another."),
         _EX_T("OpenGPS::ISO5436_2Container::Create"));
   }

   CreateDocument(&record1, record2, &matrixDimension, 0, useBinaryData);
}

void ISO5436_2Container::Create(
                                        const Schemas::ISO5436_2::Record1Type& record1,
                                        const Schemas::ISO5436_2::Record2Type* record2,
                                        const unsigned long listDimension,
                                        const OGPS_Boolean useBinaryData) throw(...)
{
   if(HasDocument())
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("The document already exists."),
         _EX_T("Close the existing document before you create another."),
         _EX_T("OpenGPS::ISO5436_2Container::Create"));
   }

   CreateDocument(&record1, record2, NULL, listDimension, useBinaryData);
}

PointIteratorAutoPtr ISO5436_2Container::CreateNextPointIterator() throw(...)
{
   return PointIteratorAutoPtr(new PointIteratorImpl(this, TRUE, IsMatrix()));
}

PointIteratorAutoPtr ISO5436_2Container::CreatePrevPointIterator() throw(...)
{
   return PointIteratorAutoPtr(new PointIteratorImpl(this, FALSE, IsMatrix()));
}

void ISO5436_2Container::SetMatrixPoint(
   const unsigned long u,
   const unsigned long v,
   const unsigned long w,
   const PointVector* const vector) throw(...)
{
   _ASSERT(HasDocument());
   _ASSERT(IsMatrix());
   _ASSERT(m_PointVector.get());
   _ASSERT(m_ProxyContext.get());
#ifdef _DEBUG
   OGPS_Int32 value_x = -1, value_y = -1;
   if(vector != NULL)
   {
      if(vector->GetX()->GetPointType() == OGPS_Int32PointType)
      {
         vector->GetX()->Get(&value_x);
      }
      if(vector->GetY()->GetPointType() == OGPS_Int32PointType)
      {
         vector->GetY()->Get(&value_y);
      }
   }
#endif
   _ASSERT(!vector || (IsIncrementalX() && vector->GetX()->GetPointType() == OGPS_MissingPointType || (value_x >= 0 && u == (unsigned long)value_x)) || (!IsIncrementalX() && vector->GetX()->GetPointType() != OGPS_MissingPointType));
   _ASSERT(!vector || (IsIncrementalY() && vector->GetY()->GetPointType() == OGPS_MissingPointType || (value_y >= 0 && v == (unsigned long)value_y)) || (!IsIncrementalY() && vector->GetY()->GetPointType() != OGPS_MissingPointType));
   _ASSERT(!vector || vector->GetZ()->GetPointType() != OGPS_MissingPointType);

   if(!m_ProxyContext->IsMatrix())
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("Attempt to add a data point in matrix format when a point list is supported only."),
         _EX_T("The current instance of the document does not support the matrix topology."),
         _EX_T("OpenGPS::ISO5436_2Container::SetMatrixPoint"));
   }

   ((PointVectorProxyContextMatrix*)m_ProxyContext.get())->SetIndex(u, v, w);

   if(vector)
   {
      m_PointVector->Set(*vector);
   }

   GetVectorBuffer()->GetValidityProvider()->SetValid(m_ProxyContext->GetIndex(), vector != NULL);
}

void ISO5436_2Container::GetMatrixPoint(
   const unsigned long u,
   const unsigned long v,
   const unsigned long w,
   PointVector& vector) throw(...)
{
   _ASSERT(HasDocument());
   _ASSERT(IsMatrix());
   _ASSERT(m_PointVector.get());
   _ASSERT(m_ProxyContext.get());

   if(!m_ProxyContext->IsMatrix())
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("Attempt to read a data point in matrix format when a point list is supported only."),
         _EX_T("The current instance of the document does not support the matrix topology."),
         _EX_T("OpenGPS::ISO5436_2Container::GetMatrixPoint"));
   }

   ((PointVectorProxyContextMatrix*)m_ProxyContext.get())->SetIndex(u, v, w);

   if(GetVectorBuffer()->GetValidityProvider()->IsValid(m_ProxyContext->GetIndex()))
   {
      m_PointVector->Get(vector);
   }
   else
   {
      vector.GetX()->Reset();
      vector.GetY()->Reset();
      vector.GetZ()->Reset();
   }

   if(IsIncrementalX())
   {
      _ASSERT(vector.GetX()->GetPointType() == OGPS_MissingPointType);

      vector.SetX(ConvertULongToInt32(u));
   }

   if(IsIncrementalY())
   {
      _ASSERT(vector.GetY()->GetPointType() == OGPS_MissingPointType);

      vector.SetY(ConvertULongToInt32(v));
   }
}

void ISO5436_2Container::SetListPoint(
   const unsigned long index,
   const PointVector& vector) throw(...)
{   
   _ASSERT(HasDocument());
   _ASSERT(!IsMatrix());
   _ASSERT(m_PointVector.get());
   _ASSERT(m_ProxyContext.get());

   _ASSERT((IsIncrementalX() && vector.GetX()->GetPointType() == OGPS_MissingPointType) || (!IsIncrementalX() && vector.GetX()->GetPointType() != OGPS_MissingPointType) || (IsIncrementalX() && vector.GetX()->GetPointType() == OGPS_Int32PointType));
   _ASSERT((IsIncrementalY() && vector.GetY()->GetPointType() == OGPS_MissingPointType) || (!IsIncrementalY() && vector.GetY()->GetPointType() != OGPS_MissingPointType) || (IsIncrementalY() && vector.GetY()->GetPointType() == OGPS_Int32PointType));
   _ASSERT(vector.GetZ()->GetPointType() != OGPS_MissingPointType);

   if(m_ProxyContext->IsMatrix())
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("Attempt to add a data point to a point list when matrix topology is supported only."),
         _EX_T("Data points of the current instance of the document must be accessed as a point list only."),
         _EX_T("OpenGPS::ISO5436_2Container::SetListPoint"));
   }

   ((PointVectorProxyContextList*)m_ProxyContext.get())->SetIndex(index);

   m_PointVector->Set(vector);
}

void ISO5436_2Container::GetListPoint(
   const unsigned long index,
   PointVector& vector) throw(...)
{
   _ASSERT(HasDocument());
   _ASSERT(!IsMatrix());
   _ASSERT(m_PointVector.get());
   _ASSERT(m_ProxyContext.get());

   if(m_ProxyContext->IsMatrix())
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("Attempt to read a data point from a point list when matrix topology is supported only."),
         _EX_T("Data points of the current instance of the document must be accessed as a point list only."),
         _EX_T("OpenGPS::ISO5436_2Container::GetListPoint"));
   }

   ((PointVectorProxyContextList*)m_ProxyContext.get())->SetIndex(index);

   m_PointVector->Get(vector);

   if(IsIncrementalX())
   {
      _ASSERT(vector.GetX()->GetPointType() == OGPS_MissingPointType);

      vector.SetX(ConvertULongToInt32(index));
   }

   if(IsIncrementalY())
   {
      _ASSERT(vector.GetY()->GetPointType() == OGPS_MissingPointType);

      vector.SetY(ConvertULongToInt32(index));
   }
}

void ISO5436_2Container::GetMatrixCoord(
   const unsigned long u,
   const unsigned long v,
   const unsigned long w,
   OGPS_Double* const x,
   OGPS_Double* const y,
   OGPS_Double* const z) throw(...)
{
   _ASSERT(HasDocument());
   _ASSERT(IsMatrix());

   PointVector vector;
   GetMatrixPoint(u, v, w, vector);
   ConvertPointToCoord(vector, x, y, z);
}

OGPS_Boolean ISO5436_2Container::IsMatrixCoordValid(
   unsigned long u,
   unsigned long v,
   unsigned long w) throw(...)
{
   _ASSERT(HasDocument());
   _ASSERT(IsMatrix());

   if(!m_ProxyContext->IsMatrix())
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("Attempt to verify if a data point in matrix topology does exist when a point list is supported only."),
         _EX_T("Data points within point lists are always valid. It makes sense to verify when using matrix topology only."),
         _EX_T("OpenGPS::ISO5436_2Container::IsMatrixCoordValid"));
   }

   ((PointVectorProxyContextMatrix*)m_ProxyContext.get())->SetIndex(u, v, w);

   return GetVectorBuffer()->GetValidityProvider()->IsValid(m_ProxyContext->GetIndex());
}

void ISO5436_2Container::GetListCoord(
   const unsigned long index,
   OGPS_Double* const x,
   OGPS_Double* const y,
   OGPS_Double* const z) throw(...)
{
   _ASSERT(HasDocument());
   _ASSERT(!IsMatrix());

   PointVector vector;
   GetListPoint(index, vector);
   ConvertPointToCoord(vector, x, y, z);
}

void ISO5436_2Container::ConvertPointToCoord(
   const  PointVector& vector,
   OGPS_Double* const x,
   OGPS_Double* const y,
   OGPS_Double* const z) throw(...)
{
   OGPS_Double *myx, *myy, *myz;

   myx = vector.GetX()->IsValid() ? x : NULL;
   myy = vector.GetY()->IsValid() ? y : NULL;
   myz = vector.GetZ()->IsValid() ? z : NULL;

   vector.GetXYZ(myx, myy, myz);

   if(myx)
   {
      *myx *= GetIncrementX();
      *myx += GetOffsetX();
   }
   else
   {
      if(x)
      {
         *x = 0.0;
      }
   }

   if(myy)
   {
      *myy *= GetIncrementY();
      *myy += GetOffsetY();
   }
   else
   {
      if(y)
      {
         *y = 0.0;
      }
   }

   if(myz)
   {
      *myz += GetOffsetZ();
   }
   else
   {
      if(z)
      {
         *z = 0.0;
      }
   }
}

OpenGPS::Schemas::ISO5436_2::ISO5436_2Type* const ISO5436_2Container::GetDocument()
{
   return m_Document;
}

void ISO5436_2Container::Write(const int compressionLevel) throw(...)
{
   _ASSERT(compressionLevel >= -1 && compressionLevel <= 9);

   m_CompressionLevel = compressionLevel;

   Compress();
}

void ISO5436_2Container::Close()
{
   Reset();
   RemoveTempDir();
}

OGPS_Boolean ISO5436_2Container::IsMatrix() const throw(...)
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
   if(!m_Document->Record3().ListDimension().present())
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("It could not be decided whether the X3P instance is of either matrix or list type."),
         _EX_T("Point vectors can be either seen as topologically ordered matrix space or unordered sets. Make your decision within the XML document before using this method."),
         _EX_T("ISO5436_2Container::IsMatrix"));
   }

   return FALSE;
}

String ISO5436_2Container::CreateContainerTempFilePath() const
{
   const Environment* const env = Environment::GetInstance();

   return env->ConcatPathes(GetTempDir(), env->GetUniqueName());
}

String ISO5436_2Container::GetFullFilePath() const
{
   // TODO
   return m_FilePath;
}

const OpenGPS::String& ISO5436_2Container::GetFilePath() const
{
   return m_FilePath;
}

const OpenGPS::String& ISO5436_2Container::GetTempDir() const
{
   _ASSERT(m_TempPath.length() > 0);

   return m_TempPath;
}

String ISO5436_2Container::GetMainArchiveName() const
{
   return _OPENGPS_XSD_ISO5436_MAIN_PATH;
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
      const Environment* const env = Environment::GetInstance();
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
      const Environment* const env = Environment::GetInstance();
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

void ISO5436_2Container::DecompressMain() const throw(...)
{
   const OpenGPS::String src = GetMainArchiveName();
   const OpenGPS::String dst = GetMainFileName();

   _VERIFY(Decompress(src, dst), TRUE);
}

void ISO5436_2Container::DecompressChecksum() throw(...)
{
   _ASSERT(HasDocument());

   const OpenGPS::String src = GetChecksumArchiveName();
   const OpenGPS::String dst = GetChecksumFileName();

   _VERIFY(Decompress(src, dst), TRUE);
   VerifyMainChecksum();
}

void ISO5436_2Container::DecompressDataBin() throw(...)
{
   if(IsBinary())
   {
      _VERIFY(Decompress(GetPointDataArchiveName(), GetPointDataFileName()), TRUE);
      VerifyDataBinChecksum();

      if(HasValidPointsLink())
      {
         _VERIFY(Decompress(GetValidPointsArchiveName(), GetValidPointsFileName()), TRUE);
         VerifyValidBinChecksum();
      }
   }
}

OGPS_Boolean ISO5436_2Container::Decompress(const OpenGPS::String& src, const OpenGPS::String& dst, const OGPS_Boolean fileNotFoundAllowed) const throw(...)
{
   // Gets absolute path to X3P file.
   OpenGPS::String filePath = GetFullFilePath();

   // Gets handler to zip archive or NULL.
   unzFile handle = unzOpen(filePath.ToChar());

   if(!handle)
   {
      throw OpenGPS::Exception(
         OGPS_ExGeneral,
         _EX_T("The X3P archive to be decompressed could not be opened."),
         _EX_T("Verify whether the file exists and if you have sufficient access privilegs."),
         _EX_T("OpenGPS::ISO5436_2Container::Decompress"));
   }

   OGPS_Boolean fileNotFound = FALSE;
   OGPS_Boolean fileNotOpened = FALSE;
   OGPS_Boolean targetNotWritten = FALSE;
   OGPS_Boolean success = FALSE;

   try
   {
      // Locate the document/file to be decompressed in the archive.
      OpenGPS::String srcbuf(src);
      if(unzLocateFile(handle, srcbuf.ToChar(), 2 /* case insensitive search */) == UNZ_OK)
      {
         // Open the current file for reading.
         if(unzOpenCurrentFile(handle) == UNZ_OK)
         {
            // Need information about file size
            unz_file_info fileInfo;
            if(unzGetCurrentFileInfo(handle, &fileInfo, NULL, 0, NULL, 0, NULL, 0) == UNZ_OK)
            {
               // Open binary target stream for uncompressed data
               OutputBinaryFileStream binaryTarget(dst);

               if(!binaryTarget.fail())
               {
                  unsigned long written = 0;
                  unsigned long length = fileInfo.uncompressed_size;

                  // Don't uncompress this file as a whole, but in loops
                  // of a predefined maximum chunk size. Otherwise we
                  // might get out of memory...
                  while(written < length)
                  {
                     int size = min(length - written, _OPENGPS_ZIP_CHUNK_MAX);                  

                     _ASSERT(size > 0);

                     voidp buffer = (voidp)malloc(size);

                     if(!buffer)
                     {
                        written = 0;
                        break;
                     }

                     int bytesCopied = 0;

                     try
                     {
                        bytesCopied = unzReadCurrentFile(handle, buffer, size);

                        if(bytesCopied == size)
                        {
                           binaryTarget.write((OpenGPS::UnsignedBytePtr)buffer, bytesCopied);
                        }

                        _OPENGPS_FREE(buffer);
                     }
                     catch(...)
                     {
                        _OPENGPS_FREE(buffer);
                        throw;
                     }

                     if(bytesCopied != size || binaryTarget.fail())
                     {
                        written = 0;
                        break;
                     }

                     written += size;
                  }

                  success = (written == length);
               }
               else
               {
                  targetNotWritten = TRUE;
               }

               binaryTarget.close();

               targetNotWritten = targetNotWritten || binaryTarget.fail();
            }
            else
            {
               fileNotOpened = TRUE;
            }
         }
         else
         {
            fileNotOpened = TRUE;
         }
      }
      else
      {
         fileNotFound = TRUE;
      }
   }
   catch(...)
   {
      // Closes the file handle.
      _VERIFY(unzClose(handle), UNZ_OK);
      throw;
   }

   // Closes the file handle.
   _VERIFY(unzClose(handle), UNZ_OK);

   if(fileNotFound && !fileNotFoundAllowed)
   {
      if(fileNotFound)
      {
         throw OpenGPS::Exception(
            OGPS_ExGeneral,
            _EX_T("The X3P container document does not contain the supposed resource."),
            _EX_T("For a X3P archive to be valid there must exist an instance of the ISO5436-2 XML specification in its root named main.xml. Also all additional resources given in main.xml must be contained herein."),
            _EX_T("OpenGPS::ISO5436_2Container::Decompress"));
      }

      if(fileNotOpened)
      {
         throw OpenGPS::Exception(
            OGPS_ExGeneral,
            _EX_T("A resource successfully located within the X3P archive could not be opened."),
            _EX_T("Please check whether the X3P archive is corrupted using a zip file utility of your choice, then repair the archive and try again."),
            _EX_T("OpenGPS::ISO5436_2Container::Decompress"));
      }

      if(targetNotWritten)
      {
         throw OpenGPS::Exception(
            OGPS_ExGeneral,
            _EX_T("A resource contained in an X3P archive could not be extracted to its target file."),
            _EX_T("Please check your access permissions on the temporary directory."),
            _EX_T("OpenGPS::ISO5436_2Container::Decompress"));
      }

      if(!success)
      {
         throw OpenGPS::Exception(
            OGPS_ExGeneral,
            _EX_T("A resource contained in an X3P archive could not be extracted to its target file."),
            _EX_T("Please check if you have enough space left on your filesystem."),
            _EX_T("OpenGPS::ISO5436_2Container::Decompress"));
      }
   }

   return success;
}

void ISO5436_2Container::Compress() throw(...)
{
   OGPS_Boolean noHandleCreated = FALSE;
   OGPS_Boolean vendorfilesAdded = TRUE;
   OGPS_Boolean success = FALSE;

   CreateTempDir();

   OpenGPS::String targetZip = CreateContainerTempFilePath();
   zipFile handle = zipOpen(targetZip.ToChar(), APPEND_STATUS_CREATE);

   try
   {
      if(handle)
      {
         try
         {
            SavePointBuffer(handle);
            vendorfilesAdded = WriteVendorSpecific(handle);
            SaveXmlDocument(handle);

            if(HasValidPointsLink())
            {
               SaveValidPointsLink(handle);
            }

            success = TRUE;
         }
         catch(...)
         {
            _VERIFY(zipClose(handle, NULL), ZIP_OK);
            throw;
         }
         _VERIFY(zipClose(handle, NULL), ZIP_OK);
      }
      else
      {
         noHandleCreated = TRUE;
      }

      if(success)
      {
         Environment::GetInstance()->RenameFile(targetZip, GetFullFilePath());
      }
   }
   catch(...)
   {
      RemoveTempDir();
      throw;
   }

   RemoveTempDir();

   if(noHandleCreated)
   {
      throw OpenGPS::Exception(
         OGPS_ExGeneral,
         _EX_T("The X3P archive could not be written."),
         _EX_T("Zlib library could not create a handle to the X3P target file. Check your permissions on the temporary directory."),
         _EX_T("OpenGPS::ISO5436_2Container::Compress"));
   }

   if(!vendorfilesAdded)
   {
      throw OpenGPS::Exception(
         OGPS_ExWarning,
         _EX_T("At least some of the vendorspecific files supplied could not be added to the X3P archive."),
         _EX_T("Please verify that the files containing additional data still exist on the filesystem at this time."),
         _EX_T("OpenGPS::ISO5436_2Container::Compress"));
   }
}

void ISO5436_2Container::CreateDocument(
   const Schemas::ISO5436_2::Record1Type* const record1,
   const Schemas::ISO5436_2::Record2Type* const record2,
   const Schemas::ISO5436_2::MatrixDimensionType* const matrixDimension,
   const unsigned long listDimension,
   const OGPS_Boolean useBinaryData) throw(...)
{
   _ASSERT(!HasDocument());
   _ASSERT(record1);
   _ASSERT(matrixDimension != NULL && listDimension == 0 || matrixDimension == NULL && listDimension > 0);

   try
   {
      Schemas::ISO5436_2::Record3Type record3;
      Schemas::ISO5436_2::Record4Type record4(_OPENGPS_XSD_ISO5436_MAIN_CHECKSUM_PATH);

      m_Document = new Schemas::ISO5436_2::ISO5436_2Type(*record1, record3, record4);

      if(record2)
      {         
         m_Document->Record2(*record2);
      }

      if(matrixDimension)
      {
         m_Document->Record3().MatrixDimension(*matrixDimension);
      }
      else
      {
         m_Document->Record3().ListDimension(listDimension);
      }

      if(useBinaryData)
      {
         Schemas::ISO5436_2::DataLinkType dataLink(_OPENGPS_XSD_ISO5436_DATALINK_PATH, NULL);
         m_Document->Record3().DataLink(dataLink);
      }
      else
      {
         Schemas::ISO5436_2::DataListType dataList;
         m_Document->Record3().DataList(dataList);
      }

      // Build and setup internal point buffer
      VectorBufferBuilderAutoPtr v_builder(new VectorBufferBuilder());
      if(BuildVectorBuffer(*v_builder.get()))
      {
         m_VectorBufferBuilder = v_builder;
      }
      m_ProxyContext.reset(CreatePointVectorProxyContext());
      _ASSERT(m_ProxyContext.get());
      m_PointVector = GetVectorBuffer()->GetPointVectorProxy(*m_ProxyContext.get());
   }
   catch(...)
   {
      Reset();
      throw;
   }

   m_IsReadOnly = FALSE;
   m_IsCreating = TRUE;
}

void ISO5436_2Container::ReadDocument() throw(...)
{
   _ASSERT(!HasDocument());

   ReadXmlDocument();

   _ASSERT(HasDocument());
}

void ISO5436_2Container::ReadXmlDocument() throw(...)
{
   xml_schema::properties props;
   if(!ConfigureNamespaceMap(props))
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("The location of your local iso5436_2.xsd specification could not be extracted."),
         _EX_T("Verify that the environment variable OPENGPS_LOCATION is set and correctly points to the root of your local openGPS(R) installation."),
         _EX_T("OpenGPS::ISO5436_2Container::ReadXmlDocument"));
   }

   OpenGPS::String xmlFilePath = GetMainFileName();

   try
   {
      m_Document = Schemas::ISO5436_2::ISO5436_2(xmlFilePath, 0, props).release();
   }
   catch(const xml_schema::exception& e)
   {
#ifdef _UNICODE
      std::wostringstream dump;
#else
      std::ostringstream dump;
#endif /* _UNICODE */

      dump << e;

      OpenGPS::String m(dump.str());

      throw OpenGPS::Exception(
         OGPS_ExGeneral,
         e.what(),
         m.ToChar(),
         NULL);
   }
}

void ISO5436_2Container::CreatePointBuffer() throw(...)
{
   _ASSERT(!HasVectorBuffer());
   _ASSERT(HasDocument());

   // Build and setup internal point buffer
   VectorBufferBuilderAutoPtr v_builder(new VectorBufferBuilder());
   if(BuildVectorBuffer(*v_builder.get()))
   {
      m_VectorBufferBuilder = v_builder;
   }

   VectorBuffer* const vectorBuffer = GetVectorBuffer();

   // read valid points file
   if(HasValidPointsLink() && vectorBuffer->HasValidityBuffer())
   {
      InputBinaryFileStream vstream(GetValidPointsFileName());
      vectorBuffer->GetValidityBuffer()->Read(vstream);
   }

   // Create point parser for this document
   PointVectorParserBuilderAutoPtr p_builder(new PointVectorParserBuilder());
   BuildPointVectorParser(*p_builder.get());

   PointVectorParser* parser = p_builder->GetParser();
   PointVectorReaderContextAutoPtr context(CreatePointVectorReaderContext());
   PointVectorProxyContextAutoPtr proxy_context(CreatePointVectorProxyContext());

   _ASSERT(context.get());
   _ASSERT(proxy_context.get());

   PointVectorAutoPtr vector(vectorBuffer->GetPointVectorProxy(*proxy_context.get()));

   while(context->MoveNext())
   {
      if(context->IsValid())
      {
         parser->Read(*context.get(), *vector);
      }
      else
      {
         if(vectorBuffer->HasValidityBuffer())
         {
            if(!vectorBuffer->GetValidityBuffer()->IsAllocated())
            {
               vectorBuffer->GetValidityBuffer()->Allocate();
            }
         }

         vectorBuffer->GetValidityProvider()->SetValid(proxy_context->GetIndex(), FALSE);
      }

      proxy_context->IncrementIndex();
   }

   // When the point buffer has been created,
   // we can savely drop the original xml content
   ResetXmlPointList();

   // initialize global vector proxy
   m_ProxyContext.reset(CreatePointVectorProxyContext());
   _ASSERT(m_ProxyContext.get());
   m_PointVector = vectorBuffer->GetPointVectorProxy(*m_ProxyContext.get());
}

void ISO5436_2Container::ResetXmlPointList()
{
   _ASSERT(HasDocument());

   if(m_Document->Record3().DataList().present())
   {
      m_Document->Record3().DataList()->Datum().clear();
   }
}

void ISO5436_2Container::ResetValidPointsLink()
{
   _ASSERT(HasDocument());

   if(!IsBinary() || !GetVectorBuffer()->HasValidityBuffer() || !GetVectorBuffer()->GetValidityBuffer()->IsAllocated())
   {
      // no, we do not need an external validity file
      if(m_Document->Record3().DataLink().present() && m_Document->Record3().DataLink()->ValidPointsLink().present())
      {
         m_Document->Record3().DataLink()->ValidPointsLink().reset();
      }
   }
   else
   {
      // yes, we do need an external validity file, so set default pathes if nothing has been set up yet
      if(!m_Document->Record3().DataLink()->ValidPointsLink().present())
      {
         m_Document->Record3().DataLink()->ValidPointsLink(_OPENGPS_XSD_ISO5436_VALIDPOINTSLINK_PATH);
      }
   }

   // reset any checksum data
   if(m_Document->Record3().DataLink().present() && m_Document->Record3().DataLink()->MD5ChecksumValidPoints().present())
   {
      m_Document->Record3().DataLink()->MD5ChecksumValidPoints().reset();
   }
}

void ISO5436_2Container::SaveChecksumFile(zipFile handle, const OpenGPS::UnsignedByte checksum[16]) throw(...)
{
   _ASSERT(handle);

   OGPS_Boolean retval = FALSE;

   OpenGPS::String section(GetChecksumArchiveName());
   if(zipOpenNewFileInZip(handle,
      section.ToChar(),
      NULL,
      NULL,
      0,
      NULL,
      0,
      NULL,
      Z_DEFLATED,
      m_CompressionLevel) == ZIP_OK)
   {
      ZipStreamBuffer buffer(handle, FALSE);
      ZipOutputStream zipOut(buffer);

      if (!zipOut.fail ())
      {
         OpenGPS::String md5out;
         if(md5out.ConvertFromMd5(checksum))
         {
            OpenGPS::String mainArchiveName(GetMainArchiveName());

            zipOut.write(md5out.ToChar());
            zipOut.write(" *");
            zipOut.write(mainArchiveName.ToChar());
            zipOut.write("\n");

            retval = TRUE;
         }
      }

      if(zipCloseFileInZip(handle) == ZIP_OK)
      {
         retval = retval && TRUE;
      }
   }

   if(!retval)
   {
      throw OpenGPS::Exception(
         OGPS_ExGeneral,
         _EX_T("The main checksum file could not be written to the X3P file container."),
         _EX_T("Check whether there is enough space left on your filesystem and for appropriate permissions."),
         _EX_T("OpenGPS::ISO5436_2Container::SaveChecksumFile"));
   }
}

void ISO5436_2Container::SaveXmlDocument(zipFile handle) throw(...)
{
   _ASSERT(handle);

   xml_schema::namespace_infomap map;
   ConfigureNamespaceMap(map);

   // Creates new file in the zip container.
   OpenGPS::String mainDocument(GetMainArchiveName());
   if(zipOpenNewFileInZip(handle,
      mainDocument.ToChar(),
      NULL,
      NULL,
      0,
      NULL,
      0,
      NULL,
      Z_DEFLATED,
      m_CompressionLevel) != ZIP_OK)
   {
      throw OpenGPS::Exception(
         OGPS_ExGeneral,
         _EX_T("The main ISO5436-2 XML document could not be written to the X3P file container."),
         _EX_T("Zlib could not open the appropriate handle. Check your permissions and that there is enough space left on your filesystem."),
         _EX_T("OpenGPS::ISO5436_2Container::SaveXmlDocument"));
   }

   ZipStreamBuffer buffer(handle, TRUE);
   ZipOutputStream zipOut(buffer);

   if (!zipOut.fail ())
   {
      try
      {
         xercesc::XMLPlatformUtils::Initialize();
         try
         {
            Schemas::ISO5436_2::ISO5436_2(zipOut, *m_Document, map, _T("UTF-8"), xml_schema::flags::dont_initialize);
            xercesc::XMLPlatformUtils::Terminate();
         }
         catch(...)
         {
            xercesc::XMLPlatformUtils::Terminate();
            throw;
         }
      }
      catch(const xml_schema::exception& e)
      {
         _VERIFY(zipCloseFileInZip(handle), ZIP_OK);

#ifdef _UNICODE
         std::wostringstream dump;
#else
         std::ostringstream dump;
#endif /* _UNICODE */

         dump << e;
      
         OpenGPS::String m(dump.str());

         throw OpenGPS::Exception(
            OGPS_ExGeneral,
            e.what(),
            m.ToChar(),
            NULL);
      }
      catch(...)
      {
         _VERIFY(zipCloseFileInZip(handle), ZIP_OK);
         throw;
      }
      _VERIFY(zipCloseFileInZip(handle), ZIP_OK);
   }

   OpenGPS::UnsignedByte md5[16];
   _VERIFY(buffer.GetMd5(md5), TRUE);
   SaveChecksumFile(handle, md5);
}

void ISO5436_2Container::ConfigureNamespaceMap(xml_schema::namespace_infomap& map) const
{
   map[_T("p")].name = _OPENGPS_XSD_ISO5436_NAMESPACE;
   map[_T("p")].schema = _OPENGPS_XSD_ISO5436_LOCATION;
}

OGPS_Boolean ISO5436_2Container::ConfigureNamespaceMap(xml_schema::properties& props) const
{
   OpenGPS::String xsdPathBuf;
   if(Environment::GetInstance()->GetVariable(_OPENGPS_ENV_OPENGPS_LOCATION, xsdPathBuf))
   {
      xsdPathBuf = Environment::GetInstance()->ConcatPathes(xsdPathBuf, _OPENGPS_ISO5436_LOCATION);

      OpenGPS::String xsdPath;
      Environment::GetInstance()->GetPathName(xsdPathBuf, xsdPath);
      if(xsdPath.size() > 0)
      {
         xsdPath = _OPENGPS_FILE_URI_PREF + xsdPath;
         props.schema_location(_OPENGPS_XSD_ISO5436_NAMESPACE, xsdPath);
         return TRUE;
      }
   }

   return FALSE;
}

void ISO5436_2Container::SaveValidPointsLink(zipFile handle) throw(...)
{
   _ASSERT(HasValidPointsLink());

   // Creates new file in the zip container.
   OpenGPS::String section(GetValidPointsArchiveName());
   if(zipOpenNewFileInZip(handle,
      section.ToChar(),
      NULL,
      NULL,
      0,
      NULL,
      0,
      NULL,
      Z_DEFLATED,
      m_CompressionLevel) != ZIP_OK)
   {
      throw OpenGPS::Exception(
         OGPS_ExGeneral,
         _EX_T("The binary point validity file could not be written to the X3P archive."),
         _EX_T("Zlib could not open the appropriate handle. Check your permissions and that there is enough space left on your filesystem."),
         _EX_T("OpenGPS::ISO5436_2Container::SaveValidPointsLink"));
   }

   VectorBuffer* const vectorBuffer = GetVectorBuffer();

   ZipStreamBuffer vbuffer(handle, TRUE);
   ZipOutputStream vstream(vbuffer);

   try
   {
      if(!vstream.fail())
      {
         _ASSERT(vectorBuffer->HasValidityBuffer());
         vectorBuffer->GetValidityBuffer()->Write(vstream);
      }
   }
   catch(...)
   {
      _VERIFY(zipCloseFileInZip(handle), ZIP_OK);
      throw;
   }
   _VERIFY(zipCloseFileInZip(handle), ZIP_OK);

   OpenGPS::UnsignedByte md5[16];
   _VERIFY(vbuffer.GetMd5(md5), TRUE);

   const Schemas::ISO5436_2::DataLinkType::MD5ChecksumValidPoints_type checksum(md5, 16);
   m_Document->Record3().DataLink()->MD5ChecksumValidPoints(checksum);
}

void ISO5436_2Container::SavePointBuffer(zipFile handle) throw(...)
{
   _ASSERT(handle);

   OGPS_Boolean isBinary = IsBinary();

   if(isBinary)
   {
      // Creates new file in the zip container.
      OpenGPS::String section(GetPointDataArchiveName());
      if(zipOpenNewFileInZip(handle,
         section.ToChar(),
         NULL,
         NULL,
         0,
         NULL,
         0,
         NULL,
         Z_DEFLATED,
         m_CompressionLevel) != ZIP_OK)
      {
         throw OpenGPS::Exception(
            OGPS_ExGeneral,
            _EX_T("Could not write binary point data to the X3P archive."),
            _EX_T("Zlib could not open the target handle. Check for filesystem permissions and enough space left."),
            _EX_T("OpenGPS::ISO5436_2Container::SavePointBuffer"));
      }
   }

   try
   {
      _ASSERT(HasDocument() && HasVectorBuffer());

      // Before we start: reset changes made to the
      // point list xml tag. Points will be replaced
      // with those vlaues in the current vector buffer.
      ResetXmlPointList();
      ResetValidPointsLink();

      // Create point parser for this document
      PointVectorParserBuilderAutoPtr p_builder(new PointVectorParserBuilder());
      BuildPointVectorParser(*p_builder.get());

      PointVectorParser* parser = p_builder->GetParser();
      PointVectorWriterContextAutoPtr context(CreatePointVectorWriterContext(handle));

      _ASSERT(context.get());

      VectorBuffer* const vectorBuffer = GetVectorBuffer();

      PointVectorProxyContextAutoPtr proxy_context(CreatePointVectorProxyContext());

      _ASSERT(proxy_context.get());

      PointVectorAutoPtr vector(vectorBuffer->GetPointVectorProxy(*proxy_context.get()));

      if(proxy_context->CanIncrementIndex())
      {
         do
         {
            if(isBinary || vectorBuffer->GetValidityProvider()->IsValid(proxy_context->GetIndex()))
            {
               parser->Write(*context.get(), *vector);
            }
            context->MoveNext();
         } while(proxy_context->IncrementIndex());
      }

      if(isBinary)
      {
         /* MD5 checksum */
         OpenGPS::UnsignedByte md5[16];
         ((BinaryPointVectorWriterContext*)context.get())->GetMd5(md5);
         const Schemas::ISO5436_2::DataLinkType::MD5ChecksumPointData_type checksum(md5, 16);
         m_Document->Record3().DataLink()->MD5ChecksumPointData(checksum);
      }
   }
   catch(...)
   {
      if(isBinary)
      {
         _VERIFY(zipCloseFileInZip(handle), ZIP_OK);
      }
      throw;
   }

   if(isBinary)
   {
      _VERIFY(zipCloseFileInZip(handle), ZIP_OK);
   }
}

void ISO5436_2Container::BuildPointVectorParser(PointVectorParserBuilder& builder) const
{
   OGPS_DataPointType xType = GetXaxisDataType();
   OGPS_DataPointType yType = GetYaxisDataType();
   OGPS_DataPointType zType = GetZaxisDataType();

   builder.BuildParser();
   builder.BuildX(xType);
   builder.BuildY(yType);
   builder.BuildZ(zType);
}

OGPS_Boolean ISO5436_2Container::BuildVectorBuffer(VectorBufferBuilder& builder) const
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
      builder.BuildValidityProvider());
}

PointVectorReaderContext* ISO5436_2Container::CreatePointVectorReaderContext()
{
   // instantiate binary reader context
   if(IsBinary())
   {
      // path to the source binary file
      const OpenGPS::String binaryFilePath = GetPointDataFileName();
      if(binaryFilePath.length() > 0)
      {
         // find out if we are on lsb or msb
         // hardware and create appropriate context
         if(Environment::GetInstance()->IsLittleEndian())
         {
            return new BinaryLSBPointVectorReaderContext(binaryFilePath);
         }

         return new BinaryMSBPointVectorReaderContext(binaryFilePath);
      }

      return NULL;
   }

   // instantiate xml string reader context...
   // find list tag in xml document and check whether it is present
   const Schemas::ISO5436_2::Record3Type::DataList_optional& dataList = m_Document->Record3().DataList();
   if(dataList.present())
   {
      const Schemas::ISO5436_2::DataListType::Datum_sequence& datum = dataList->Datum();
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
      // hardware and create appropriate context
      if(Environment::GetInstance()->IsLittleEndian())
      {
         return new BinaryLSBPointVectorWriterContext(handle);
      }

      return new BinaryMSBPointVectorWriterContext(handle);
   }

   // instantiate xml string reader context...
   // find list tag in xml document and check whether it is present
   Schemas::ISO5436_2::Record3Type::DataList_optional& dataList = m_Document->Record3().DataList();
   if(dataList.present())
   {
      Schemas::ISO5436_2::DataListType::Datum_sequence& datum = dataList->Datum();

      // should be empty for now (and later filled by the writer)
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

OGPS_DataPointType ISO5436_2Container::GetAxisDataType(const Schemas::ISO5436_2::AxisDescriptionType& axis, const OGPS_Boolean incremental) const
{
   if(axis.DataType().present())
   {
      if(!incremental || axis.AxisType() != Schemas::ISO5436_2::AxisType::I)
      {
         switch(axis.DataType().get())
         {
         case Schemas::ISO5436_2::DataType::I:
            return OGPS_Int16PointType;
         case Schemas::ISO5436_2::DataType::L:
            return OGPS_Int32PointType;
         case Schemas::ISO5436_2::DataType::F:
            return OGPS_FloatPointType;
         case Schemas::ISO5436_2::DataType::D:
            return OGPS_DoublePointType;
         default:
            _ASSERT(FALSE);
            break;
         }
      }
   }

   return OGPS_MissingPointType;
}

unsigned long ISO5436_2Container::GetPointCount() const throw(...)
{
   _ASSERT(HasDocument());

   // calculate point count of matrix type
   if(IsMatrix())
   {
      const unsigned long long xSize = m_Document->Record3().MatrixDimension()->SizeX();
      const unsigned long long ySize = m_Document->Record3().MatrixDimension()->SizeY();
      const unsigned long long zSize = m_Document->Record3().MatrixDimension()->SizeZ();

      _ASSERT(xSize > 0 && ySize > 0 && zSize > 0);

      return SafeMultipilcation(SafeMultipilcation(xSize, ySize), zSize);
   }

   // calculate point count of list type
   return ConvertULongLongToULong(m_Document->Record3().ListDimension().get());
}

unsigned long ISO5436_2Container::GetMaxU() const throw(...)
{
   _ASSERT(HasDocument());

   if(IsMatrix())
   {
      return ConvertULongLongToULong(m_Document->Record3().MatrixDimension()->SizeX());
   }

   return ConvertULongLongToULong(m_Document->Record3().ListDimension().get());
}

unsigned long ISO5436_2Container::GetMaxV() const throw(...)
{
   _ASSERT(HasDocument());

   if(IsMatrix())
   {
      return ConvertULongLongToULong(m_Document->Record3().MatrixDimension()->SizeY());
   }

   return ConvertULongLongToULong(m_Document->Record3().ListDimension().get());
}

unsigned long ISO5436_2Container::GetMaxW() const throw(...)
{
   _ASSERT(HasDocument());

   if(IsMatrix())
   {
      return ConvertULongLongToULong(m_Document->Record3().MatrixDimension()->SizeZ());
   }

   return ConvertULongLongToULong(m_Document->Record3().ListDimension().get());
}

OGPS_Boolean ISO5436_2Container::IsBinary() const
{
   _ASSERT(HasDocument());

   const OGPS_Boolean hasDataLink = m_Document->Record3().DataLink().present();

   // ensure data ist available in either binary or text format
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
   m_MainChecksum = TRUE;
   m_DataBinChecksum = TRUE;
   m_ValidBinChecksum = TRUE;
   m_IsReadOnly = FALSE;
   _OPENGPS_DELETE(m_Document);
   m_VectorBufferBuilder.release();
   m_PointVector.release();
   m_ProxyContext.release();
   m_PointDataFileName.clear();
   m_ValidPointsFileName.clear();
   m_VendorURI.clear();
   m_VendorSpecific.clear();
}

OGPS_Boolean ISO5436_2Container::HasDocument() const
{
   return m_Document != NULL;
}

OGPS_Boolean ISO5436_2Container::HasVectorBuffer() const
{
   OGPS_Boolean retval = m_VectorBufferBuilder.get() != NULL;
   _ASSERT(!retval || m_VectorBufferBuilder->GetBuffer());
   return retval;
}

void ISO5436_2Container::CreateTempDir() throw(...)
{
   _ASSERT(!HasTempDir());

   const Environment* const env = Environment::GetInstance();
   const OpenGPS::String uniqueDirectory = env->GetUniqueName();

   OpenGPS::String temp;
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
      const OpenGPS::String sysTemp = env->GetTempDir();
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

      throw OpenGPS::Exception(
         OGPS_ExGeneral,
         _EX_T("Temporary directory could not be created."),
         _EX_T("Check for filesystem permissions."),
         _EX_T("OpenGPS::ISO5436_2Container::CreateTempDir"));
   }
}

OGPS_Boolean ISO5436_2Container::HasTempDir() const
{
   return m_TempPath.length() > 0;
}

void ISO5436_2Container::RemoveTempDir()
{
   m_PointDataFileName.clear();
   m_ValidPointsFileName.clear();

   if(HasTempDir())
   {
      if(Environment::GetInstance()->RemoveDir(m_TempPath))
      {
         m_TempPath.clear();
      }
   }
}

VectorBuffer* ISO5436_2Container::GetVectorBuffer()
{
   _ASSERT(m_VectorBufferBuilder.get());
   return m_VectorBufferBuilder->GetBuffer();
}

OGPS_Boolean ISO5436_2Container::IsIncrementalX() const
{
   _ASSERT(HasDocument());

   const Schemas::ISO5436_2::Record1Type::Axes_type::CX_type& cx = m_Document->Record1().Axes().CX();
   return (cx.AxisType() == Schemas::ISO5436_2::Record1Type::Axes_type::CX_type::AxisType_type::I);
}

OGPS_Boolean ISO5436_2Container::IsIncrementalY() const
{
   _ASSERT(HasDocument());

   const Schemas::ISO5436_2::Record1Type::Axes_type::CY_type& cy = m_Document->Record1().Axes().CY();
   return (cy.AxisType() == Schemas::ISO5436_2::Record1Type::Axes_type::CY_type::AxisType_type::I);
}

double ISO5436_2Container::GetIncrementX() const
{
   _ASSERT(HasDocument());

   const Schemas::ISO5436_2::Record1Type::Axes_type::CX_type& cx = m_Document->Record1().Axes().CX();
   if(cx.AxisType() == Schemas::ISO5436_2::Record1Type::Axes_type::CX_type::AxisType_type::I)
   {
      if(cx.Increment().present())
      {
         return cx.Increment().get();
      }
   }

   return (1.0);
}

double ISO5436_2Container::GetIncrementY() const
{
   _ASSERT(HasDocument());

   const Schemas::ISO5436_2::Record1Type::Axes_type::CY_type& cy = m_Document->Record1().Axes().CY();
   if(cy.AxisType() == Schemas::ISO5436_2::Record1Type::Axes_type::CY_type::AxisType_type::I)
   {
      if(cy.Increment().present())
      {
         return cy.Increment().get();
      }
   }

   return (1.0);
}

double ISO5436_2Container::GetOffsetX() const
{
   _ASSERT(HasDocument());

   const Schemas::ISO5436_2::Record1Type::Axes_type::CX_type& cx = m_Document->Record1().Axes().CX();
   if(cx.Offset().present())
   {
      return cx.Offset().get();
   }

   return (0.0);
}

double ISO5436_2Container::GetOffsetY() const
{
   _ASSERT(HasDocument());

   const Schemas::ISO5436_2::Record1Type::Axes_type::CY_type& cy = m_Document->Record1().Axes().CY();
   if(cy.Offset().present())
   {
      return cy.Offset().get();
   }

   return (0.0);
}

double ISO5436_2Container::GetOffsetZ() const
{
   _ASSERT(HasDocument());

   const Schemas::ISO5436_2::Record1Type::Axes_type::CZ_type& cz = m_Document->Record1().Axes().CZ();
   if(cz.Offset().present())
   {
      return cz.Offset().get();
   }

   return (0.0);
}

PointVectorProxyContext* ISO5436_2Container::CreatePointVectorProxyContext() const throw(...)
{
   _ASSERT(HasDocument());

   if(IsMatrix())
   {
      const Schemas::ISO5436_2::MatrixDimensionType& mtype = m_Document->Record3().MatrixDimension().get();

      SafeMultipilcation(SafeMultipilcation(mtype.SizeX(), mtype.SizeY()), mtype.SizeZ());
      
      const unsigned long sx = ConvertULongLongToULong(mtype.SizeX());
      const unsigned long sy = ConvertULongLongToULong(mtype.SizeY());
      const unsigned long sz = ConvertULongLongToULong(mtype.SizeZ());

      return new PointVectorProxyContextMatrix(sx, sy, sz);
   }

   _ASSERT(m_Document->Record3().ListDimension().present());

   return new PointVectorProxyContextList(ConvertULongLongToULong(m_Document->Record3().ListDimension().get()));
}

OGPS_Int32 ISO5436_2Container::ConvertULongToInt32(const unsigned long long value) const throw(...)
{
   if((unsigned long long)std::numeric_limits<OGPS_Int32>::max() < value)
   {
      throw OpenGPS::Exception(OGPS_ExOverflow,
         _EX_T("An integer overflow occured."),
         _EX_T("Point data of implicit axes is derived from the current point index. Since point indexes are of unsigned type, very large values can not be converted into a signed integer. But within a point vector the components are of signed integer and therefore the current index value could not be saved in a point vector. In other words: reduce the amount of point data stored to be able to fit the upper bound of a signed integer data type."),
         _EX_T("OpenGPS::ISO5436_2Container::ConvertULongToInt32"));
   }

   return (OGPS_Int32)value;
}

unsigned long ISO5436_2Container::ConvertULongLongToULong(const unsigned long long value) const throw(...)
{
   if((unsigned long long)std::numeric_limits<unsigned long>::max() < value)
   {
      throw OpenGPS::Exception(OGPS_ExOverflow,
         _EX_T("An integer overflow occured."),
         _EX_T("The amount of point data stored as a list of points or at least one of the dimensions of the topology matrix did exceed the maximum value that could be handled by an unsigned long data type. To avoid this problem either reduce the amount of point data stored or the dimensions of the matrix topology to be able to fit the upper bound of an unsigned long data type."),
         _EX_T("OpenGPS::ISO5436_2Container::ConvertULongLongToULong"));
   }

   return (unsigned long)value;
}

unsigned long ISO5436_2Container::SafeMultipilcation(const unsigned long long value1, const unsigned long long value2) const throw(...)
{
   if(value1 > (std::numeric_limits<unsigned long>::max() / value2))
   {
      throw OpenGPS::Exception(OGPS_ExOverflow,
         _EX_T("An integer overflow occured due to a multiplication operation."),
         _EX_T("Two values of the same data type were multiplied with each other, but the result is too large to fit the same data type. Your point data is layed out by a matrix topology. Then every data point of a single axis may be indexed successfully of their own, but a single index might not be sufficient to be able of indexing all three axes components at once. Since this is a required property of the current implementation, the amount of point data needs to be reduced, so that indexing of all axes components with a single index becomes possible again."),
         _EX_T("OpenGPS::ISO5436_2Container::SafeMultipilcation"));
   }

   return ((unsigned long)(value1 * value2));
}

void ISO5436_2Container::TestChecksums() throw(...)
{
   if(!m_MainChecksum)
   {
      throw OpenGPS::Exception(
         OGPS_ExWarning,
         _EX_T("The checksum of the main.xml document contained in an X3P archive could not be verified."),
         _EX_T("Although some data had been extracted there is no guarantee of their integrity."),
         _EX_T("OpenGPS::ISO5436_2Container::TestChecksums"));
   }

   if(!m_DataBinChecksum)
   {
      throw OpenGPS::Exception(
         OGPS_ExWarning,
         _EX_T("The checksum of binary point data contained in an X3P archive could not be verified."),
         _EX_T("Although some data had been extracted there is no guarantee of their integrity."),
         _EX_T("OpenGPS::ISO5436_2Container::TestChecksums"));
   }
   
   if(!m_ValidBinChecksum)
   {
      throw OpenGPS::Exception(
         OGPS_ExWarning,
         _EX_T("The checksum of binary point validity data contained in an X3P archive could not be verified."),
         _EX_T("Although some data had been extracted there is no guarantee of their integrity."),
         _EX_T("OpenGPS::ISO5436_2Container::TestChecksums"));
   }
}

void ISO5436_2Container::AppendVendorSpecific(const OpenGPS::String& vendorURI, const OpenGPS::String& filePath)
{
   _ASSERT(vendorURI.size() > 0 && filePath.size() > 0);
   _ASSERT(m_VendorURI.empty() || m_VendorSpecific.size() > 0);

   if(!m_VendorURI.empty() && m_VendorURI != vendorURI)
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("The vendor URI specified is inconsistent."),
         _EX_T("The current vendor URI does not equal the vendor URI given by a previous call."),
         _EX_T("OpenGPS::ISO5436_2Container::AppendVendorSpecific"));
   }

   if(m_VendorURI.empty())
   {
      m_VendorURI = vendorURI;
   }

   m_VendorSpecific.push_back(filePath);
}

OGPS_Boolean ISO5436_2Container::GetVendorSpecific(const OpenGPS::String& vendorURI, const OpenGPS::String& fileName, const OpenGPS::String& targetPath)
{
   if(!HasDocument())
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("The document does not exists."),
         _EX_T("An X3P archive must have been opened before vendorspecific data can be obtained."),
         _EX_T("OpenGPS::ISO5436_2Container::GetVendorSpecific"));
   }

   if(m_IsCreating)
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidOperation,
         _EX_T("This method call is invalid in the current context."),
         _EX_T("Vendorspecific data can be extracted only if an X3P archive is opened. But currently such an archive is to be created."),
         _EX_T("OpenGPS::ISO5436_2Container::GetVendorSpecific"));
   }

   if(vendorURI.empty())
   {
      throw OpenGPS::Exception(
         OGPS_ExInvalidArgument,
         _EX_T("The argument vendorURI is empty."),
         _EX_T("Supply an URI that is unique worldwide to identify your data format. Example: http://www.example-inc.com/myformat"),
         _EX_T("OpenGPS::ISO5436_2Container::GetVendorSpecific"));
   }

   if(m_Document->VendorSpecificID().present() && m_Document->VendorSpecificID().get() == vendorURI)
   {
      return Decompress(fileName, targetPath, TRUE);
   }

   return FALSE;
}

OGPS_Boolean ISO5436_2Container::WriteVendorSpecific(zipFile handle)
{
   OGPS_Boolean success = TRUE;

   if(m_VendorURI.size() > 0 && m_VendorSpecific.size() > 0)
   {
      m_Document->VendorSpecificID(m_VendorURI);

      for(size_t n = 0; n < m_VendorSpecific.size(); ++n)
      {
         // Creates new file in the zip container.
         OpenGPS::String vendor = m_VendorSpecific[n];
         OpenGPS::String avname = Environment::GetInstance()->GetFileName(vendor);
         if(zipOpenNewFileInZip(handle,
            avname.ToChar(),
            NULL,
            NULL,
            0,
            NULL,
            0,
            NULL,
            Z_DEFLATED,
            m_CompressionLevel) != ZIP_OK)
         {
            throw OpenGPS::Exception(
               OGPS_ExWarning,
               _EX_T("Could not write additional vendor specific data to the X3P archive."),
               _EX_T("Zlib could not open the target handle. Check for filesystem permissions and enough space left."),
               _EX_T("OpenGPS::ISO5436_2Container::WriteVendorSpecific"));
         }

         try
         {
            ZipStreamBuffer vbuffer(handle, FALSE);
            ZipOutputStream vstream(vbuffer);

            std::ifstream src(vendor.ToChar(), std::ios::in | std::ios::binary | std::ios::ate);

            if(!src.is_open())
            {
               success = FALSE;
            }
            else
            {
               // get length of file:
               int length = src.tellg();
               src.seekg(0, std::ios::beg);

               if(length < 0)
               {
                  throw OpenGPS::Exception(
                     OGPS_ExGeneral,
                     _EX_T("Could not write additional vendor specific data to the X3P archive."),
                     _EX_T("The size of the source file could not be determined."),
                     _EX_T("OpenGPS::ISO5436_2Container::WriteVendorSpecific"));
               }

               if(length > 0)
               {
                  do
                  {
                     const int chunk = min(_OPENGPS_ZIP_CHUNK_MAX, length);
                     voidp buffer = (voidp)malloc(chunk);

                     if(buffer)
                     {
                        try
                        {
                           src.read((OpenGPS::BytePtr)buffer, chunk);

                           if(src.fail())
                           {
                              if(!src.eof())
                              {
                                 throw OpenGPS::Exception(
                                    OGPS_ExGeneral,
                                    _EX_T("Could not write additional vendor specific data to the X3P archive."),
                                    _EX_T("The vendorspecific file to be added could not be read completely."),
                                    _EX_T("OpenGPS::ISO5436_2Container::WriteVendorSpecific"));
                              }
                           }

                           if(zipWriteInFileInZip(handle, (voidp)buffer, chunk) != ZIP_OK)
                           {
                              throw OpenGPS::Exception(
                                 OGPS_ExInvalidOperation,
                                 _EX_T("Could not write additional vendor specific data to the X3P archive."),
                                 _EX_T("Zlib could write all external data. Please check that there is enough space left on the device."),
                                 _EX_T("OpenGPS::ISO5436_2Container::WriteVendorSpecific"));
                           }
                           _OPENGPS_FREE(buffer);
                        }
                        catch(...)
                        {
                           _OPENGPS_FREE(buffer);
                           throw;
                        }
                     }
                     else
                     {
                        throw OpenGPS::Exception(
                           OGPS_ExGeneral,
                           _EX_T("Could not allocate memory via malloc."),
                           _EX_T("Verify that there is enough free virtual memory installed."),
                           _EX_T("OpenGPS::ISO5436_2Container::WriteVendorSpecific"));
                     }

                     length -= chunk;
                  } while(!src.eof() && length > 0);
               }
            }            
         }
         catch(...)
         {
            _VERIFY(zipCloseFileInZip(handle), ZIP_OK);
            throw;
         }

         _VERIFY(zipCloseFileInZip(handle), ZIP_OK);
      }
   }
   else
   {
      if(m_Document->VendorSpecificID().present())
      {
         m_Document->VendorSpecificID().reset();
      }
   }

   return success;
}
