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
 *   openGPS and the openGPS logo is a registered trademark of             *
 *   Physikalisch Technische Bundesanstalt (PTB)                           *
 *   http://www.ptb.de/                                                    *
 *                                                                         *
 *   More information about openGPS can be found at                        *
 *   http://www.opengps.eu/                                                *
 ***************************************************************************/

#include <opengps/cxx/opengps.hxx>

#include <opengps/iso5436_2.h>
#include <opengps/cxx/iso5436_2.hxx>
#include <opengps/cxx/iso5436_2_handle.hxx>
#include <opengps/cxx/iso5436_2_xsd.hxx>
#include <opengps/cxx/point_iterator.hxx>
#include <opengps/cxx/point_vector.hxx>
#include <opengps/cxx/data_point.hxx>
#include <opengps/cxx/string.hxx>

#include <string>
#include <iostream>
#include <ostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <limits>

#include <tchar.h>

using namespace std;
using namespace OpenGPS::Schemas::ISO5436_2;

void simpleExample(OpenGPS::String fileName)
{
   /* Simple example where we have two incremental and one absolute axis (the z-axis). */

   /* Create RECORD1 */
   Record1Type::Revision_type revision(_T("ISO5436 - 2000"));
   Record1Type::FeatureType_type featureType(_T("SUR")); /* Can't find an enum, here. */

   Record1Type::Axes_type::CX_type::AxisType_type xaxisType(Record1Type::Axes_type::CX_type::AxisType_type::I); /* incremental */
   Record1Type::Axes_type::CX_type::DataType_type xdataType(Record1Type::Axes_type::CX_type::DataType_type::D); /* double */
   Record1Type::Axes_type::CX_type xaxis(xaxisType);
   xaxis.DataType(xdataType);

   /* Set increment and offset. */
   xaxis.Increment(1.60160000000000E-0002);
   xaxis.Offset(0.0);

   Record1Type::Axes_type::CY_type::AxisType_type yaxisType(Record1Type::Axes_type::CY_type::AxisType_type::I); /* incremental */
   Record1Type::Axes_type::CY_type::DataType_type ydataType(Record1Type::Axes_type::CY_type::DataType_type::D); /* double */
   Record1Type::Axes_type::CY_type yaxis(yaxisType);
   yaxis.DataType(ydataType);

   /* Set increment and offset. */
   yaxis.Increment(1.60160000000000E-0002);
   yaxis.Offset(0.0);

   Record1Type::Axes_type::CZ_type::AxisType_type zaxisType(Record1Type::Axes_type::CZ_type::AxisType_type::A); /* absolute */
   Record1Type::Axes_type::CZ_type::DataType_type zdataType(Record1Type::Axes_type::CZ_type::DataType_type::D); /* double */
   Record1Type::Axes_type::CZ_type zaxis(zaxisType);
   zaxis.DataType(zdataType);

   Record1Type::Axes_type axis(xaxis, yaxis, zaxis);

   Record1Type record1(revision, featureType, axis);

   /* Create RECORD2 */
   Record2Type::Date_type date(_T("2007-04-30T13:58:02.6+02:00"));

   Record2Type::Instrument_type::Manufacturer_type manufacturer(_T("NanoFocus AG"));
   Record2Type::Instrument_type::Model_type model(_T("µSurf X"));
   Record2Type::Instrument_type::Serial_type serial(_T("12345abc"));
   Record2Type::Instrument_type::Version_type version(_T("Software V1.0, Hardware V1.0"));
   Record2Type::Instrument_type instrument(manufacturer, model, serial, version);

   Record2Type::CalibrationDate_type calibrationDate(_T("2007-04-30T13:58:02.6+02:00"));

   Record2Type::ProbingSystem_type::Type_type type(Record2Type::ProbingSystem_type::Type_type::NonContacting);
   Record2Type::ProbingSystem_type::Identification_type id(_T("LensName,Setupname,..."));
   Record2Type::ProbingSystem_type probingSystem(type, id);

   Record2Type::Comment_type comment(_T("This is a user comment specific to this dataset."));

   Record2Type record2(date, instrument, calibrationDate, probingSystem);
   record2.Comment(comment);

   /* Create MATRIX */
   MatrixDimensionType matrix(1, 1, 2);


   /* Create ISO5436_2 container */
   OGPS_ISO5436_2Handle handle = ogps_CreateMatrixISO5436_2(fileName.c_str(), NULL, record1, record2, matrix, FALSE);

   /* Add data points */
   /* 1. Create point vector buffer for three points. */
   OGPS_PointVectorPtr vector = ogps_CreatePointVector();

   /* Matrix indices. */
   unsigned long u = 0;
   unsigned long v = 0;
   unsigned long w = 0;

   /* 2. Since the z axis is of absolute type and the
   * other two are incremental, we simply set up just z
   * values and leave x an y values untouched (missing). */
   ogps_SetDoubleZ(vector, 4.86219120804151E-0001);

   /* 3. Write into document */
   ogps_SetMatrixPoint(handle, u , v, w, vector);

   /* Loop all data points we want to add... */

   /* 2b/3b. We have missing point data, here. */
   ogps_SetMatrixPoint(handle, u , v, w + 1, NULL);

   /* The above will show up in xml like this: */
   /*
   * <DataList>
   * <Datum>4.86219120804151E-0001</Datum>
   *
   * <!-- missing data points are represented by an empty tag-->
   * <Datum/>
   *
   * <Datum>8.23683772970184E-0001</Datum>
   * </DataList>
   */

   /* Free buffer */
   ogps_FreePointVector(&vector);

   /* Create vendorspecific data. */
   OpenGPS::String vname(fileName.substr(0, fileName.find_last_of(_T("/\\")) + 1));
   vname.append(_T("vendor.tmp"));
   std::wofstream vendor_dat(vname.ToChar(), std::ios::trunc);
   vendor_dat << _T("Vendorspecific data.") << std::endl;
   vendor_dat.close();

   ogps_AppendVendorSpecific(handle, _T("http://www.example.com/format/version1"), vname.c_str());

   /* Finally: write container to disk. */
   ogps_WriteISO5436_2(handle);
   ogps_CloseISO5436_2(&handle);
}


void mediumComplexExample(OpenGPS::String fileName)
{
   /* More complex example where we have three absolute axis. */

   /* Create RECORD1 */
   Record1Type::Revision_type revision(_T("ISO5436 - 2000"));
   Record1Type::FeatureType_type featureType(_T("SUR")); /* Can't find an enum, here? */

   Record1Type::Axes_type::CX_type::AxisType_type xaxisType(Record1Type::Axes_type::CX_type::AxisType_type::A); /* absolute */
   Record1Type::Axes_type::CX_type::DataType_type xdataType(Record1Type::Axes_type::CX_type::DataType_type::L); /* int32 */
   Record1Type::Axes_type::CX_type xaxis(xaxisType);
   xaxis.DataType(xdataType);

   Record1Type::Axes_type::CY_type::AxisType_type yaxisType(Record1Type::Axes_type::CY_type::AxisType_type::A); /* absolute */
   Record1Type::Axes_type::CY_type::DataType_type ydataType(Record1Type::Axes_type::CY_type::DataType_type::F); /* float */
   Record1Type::Axes_type::CX_type yaxis(xaxisType);
   yaxis.DataType(ydataType);

   Record1Type::Axes_type::CZ_type::AxisType_type zaxisType(Record1Type::Axes_type::CZ_type::AxisType_type::A); /* absolute */
   Record1Type::Axes_type::CZ_type::DataType_type zdataType(Record1Type::Axes_type::CZ_type::DataType_type::D); /* double */
   Record1Type::Axes_type::CX_type zaxis(xaxisType);
   zaxis.DataType(zdataType);

   Record1Type::Axes_type axis(xaxis, yaxis, zaxis);

   Record1Type record1(revision, featureType, axis);

   /* Create RECORD2 */
   Record2Type::Date_type date(_T("2007-04-30T13:58:02.6+02:00"));

   Record2Type::Instrument_type::Manufacturer_type manufacturer(_T("NanoFocus AG"));
   Record2Type::Instrument_type::Model_type model(_T("µSurf X"));
   Record2Type::Instrument_type::Serial_type serial(_T("12345abc"));
   Record2Type::Instrument_type::Version_type version(_T("Software V1.0, Hardware V1.0"));
   Record2Type::Instrument_type instrument(manufacturer, model, serial, version);

   Record2Type::CalibrationDate_type calibrationDate(_T("2007-04-30T13:58:02.6+02:00"));

   Record2Type::ProbingSystem_type::Type_type type(Record2Type::ProbingSystem_type::Type_type::NonContacting);
   Record2Type::ProbingSystem_type::Identification_type id(_T("LensName,Setupname,..."));
   Record2Type::ProbingSystem_type probingSystem(type, id);

   Record2Type::Comment_type comment(_T("This is a user comment specific to this dataset."));

   Record2Type record2(date, instrument, calibrationDate, probingSystem);
   record2.Comment(comment);

   /* Create MATRIX */
   MatrixDimensionType matrix(4, 2, 2);

   /* Create ISO5436_2 container */
   OGPS_ISO5436_2Handle handle = ogps_CreateMatrixISO5436_2(fileName.c_str(), NULL, record1, record2, matrix, TRUE);

   /* Add data points */
   /* 1. Create point vector buffer for three points. */
   OGPS_PointVectorPtr vector = ogps_CreatePointVector();

   /* 2. There are three absolute axis, so we have to
   * provide point values for all three axis. Though, we
   * do care about their data type. */

   /* Use iterator to create points in this example. */
   OGPS_PointIteratorPtr iterator = ogps_CreateNextPointIterator(handle);

   /* Create new points (current point set should be empty - of course) */
   /* Loop all 16 data points we want to add... */
   while(ogps_MoveNextPoint(iterator))
   {
      /* Int not short! Although this wouldn't mess up
      * if we had forgotten "L": both data types are compatible,
      * and we would have accepted both (short and int) in
      * this special case here. */
      ogps_SetInt32X(vector, 4L);
      /* automatic type conversion occurs for double */
      ogps_SetFloatY(vector, 2.5F);
      /* Z has data type double */
      ogps_SetDoubleZ(vector, 4.8);

      /* 3. Write into document */  
      ogps_SetCurrentPoint(iterator, vector);

      if(ogps_HasError())
      {
         break;
      }
   }

   // Free iterator
   ogps_FreePointIterator(&iterator);

   /* The above will show up in xml like this: */
   /*
   * <DataList>
   * <Datum>4;2.5;4.8</Datum>
   * </DataList>
   */  

   /* Free buffer */
   ogps_FreePointVector(&vector);

   /* Finally: write container to disk. */
   ogps_WriteISO5436_2(handle);
   ogps_CloseISO5436_2(&handle);
}

void readonlyExample(OpenGPS::String fileName)
{
   /* We want to read in some file and read its point data. */

   /* Open the file, hopefully everything went well... */
   OGPS_ISO5436_2Handle handle = ogps_OpenISO5436_2(fileName.c_str(), NULL, TRUE);

   if(!handle)
      return;

   /* Is data list? / Is matrix? - don't care; we use point iterator. */

   /* Create point buffer. */
   OGPS_PointVectorPtr vector = ogps_CreatePointVector();

   /* Use iterator to create points in this example. */
   OGPS_PointIteratorPtr iterator = ogps_CreateNextPointIterator(handle);

   /* Matrix indices. */
   unsigned long u = 0;
   unsigned long v = 0;
   unsigned long w = 0;

   /* Iterate point data (ignoring the fact whether they were stored
    * in xml directly or in external binary file). */
   while(ogps_MoveNextPoint(iterator))
   {
      // Get coordinates of this point in the matrix
      ogps_GetMatrixPosition(iterator, &u, &v, &w);

      /* Get points at current position. */
      ogps_GetCurrentPoint(iterator, vector);

      if(ogps_HasError())
      {
         break;
      }

      /* Valid data point (not missing)?  */
      if(ogps_IsValidPoint(vector))
      {
         double x, y, z;
         ogps_GetXYZ(vector, &x, &y, &z);

         // Write point to console
         std::cout << ("U: ") << u << ("; V: ") << v << ("; W: ") << w << ("X: ") << x << ("; Y: ") << y << ("; Z: ") << z <<std::endl;
      }    
      else
      {
         std::cout << ("U: ") << u << ("; V: ") << v << ("; W: ") << w << (": Invalid point") <<std::endl;
      }

   }

   std::cout << std::endl;

   // Failed?
   assert(!ogps_HasNextPoint(iterator));

   /* Free iterator/buffer */
   ogps_FreePointIterator(&iterator);
   ogps_FreePointVector(&vector);

   /* Close file */
   ogps_CloseISO5436_2(&handle);
}

void readonlyExample2(OpenGPS::String fileName)
{
   /* We want to read in some file and read its point data. */
   /* This time reading real typed data points. */
   /* With type information obtained from xml document. */

   /* Open the file, hopefully everything went well... */
   OGPS_ISO5436_2Handle handle = ogps_OpenISO5436_2(fileName.c_str(), NULL, TRUE);

   if(!handle)
      return;

   /* Obtain handle to xml document. */
   ISO5436_2Type * const document = ogps_GetDocument(handle);

   /* Z axis data type must be present (even if it is an incremental axis). */
   _ASSERT(document->Record1().Axes().CZ().DataType().present());

   /* Is data list? / Is matrix? - don't care; we use point iterator. */

   /* Create point buffer. */
   OGPS_PointVectorPtr vector = ogps_CreatePointVector();

   /* Use iterator to create points in this example. */
   OGPS_PointIteratorPtr iterator = ogps_CreateNextPointIterator(handle);

   /* Iterate point data (ignoring if they were stored
   * in xml directly or in external binary file). */
   while(ogps_MoveNextPoint(iterator))
   {
      /* Get points at current position. */
      ogps_GetCurrentPoint(iterator, vector);

      if(ogps_HasError())
      {
         break;
      }
      
      /* Valid data point (not missing)?  */
      if(ogps_IsValidPoint(vector))
      {
         std::cout << "X; Y; Z = ";

         switch(ogps_GetPointTypeX(vector))
         {
        case OGPS_Int16PointType : {
           short xs = ogps_GetInt16X(vector);
           std::cout << xs;
                                   } break;
        case OGPS_Int32PointType : {
           int xi = ogps_GetInt32X(vector);
           std::cout << xi;
                                   } break;
        case OGPS_FloatPointType : {
           float xf = ogps_GetFloatX(vector);
           std::cout << xf;
                                   } break;
        case OGPS_DoublePointType : {
           double xd = ogps_GetDoubleX(vector);
           std::cout << xd;
                                    } break;
         }

         std::cout << "; ";

         switch(ogps_GetPointTypeY(vector))
         {
        case OGPS_Int16PointType : {
           short ys = ogps_GetInt16Y(vector);
           std::cout << ys;
                                   } break;
        case OGPS_Int32PointType : {
           int yi = ogps_GetInt32Y(vector);
           std::cout << yi;
                                   } break;
        case OGPS_FloatPointType : {
           float yf = ogps_GetFloatY(vector);
           std::cout << yf;
                                   } break;
        case OGPS_DoublePointType : {
           double yd = ogps_GetDoubleY(vector);
           std::cout << yd;
                                    } break;
         }

         std::cout << "; ";

         switch(ogps_GetPointTypeZ(vector))
         {
        case OGPS_Int16PointType : {
           short zs = ogps_GetInt16Z(vector);
           std::cout << zs;
                                   } break;
        case OGPS_Int32PointType : {
           int zi = ogps_GetInt32Z(vector);
           std::cout << zi;
                                   } break;
        case OGPS_FloatPointType : {
           float zf = ogps_GetFloatZ(vector);
           std::cout << zf;
                                   } break;
        case OGPS_DoublePointType : {
           double zd = ogps_GetDoubleZ(vector);
           std::cout << zd;
                                    } break;
         } 

         std::cout << std::endl;
      }
      else
      {
         std::cout << "X; Y; Z = MISSING" << std::endl;
      }
   }

   std::cout << std::endl;

   // Failed?
   assert(!ogps_HasNextPoint(iterator));

   /* Free iterator/buffer */
   ogps_FreePointIterator(&iterator);
   ogps_FreePointVector(&vector);

   /* Close file */
   ogps_CloseISO5436_2(&handle);
}

void readonlyExample3(OpenGPS::String fileName)
{
   /* We want to read in some file and read its point data. */
   /* Same as above but with C++ Interface. */

   /* Open the file, hopefully everything went well... */
   OpenGPS::ISO5436_2 iso5436_2(fileName);
   iso5436_2.Open(TRUE);

   /* Obtain handle to xml document. */
   const OpenGPS::Schemas::ISO5436_2::ISO5436_2Type* const document = iso5436_2.GetDocument();

   if(document)
   {

      /* Is data list? / Is matrix? - don't care; we use point iterator. */

      /* Use iterator to create points in this example. */
      OpenGPS::PointIteratorAutoPtr iterator = iso5436_2.CreateNextPointIterator();

      /* Iterate point data (ignoring if they were stored
      * in xml directly or in external binary file). */
      OpenGPS::PointVector vector;
      while(iterator->MoveNext())
      {
         /* Get points at current position. */
         iterator->GetCurrent(vector);

         std::cout << "X; Y; Z = ";

         /* Valid data point (not missing)?  */
         if(vector.IsValid())
         {
            switch(vector.GetX()->GetPointType()) {
        case OGPS_Int16PointType : {
           short xs;
           vector.GetX()->Get(&xs);
           std::cout << xs;
                                   } break;
        case OGPS_Int32PointType : {
           int xi;
           vector.GetX()->Get(&xi);
           std::cout << xi;
                                   } break;
        case OGPS_FloatPointType : {
           float xf;
           vector.GetX()->Get(&xf);
           std::cout << xf;
                                   } break;
        case OGPS_DoublePointType : {
           double xd;
           vector.GetX()->Get(&xd);
           std::cout << xd;
                                    } break;
        default: {
           /* incremental axis without data type */
                 } break;
            }

            std::cout << "; ";

            switch(vector.GetY()->GetPointType()) {
        case OGPS_Int16PointType : {
           short ys;
           vector.GetY()->Get(&ys);
           std::cout << ys;
                                   } break;
        case OGPS_Int32PointType : {
           int yi;
           vector.GetY()->Get(&yi);
           std::cout << yi;
                                   } break;
        case OGPS_FloatPointType : {
           float yf;
           vector.GetY()->Get(&yf);
           std::cout << yf;
                                   } break;
        case OGPS_DoublePointType : {
           double yd;
           vector.GetY()->Get(&yd);
           std::cout << yd;
                                    } break;
        default: {
           /* incremental axis without data type */
                 } break;
            }

            std::cout << "; ";

            switch(vector.GetZ()->GetPointType()) {
        case OGPS_Int16PointType : {
           short zs;
           vector.GetZ()->Get(&zs);
           std::cout << zs;
                                   } break;
        case OGPS_Int32PointType : {
           int zi;
           vector.GetZ()->Get(&zi);
           std::cout << zi;
                                   } break;
        case OGPS_FloatPointType : {
           float zf;
           vector.GetZ()->Get(&zf);
           std::cout << zf;
                                   } break;
        case OGPS_DoublePointType : {
           double zd;
           vector.GetZ()->Get(&zd);
           std::cout << zd;
                                    } break;
        default: {
           /* missing - should not happen. Z axis must be present. */
                 } break;
            }

            std::cout << std::endl;
         }
         else
         {
            std::cout << "X; Y; Z = MISSING" << std::endl;
         }
      }

      std::cout << std::endl;

      // Failed?
      assert(!iterator->HasNext());

   }

   /* Close file */
   iso5436_2.Close();
}

void readonlyExample4(OpenGPS::String fileName)
{
   /* We want to read in some file and read its point data. */

   /* Open the file, hopefully everything went well... */
   OpenGPS::ISO5436_2 iso5436_2(fileName);
   iso5436_2.Open(TRUE);
   if(!ogps_HasError())
   {
      /* Obtain handle to xml document. */
      const OpenGPS::Schemas::ISO5436_2::ISO5436_2Type* const document = iso5436_2.GetDocument();
      if(document)
      {
         /* Is data list? / Is matrix? */
         if(document->Record3().ListDimension().present())
         {
            _ASSERT(!document->Record3().MatrixDimension().present());

            OpenGPS::PointVector vector;

            // safe cast, since values greater than 32bit are not supported by the ISO5436-2 XML specification.
            const unsigned long maxIndex = (unsigned long)document->Record3().ListDimension().get();
            for(unsigned long index = 0; index < maxIndex; ++index)
            {
               iso5436_2.GetListPoint(index, vector);

               if(ogps_HasError())
               {
                  break;
               }

               iso5436_2.SetListPoint(index, vector);

               if(ogps_HasError())
               {
                  break;
               }

               if(vector.IsValid())
               {
                  double x, y, z;
                  vector.GetXYZ(&x, &y, &z);

                  std::cout << ("N: ") << index << ("X: ") << x << ("; Y: ") << y << ("; Z: ") << z << std::endl;
               }
            }
         }
      }
      iso5436_2.Close();
   }
}

void performanceInt16(OpenGPS::String fileName, unsigned long dimension, OGPS_Boolean binary)
{
   // Timer
   clock_t start = clock();

   /* Simple example where we have two incremental and one absolute axis (the z-axis). */

   /* Create RECORD1 */
   Record1Type::Revision_type revision(_T("ISO5436 - 2000"));
   Record1Type::FeatureType_type featureType(_T("SUR")); /* Can't find an enum, here. */

   Record1Type::Axes_type::CX_type::AxisType_type xaxisType(Record1Type::Axes_type::CX_type::AxisType_type::I); /* incremental */
   Record1Type::Axes_type::CX_type::DataType_type xdataType(Record1Type::Axes_type::CX_type::DataType_type::I); /* int16 */
   Record1Type::Axes_type::CX_type xaxis(xaxisType);
   xaxis.DataType(xdataType);

   Record1Type::Axes_type::CY_type::AxisType_type yaxisType(Record1Type::Axes_type::CY_type::AxisType_type::I); /* incremental */
   Record1Type::Axes_type::CY_type::DataType_type ydataType(Record1Type::Axes_type::CY_type::DataType_type::I); /* int16 */
   Record1Type::Axes_type::CY_type yaxis(yaxisType);
   yaxis.DataType(ydataType);

   Record1Type::Axes_type::CZ_type::AxisType_type zaxisType(Record1Type::Axes_type::CZ_type::AxisType_type::A); /* absolute */
   Record1Type::Axes_type::CZ_type::DataType_type zdataType(Record1Type::Axes_type::CZ_type::DataType_type::I); /* int16 */
   Record1Type::Axes_type::CZ_type zaxis(zaxisType);
   zaxis.DataType(zdataType);

   Record1Type::Axes_type axis(xaxis, yaxis, zaxis);

   Record1Type record1(revision, featureType, axis);

   /* Create RECORD2 */
   Record2Type::Date_type date(_T("2007-04-30T13:58:02.6+02:00"));

   Record2Type::Instrument_type::Manufacturer_type manufacturer(_T("NanoFocus AG"));
   Record2Type::Instrument_type::Model_type model(_T("µSurf X"));
   Record2Type::Instrument_type::Serial_type serial(_T("12345abc"));
   Record2Type::Instrument_type::Version_type version(_T("Software V1.0, Hardware V1.0"));
   Record2Type::Instrument_type instrument(manufacturer, model, serial, version);

   Record2Type::CalibrationDate_type calibrationDate(_T("2007-04-30T13:58:02.6+02:00"));

   Record2Type::ProbingSystem_type::Type_type type(Record2Type::ProbingSystem_type::Type_type::NonContacting);
   Record2Type::ProbingSystem_type::Identification_type id(_T("LensName,Setupname,..."));
   Record2Type::ProbingSystem_type probingSystem(type, id);

   Record2Type::Comment_type comment(_T("This is a user comment specific to this dataset."));

   Record2Type record2(date, instrument, calibrationDate, probingSystem);
   record2.Comment(comment);

   /* Create ISO5436_2 container */
   OGPS_ISO5436_2Handle handle = ogps_CreateListISO5436_2(fileName.c_str(), NULL, record1, record2, dimension, binary);

   /* Add data points */

   /* Create point vector buffer for three points. */
   OGPS_PointVectorPtr vector = ogps_CreatePointVector();

   /* Create/write random number. */
   srand((unsigned)time(0));

   for(unsigned long n = 0; n < dimension; ++n)
   {
      /* Generate random number */
      short random = (short)(rand() % std::numeric_limits<short>::max());
      ogps_SetInt16Z(vector, random);

      /* Write into document */
      ogps_SetListPoint(handle, n, vector);
   }

   /* Free buffer */
   ogps_FreePointVector(&vector);

   /* Finally: write container to disk. */
   ogps_WriteISO5436_2(handle);
   ogps_CloseISO5436_2(&handle);

   // Timer
   clock_t stop = clock();

   std::cout << std::endl << "Writing an X3P file containing " << dimension << " points in int16 " << (binary ? "binary" : "xml") << " format took " << ((stop - start)/CLOCKS_PER_SEC) << " seconds." << std::endl;
}

void performanceDouble(OpenGPS::String fileName, unsigned long dimension, OGPS_Boolean binary)
{
   // Timer
   clock_t start = clock();

   /* Simple example where we have two incremental and one absolute axis (the z-axis). */

   /* Create RECORD1 */
   Record1Type::Revision_type revision(_T("ISO5436 - 2000"));
   Record1Type::FeatureType_type featureType(_T("SUR")); /* Can't find an enum, here. */

   Record1Type::Axes_type::CX_type::AxisType_type xaxisType(Record1Type::Axes_type::CX_type::AxisType_type::I); /* incremental */
   Record1Type::Axes_type::CX_type::DataType_type xdataType(Record1Type::Axes_type::CX_type::DataType_type::I); /* int16 */
   Record1Type::Axes_type::CX_type xaxis(xaxisType);
   xaxis.DataType(xdataType);

   Record1Type::Axes_type::CY_type::AxisType_type yaxisType(Record1Type::Axes_type::CY_type::AxisType_type::I); /* incremental */
   Record1Type::Axes_type::CY_type::DataType_type ydataType(Record1Type::Axes_type::CY_type::DataType_type::I); /* int16 */
   Record1Type::Axes_type::CY_type yaxis(yaxisType);
   yaxis.DataType(ydataType);

   Record1Type::Axes_type::CZ_type::AxisType_type zaxisType(Record1Type::Axes_type::CZ_type::AxisType_type::A); /* absolute */
   Record1Type::Axes_type::CZ_type::DataType_type zdataType(Record1Type::Axes_type::CZ_type::DataType_type::D); /* double */
   Record1Type::Axes_type::CZ_type zaxis(zaxisType);
   zaxis.DataType(zdataType);

   Record1Type::Axes_type axis(xaxis, yaxis, zaxis);

   Record1Type record1(revision, featureType, axis);

   /* Create RECORD2 */
   Record2Type::Date_type date(_T("2007-04-30T13:58:02.6+02:00"));

   Record2Type::Instrument_type::Manufacturer_type manufacturer(_T("NanoFocus AG"));
   Record2Type::Instrument_type::Model_type model(_T("µSurf X"));
   Record2Type::Instrument_type::Serial_type serial(_T("12345abc"));
   Record2Type::Instrument_type::Version_type version(_T("Software V1.0, Hardware V1.0"));
   Record2Type::Instrument_type instrument(manufacturer, model, serial, version);

   Record2Type::CalibrationDate_type calibrationDate(_T("2007-04-30T13:58:02.6+02:00"));

   Record2Type::ProbingSystem_type::Type_type type(Record2Type::ProbingSystem_type::Type_type::NonContacting);
   Record2Type::ProbingSystem_type::Identification_type id(_T("LensName,Setupname,..."));
   Record2Type::ProbingSystem_type probingSystem(type, id);

   Record2Type::Comment_type comment(_T("This is a user comment specific to this dataset."));

   Record2Type record2(date, instrument, calibrationDate, probingSystem);
   record2.Comment(comment);

   /* Create ISO5436_2 container */
   OGPS_ISO5436_2Handle handle = ogps_CreateListISO5436_2(fileName.c_str(), NULL, record1, record2, dimension, binary);

   /* Add data points */

   /* Create point vector buffer for three points. */
   OGPS_PointVectorPtr vector = ogps_CreatePointVector();

   /* Create/write random number. */
   srand((unsigned)time(0));

   for(unsigned long n = 0; n < dimension; ++n)
   {
      /* Generate random number */
      double random = rand() * (std::numeric_limits<double>::max() / RAND_MAX);
      ogps_SetDoubleZ(vector, random);

      /* Write into document */
      ogps_SetListPoint(handle, n, vector);
   }

   /* Free buffer */
   ogps_FreePointVector(&vector);

   /* Finally: write container to disk. */
   ogps_WriteISO5436_2(handle);
   ogps_CloseISO5436_2(&handle);

   // Timer
   clock_t stop = clock();

   std::cout << std::endl << "Writing an X3P file containing " << dimension << " points in double " << (binary ? "binary" : "xml") << " format took " << ((stop - start)/CLOCKS_PER_SEC) << " seconds." << std::endl;
}

int _cdecl _tmain(int argc, _TCHAR* argv[])
{
   if(argc != 2)
   {
      std::cout << "Usage: ISO5436_2_XML_demo <full path to sample files>/" << std::endl << std::endl <<
         "Please specify the full path to the directory where the *.x3p sample files reside. The path should also contain the terminating directory separator. Ensure that you have write access to that path." << std::endl << std::endl <<
         "This simple demo program parses the sample files and prints its contents onto the console. Do not change the names of the sample files, since these are hard coded herein. The purpose of the demo is to get you familiar with the openGPS(R) API." << std::endl;
      return 1;
   }

   std::wstring path = argv[1];
   std::wstring tmp;
   
   tmp = path; tmp += _T("ISO5436-sample1.x3p");
   readonlyExample(tmp);

   tmp = path; tmp += _T("ISO5436-sample4_bin.x3p");
   readonlyExample2(tmp);

   tmp = path; tmp += _T("ISO5436-sample3.x3p");
   readonlyExample3(tmp);

   tmp = path; tmp += _T("ISO5436-sample2.x3p");
   readonlyExample4(tmp);

   tmp = path; tmp += _T("simple.x3p");
   simpleExample(tmp);

   tmp = path; tmp += _T("medium.x3p");
   mediumComplexExample(tmp);

   std::cout << std::endl << "Starting performance tests..." << std::endl;

   tmp = path; tmp += _T("performance_int16.x3p"); 
   performanceInt16(tmp, 1000000, FALSE);

   tmp = path; tmp += _T("performance_int16_bin.x3p"); 
   performanceInt16(tmp, 1000000, TRUE);

   tmp = path; tmp += _T("performance_double.x3p"); 
   performanceDouble(tmp, 1000000, FALSE);

   tmp = path; tmp += _T("performance_double_bin.x3p"); 
   performanceDouble(tmp, 1000000, TRUE);

   return 0;
}
