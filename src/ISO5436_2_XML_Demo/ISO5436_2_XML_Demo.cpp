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


// ISO5436_2_XML_Demo.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include <opengps/cxx/opengps.hxx>

#undef _OPENGPS_EXPORT
#define _OPENGPS_EXPORT

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

#include <tchar.h>

using namespace std;
using namespace OpenGPS::Schemas::ISO5436_2;

void simpleExample(OpenGPS::String fileName) {
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
  MatrixDimensionType matrix(4, 4, 1);
  
  /* Create ISO5436_2 container */
  OGPS_ISO5436_2Handle handle = ogps_CreateMatrixISO5436_2(fileName.c_str()/*_T("ISO5436_2-sample1.x3p")*/, NULL, record1, record2, matrix, FALSE);
  
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
  
  /* Loop all 16 data points we want to add... */
  
  /* 2b/3b. We have missing point data, here. */
  ogps_SetMatrixPoint(handle, u , v, w, NULL);
  
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
  
  /* Finally: write container to disk. */
  ogps_WriteISO5436_2(handle);
  ogps_CloseISO5436_2(&handle);
}


void mediumComplexExample(OpenGPS::String fileName) {
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
  OGPS_ISO5436_2Handle handle = ogps_CreateMatrixISO5436_2(fileName.c_str()/*_T("ISO5436_2-sample5_bin.x3p")*/, NULL, record1, record2, matrix, TRUE);
  
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
  // TODO: while(ogps_CreateNextPoint(iterator))
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
    if(!ogps_SetCurrentPoint(iterator, vector))
    {
      // Error!
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
   OGPS_ISO5436_2Handle handle = ogps_OpenISO5436_2(fileName.c_str()/*_T("ISO5436_2-sample1.x3p")*/, NULL, TRUE);

   if(!handle)
      return;

   /* Obtain handle to xml document. */
   ISO5436_2Type * document = ogps_GetDocument(handle);

   /* Is data list? / Is matrix? - don't care; we use point iterator. */

   /* Create point buffer. */
   OGPS_PointVectorPtr vector = ogps_CreatePointVector();

   /* Use iterator to create points in this example. */
   OGPS_PointIteratorPtr iterator = ogps_CreateNextPointIterator(handle);

   /* Matrix indices. */
   unsigned long u = 0;
   unsigned long v = 0;
   unsigned long w = 0;

   /* Iterate point data (ignoring if they were stored
   * in xml directly or in external binary file). */
   while(ogps_MoveNextPoint(iterator))
   {
      // Get coordinates of this point in the matrix
      ogps_GetMatrixPosition(iterator, &u, &v, &w);

      /* Get points at current position. */
      if(!ogps_GetCurrentPoint(iterator, vector))
      {
         // Error!
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

   // Failed?
   assert(!ogps_HasNextPoint(iterator));

   /* Free iterator/buffer */
   ogps_FreePointIterator(&iterator);
   ogps_FreePointVector(&vector);

   /* Close file */
   ogps_CloseISO5436_2(&handle);
}

void readonlyExample2(OpenGPS::String fileName) {
  /* We want to read in some file and read its point data. */
  /* This time reading real typed data points. */
  /* With type information obtained from xml document. */
  
  /* Open the file, hopefully everything went well... */
  OGPS_ISO5436_2Handle handle = ogps_OpenISO5436_2(fileName.c_str()/*_T("ISO5436_2-sample4_bin.x3p")*/, NULL, TRUE);

  if(!handle)
     return;
  
  /* Obtain handle to xml document. */
  ISO5436_2Type * document = ogps_GetDocument(handle);

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
    if(!ogps_GetCurrentPoint(iterator, vector))
    {
      // Error!
      break;
    }
      
    /* Valid data point (not missing)?  */
    if(ogps_IsValidPoint(vector))
    {
       if(document->Record1().Axes().CX().DataType().present())
       {
      switch(document->Record1().Axes().CX().DataType().get()) {
        case DataType::I : {
            short xs = ogps_GetInt16X(vector);
          } break;
          case DataType::L : {
            int xi = ogps_GetInt32X(vector);
          } break;
          case DataType::F : {
            float xf = ogps_GetFloatX(vector);
          } break;
          case DataType::D : {
            double xd = ogps_GetDoubleX(vector);
          } break;
        }
       }
      
       if(document->Record1().Axes().CY().DataType().present())
       {
      switch(document->Record1().Axes().CY().DataType().get()) {
        case DataType::I : {
            short ys = ogps_GetInt16Y(vector);
          } break;
          case DataType::L : {
            int yi = ogps_GetInt32Y(vector);
          } break;
          case DataType::F : {
            float yf = ogps_GetFloatY(vector);
          } break;
          case DataType::D : {
            double yd = ogps_GetDoubleY(vector);
          } break;
        }
       }

      switch(document->Record1().Axes().CZ().DataType().get()) {
        case DataType::I : {
            short zs = ogps_GetInt16Z(vector);
          } break;
          case DataType::L : {
            int zi = ogps_GetInt32Z(vector);
          } break;
          case DataType::F : {
            float zf = ogps_GetFloatZ(vector);
          } break;
          case DataType::D : {
            double zd = ogps_GetDoubleZ(vector);
          } break;
        }
    }    
  }
    
  // Failed?
  assert(!ogps_HasNextPoint(iterator));
    
  /* Free iterator/buffer */
  ogps_FreePointIterator(&iterator);
  ogps_FreePointVector(&vector);

  /* Close file */
  ogps_CloseISO5436_2(&handle);
}

void readonlyExample3(OpenGPS::String fileName) {
  /* We want to read in some file and read its point data. */
  /* Same as above but with C++ Interface. */
  
  /* Open the file, hopefully everything went well... */
   OpenGPS::ISO5436_2 iso5436_2(fileName);
   iso5436_2.Open(TRUE);

  /* Obtain handle to xml document. */
   const OpenGPS::ISO5436_2TypeAutoPtr& document = iso5436_2.GetDocument();

   if(document.get())
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

    /* Valid data point (not missing)?  */
    if(vector.IsValid())
    {
      switch(vector.GetX()->GetType()) {
        case OGPS_Int16PointType : {
            short xs;
            vector.GetX()->Get(&xs);
          } break;
          case OGPS_Int32PointType : {
            int xi;
            vector.GetX()->Get(&xi);
          } break;
          case OGPS_FloatPointType : {
            float xf;
            vector.GetX()->Get(&xf);
          } break;
          case OGPS_DoublePointType : {
            double xd;
            vector.GetX()->Get(&xd);
          } break;
          default: {
            /* incremental axis without data type */
          } break;
        }
      
      switch(vector.GetY()->GetType()) {
        case OGPS_Int16PointType : {
            short ys;
            vector.GetX()->Get(&ys);
          } break;
          case OGPS_Int32PointType : {
            int yi;
            vector.GetX()->Get(&yi);
          } break;
          case OGPS_FloatPointType : {
            float yf;
            vector.GetX()->Get(&yf);
          } break;
          case OGPS_DoublePointType : {
            double yd;
            vector.GetX()->Get(&yd);
          } break;
          default: {
            /* incremental axis without data type */
          } break;
        }
      
      switch(vector.GetZ()->GetType()) {
        case OGPS_Int16PointType : {
            short zs;
            vector.GetX()->Get(&zs);
          } break;
          case OGPS_Int32PointType : {
            int zi;
            vector.GetX()->Get(&zi);
          } break;
          case OGPS_FloatPointType : {
            float zf;
            vector.GetX()->Get(&zf);
          } break;
          case OGPS_DoublePointType : {
            double zd;
            vector.GetX()->Get(&zd);
          } break;
          default: {
            /* missing - should not happen. Z axis must be present. */
          } break;
        }
    }    
  }
    
  // Failed?
  assert(!iterator->HasNext());

   }
    
  /* Close file */
  iso5436_2.Close();
}

void readonlyExample4(OpenGPS::String fileName) {
   /* We want to read in some file and read its point data. */
    
   /* Open the file, hopefully everything went well... */
   OpenGPS::ISO5436_2 iso5436_2(fileName);
   if(iso5436_2.Open(TRUE))
   {
      /* Obtain handle to xml document. */
      const OpenGPS::ISO5436_2TypeAutoPtr& document = iso5436_2.GetDocument();
      if(document.get())
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
               if(!iso5436_2.GetListPoint(index, vector))
               {
                  // Error!
                  break;
               }

               if(!iso5436_2.SetListPoint(index, vector))
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

int _cdecl _tmain(int argc, _TCHAR* argv[])
{
   //simpleExample(_T("H:\\Profile\\jo\\openGPS\\test\\ISO5436-sample1.x3p"));
   //mediumComplexExample(_T("H:\\Profile\\jo\\openGPS\\test\\ISO5436-sample5_bin.x3p"));   
   /* readonlyExample(_T("H:\\Profile\\jo\\openGPS\\test\\ISO5436-sample1_bin.x3p"));
   readonlyExample2(_T("H:\\Profile\\jo\\openGPS\\test\\ISO5436-sample3.x3p"));
   readonlyExample3(_T("H:\\Profile\\jo\\openGPS\\test\\ISO5436-sample3.x3p"));
   readonlyExample4(_T("H:\\Profile\\jo\\openGPS\\test\\ISO5436-sample2.x3p"));
   */

   /*
   readonlyExample(_T("..\\SampleFiles\\ISO5436-sample1_bin.x3p"));
   readonlyExample2(_T("..\\SampleFiles\\ISO5436-sample3.x3p"));
   readonlyExample3(_T("..\\SampleFiles\\ISO5436-sample3.x3p"));
   readonlyExample4(_T("..\\SampleFiles\\ISO5436-sample2.x3p"));
   */

   readonlyExample(_T("S:\\openGPS\\ISO5436_XML\\install\\SampleFiles\\ISO5436-sample1_bin.x3p"));
   readonlyExample2(_T("S:\\openGPS\\ISO5436_XML\\install\\SampleFiles\\ISO5436-sample3.x3p"));
   readonlyExample3(_T("S:\\openGPS\\ISO5436_XML\\install\\SampleFiles\\ISO5436-sample3.x3p"));
   readonlyExample4(_T("S:\\openGPS\\ISO5436_XML\\install\\SampleFiles\\ISO5436-sample2.x3p"));
	return 0;
}
