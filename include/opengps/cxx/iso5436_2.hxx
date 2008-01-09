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

#ifndef _OPENGPS_CXX_ISO5436_2_HXX
#define _OPENGPS_CXX_ISO5436_2_HXX

#ifndef _OPENGPS_CXX_OPENGPS_HXX
#  include <opengps/cxx/opengps.hxx>
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
         const OpenGPS::String& file,
         const OpenGPS::String& temp);

      ISO5436_2(
         const OpenGPS::String& file);

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

      virtual ~ISO5436_2();

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

      ISO5436_2& operator=(const ISO5436_2& src);

   private:
      const OGPS_Boolean m_IsProtected;

      /* Internal instance. Either this pointer or ISO5436_2Container instance. */
      ISO5436_2* m_Instance;
   };
}


#endif /* _OPENGPS_CXX_ISO5436_2_HXX */
