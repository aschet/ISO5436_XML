/***************************************************************************
 *   Copyright by Georg Wiora (NanoFocus AG) 2008                          *
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

// Mex file to import X3P files to matlab
// 30-Nov-2008

// Preconditions:
//   ISO5436_2_XML.dll and lib must be available
//   xerces-c_2_7_vc80.dll and xerces 3.0 headers from CodeSynthesis must be installed.

// Compile from Matlab with 
// mex -L getenv('OPENGPS_LOCATION') -I [getenv(OPENGPS_LOCATION),'/../include/'] openX3P.cpp
// Replace OPENGPS_LOCATION with your installation location
// mex -g -L"S:/openGPS/ISO5436_XML/install/lib/" -L"S:/openGPS/ISO5436_XML/install/bin/" -l"ISO5436_2_XML_S" -I"S:/openGPS/ISO5436_XML/install/include/" -I"C:\Programme\CodeSynthesis XSD 3.0\include"  COMPFLAGS="$COMPFLAGS /Zc:wchar_t" openX3P.cpp
// For debugging use:
// mex -g -L"S:/openGPS/ISO5436_XML/install/lib/" -L"S:/openGPS/ISO5436_XML/install/bin/" -l"ISO5436_2_XML_S" -I"S:/openGPS/ISO5436_XML/install/include/" -I"C:\Programme\CodeSynthesis XSD 3.0\include"  COMPFLAGS="$COMPFLAGS /Zc:wchar_t" openX3P.cpp

// We want to link to the iso5436_2.dll
#define SHARED_OPENGPS_LIBRARY
#define _UNICODE
#define UNICODE

#include "X3PUtilities.h"

#include <tchar.h>
#include "mex.h"
#include <strstream>
#include <limits>
#include <opengps/cxx/opengps.hxx>
#include <opengps/iso5436_2.h>
#include <opengps/cxx/iso5436_2.hxx>
#include <opengps/cxx/iso5436_2_handle.hxx>
#include <opengps/cxx/iso5436_2_xsd.hxx>
#include <opengps/cxx/point_iterator.hxx>
#include <opengps/cxx/point_vector.hxx>
#include <opengps/cxx/data_point.hxx>
#include <opengps/cxx/string.hxx>
#include <opengps/cxx/info.hxx>

using namespace std;
using namespace OpenGPS::Schemas::ISO5436_2;

/* The gateway function */
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
  bool bHasStruct=false;
  
  const wstring SyntaxHelp( 
     L"Call Syntax:\n"
     L"  [x,y,z,meta] = openX3P(filename)\n"
     L"    x     - Array of x coordinates in meter\n"
     L"    y     - Array of y coordinates in meter\n"
     L"    z     - Array of z coordinates in meter\n"
     L"    pinfo - Info about data organisation\n"
     L"    meta  - Meta data structure of the file\n"
     L"  [z, x, y] = openX3P(filename)\n"
     L"  [z, x, y, pinfo] = openX3P(filename)\n"
     L"  [z, x, y, pinfo, meta] = openX3P(filename)\n");
  char FileName[1024];              /* filename */
  mxArray *outMatrixX=NULL;               /*output matrix with x-Values*/
  mxArray *outMatrixY=NULL;               /*output matrix with y-Values*/
  mxArray *outMatrixZ=NULL;               /*output matrix with z-Values*/

  /* check for proper number of arguments */
  if(nrhs!=1)
  {
    ostrstream msg;
    msg << "openX3P was called with " << nrhs << " input arguments!" << endl
        << SyntaxHelp << ends;
    mexErrMsgIdAndTxt("openGPS:openX3P:nrhs",msg.str());
  }
  // Check number of output arguments
  if((nlhs<3) || (nlhs>5))
  {
    ostrstream msg;
    msg << "openX3P was called with " << nlhs << " output arguments!" << endl
        << SyntaxHelp << ends;
    mexErrMsgIdAndTxt("openGPS:openX3P:nlhs",msg.str());
  }
  
  // Check for metadata requested
  bool hasMeta=false;
  if (nlhs == 5)
    hasMeta=true;
  
  // Check for point info structure requested
  bool hasPointInfo=false;
  if (nlhs >= 4)
    hasPointInfo=true;

  /* make sure the first input argument is string */
  if( !mxIsChar(prhs[0]) || 
       mxGetNumberOfElements(prhs[0])<1 ) {
       mexErrMsgIdAndTxt("openGPS:openX3P:notString","Input must be a file name string");
  }

  /* get the filename  */
  if (mxGetString(prhs[0], FileName, sizeof(FileName)-1))
  {
    mexErrMsgIdAndTxt("openGPS:openX3P:notString","Filename is too long.");
  }

  // Convert filename to wide character
  _TCHAR FileNameL[1024];
  for (unsigned int i=0; i<1024 ; i++)
    FileNameL[i] = FileName[i];
  
  /* We want to read in some file and read its point data. */
  /* Open the file, hopefully everything went well... */
  OGPS_ISO5436_2Handle handle = ogps_OpenISO5436_2(FileNameL, NULL, TRUE);

  // Check for error
  if (ogps_HasError())
  {
    // Print full description
    ostrstream msg;
    msg << "Error opening X3P-file name \"" << string(FileName) << "\"!" << endl
        << wstring(ogps_GetErrorMessage()) << endl
        << wstring(ogps_GetErrorDescription()) << endl
        << ends;
    mexErrMsgIdAndTxt("openGPS:openX3P:FileOpen",msg.str());
    return;
  }

   if(!handle)
      return;
  
   /* Is data list? / Is matrix? */ 
   // Number of points in file
   unsigned long npoints=0;
   if (ogps_IsMatrix(handle))
   {
     unsigned long su=0, sv=0, sw=0;
     // Get matrix dimensions and allocate destination arrays
     ogps_GetMatrixDimensions(handle,&su, &sv, &sw);
     npoints = su*sv*sw;
     
     // Collect dimensions in array
     mwSize dims[3];
     dims[0]=su;
     dims[1]=sv;
     dims[2]=sw;
     
     /* create the output matrix for u and v vector*/
     outMatrixX = mxCreateNumericArray(3, dims,mxDOUBLE_CLASS,mxREAL);
     outMatrixY = mxCreateNumericArray(3, dims,mxDOUBLE_CLASS,mxREAL);
     outMatrixZ = mxCreateNumericArray(3, dims,mxDOUBLE_CLASS,mxREAL);
   }
   else
   {
     // Get List size and allocate destination array
     npoints = ogps_GetListDimension(handle);
     // allocate destination arrays
     outMatrixX = mxCreateDoubleMatrix(npoints,1,mxREAL);
     outMatrixY = mxCreateDoubleMatrix(npoints,1,mxREAL);
     outMatrixZ = mxCreateDoubleMatrix(npoints,1,mxREAL);
   }
   
   // assign matrices to output array
   plhs[0] = outMatrixZ;
   plhs[1] = outMatrixX;
   plhs[2] = outMatrixY;
   
   // Get pointer to data arrays
   double *ptrX=NULL,*ptrY=NULL, *ptrZ=NULL;
   ptrX = (double*)mxGetData(outMatrixX);
   ptrY = (double*)mxGetData(outMatrixY);
   ptrZ = (double*)mxGetData(outMatrixZ);
   
   /* Create point buffer. */
   OGPS_PointVectorPtr vector = ogps_CreatePointVector();

   /* Use iterator to create points in this example. */
   OGPS_PointIteratorPtr iterator = ogps_CreateNextPointIterator(handle);

   /* Iterate point data (ignoring the fact whether they were stored
    * in xml directly or in external binary file). */
   for (unsigned int i=0; i<npoints ; i++)
   {
     // goto next point
     ogps_MoveNextPoint(iterator);
      /* Get point at current position without any transformation. */
      // ogps_GetCurrentPoint(iterator, vector);
      /* Get coordinates of current point transformed to original coordinate space */
      ogps_GetCurrentCoord(iterator, vector);

      if(ogps_HasError())
      {
         break;
      }

      /* Valid data point (not missing)?  */
      if(ogps_IsValidPoint(vector))
      {
         double x, y, z;
         ogps_GetXYZ(vector, &x, &y, &z);
         
         // Write data point to array
         *(ptrX++) = x;
         *(ptrY++) = y;
         *(ptrZ++) = z;
      }    
      else
      {
         // Write invalid point to array
         *(ptrX++) = numeric_limits<double>::quiet_NaN();
         *(ptrY++) = numeric_limits<double>::quiet_NaN();
         *(ptrZ++) = numeric_limits<double>::quiet_NaN();
      }

   }

   // Failed?
   if(ogps_HasNextPoint(iterator))
   {
     // Print full description
     ostrstream msg;
     msg << "File \"" << string(FileName) << "\" was expected to contain exactly "
         << npoints << " data points, but it seems to have more!" << endl
         << ends;
     mexErrMsgIdAndTxt("openGPS:openX3P:FileRead",msg.str());
   }

   /* Free iterator/buffer */
   ogps_FreePointIterator(&iterator);
   ogps_FreePointVector(&vector);

  // get point info 
  if (hasPointInfo)
  {
    // Get document 
    const ISO5436_2Type* const document = ogps_GetDocument(handle);
    if (ogps_HasError())
    {
      ostrstream msg;
      msg << "Could not access XML document in file \"" << string(FileName) << "\"!"
          << endl << ends;
      mexErrMsgIdAndTxt("openGPS:openX3P:XMLDocument",msg.str());
    }
    // Get a reference to record 1
    const OpenGPS::Schemas::ISO5436_2::ISO5436_2Type::Record1_type &r1 = document->Record1();
    
    // Create point info structure
    // Number of structure elements
    const unsigned int nelem=4;
    // Field names
    const char *fieldnames[nelem] = {"Revision","FeatureType","IsMatrix","IsList"};
    // Create the structure
    plhs[3] = mxCreateStructMatrix(1, 1, nelem, &(fieldnames[0]));
    
    // Get file format revision
    mxSetField(plhs[3], 0, "Revision",  ConvertWtoMStr(r1.Revision()));

    // Get Feature type "PRF", "SUR", "PCL"
    mxSetField(plhs[3], 0, "FeatureType",  ConvertWtoMStr(r1.FeatureType()));

    // Check for matrix organisation
    mxSetField(plhs[3], 0, "IsMatrix",  mxCreateLogicalScalar(ogps_IsMatrix(handle)));
    // Check for list organisation
    mxSetField(plhs[3], 0, "IsList",  mxCreateLogicalScalar(ogps_IsMatrix(handle) ? false:true));
  }
   
  // Collect meta data
  if (hasMeta)
  {
    // Get document 
    const ISO5436_2Type* const document = ogps_GetDocument(handle);
    if (ogps_HasError())
    {
      ostrstream msg;
      msg << "Could not access XML document in file \"" << string(FileName) << "\"!"
          << endl << ends;
      mexErrMsgIdAndTxt("openGPS:openX3P:XMLDocument",msg.str());
    }
    // Get a reference to optional record 2
    const OpenGPS::Schemas::ISO5436_2::ISO5436_2Type::Record2_optional &r2opt = document->Record2();

    // Check for presence of meta data
    if (r2opt.present() == FALSE)
    {
      ostrstream msg;
      msg << "File \"" << string(FileName) << "\" does not contain meta data Record2!"
          << endl << ends;
      mexWarnMsgIdAndTxt("openGPS:openX3P:XMLDocument:Record2",msg.str());
      return;
    }

    // Number of structure elements
    const unsigned int nelem=10;
    // Field names
    const char *fieldnames[nelem] = {"Date","Creator","Instrument_Manufacturer",
        "Instrument_Model","Instrument_Serial","Instrument_Version",
        "CalibrationDate","ProbingSystem_Type","ProbingSystem_Identification","Comment"};
    // Create the structure
    plhs[4] = mxCreateStructMatrix(1, 1, nelem, &(fieldnames[0]));

    // Assign Structure elements
    // Get the Record2
    const OpenGPS::Schemas::ISO5436_2::ISO5436_2Type::Record2_type &r2 = r2opt.get();

    // Data set creation date
    mxSetField(plhs[4], 0, "Date", ConvertWtoMStr(r2.Date()));

    // Check for data set creator
    if (r2.Creator().present())
    {
      if (r2.Creator().get().length() < 1)
      { 
        mexWarnMsgIdAndTxt("openGPS:openX3P:XMLDocument:Record2:Creator",
                "Data set creator name is present, but empty!");
        mxSetField(plhs[4], 0, "Creator", mxCreateString("No data set creator specified"));
      }
      else
      {
        mxSetField(plhs[4], 0, "Creator", ConvertWtoMStr(r2.Creator().get()));
      }
    }
    else
      mexWarnMsgIdAndTxt("openGPS:openX3P:XMLDocument:Record2:Creator",
              "Data set was created anonymously.");
 
    // Measuring instrument
    mxSetField(plhs[4], 0, "Instrument_Manufacturer", ConvertWtoMStr(r2.Instrument().Manufacturer()));
    mxSetField(plhs[4], 0, "Instrument_Model", ConvertWtoMStr(r2.Instrument().Model()));
    mxSetField(plhs[4], 0, "Instrument_Serial", ConvertWtoMStr(r2.Instrument().Serial()));    
    mxSetField(plhs[4], 0, "Instrument_Version", ConvertWtoMStr(r2.Instrument().Version()));    

    // Calibration
    mxSetField(plhs[4], 0, "CalibrationDate", ConvertWtoMStr(r2.CalibrationDate()));    

    // Probing system type
    mxSetField(plhs[4], 0, "ProbingSystem_Type", ConvertWtoMStr( r2.ProbingSystem().Type()));    
    mxSetField(plhs[4], 0, "ProbingSystem_Identification", ConvertWtoMStr(r2.ProbingSystem().Identification()));    

    // Extract and print information
    if (r2.Comment().present())
    {
      if (r2.Comment().get().length() < 1)
      { 
        mexWarnMsgIdAndTxt("openGPS:openX3P:XMLDocument:Record2:Comment",
                "Data set has an empty comment!");
      }
      else
      {
        mxSetField(plhs[4], 0, "Comment", ConvertWtoMStr(r2.Comment().get()));
      }
    }
    else
      mexWarnMsgIdAndTxt("openGPS:openX3P:XMLDocument:Record2:Comment",
              "Data set contains no comment.");
   
  } // End of hasMeta
   
  /* Close file */
  ogps_CloseISO5436_2(&handle);
}
