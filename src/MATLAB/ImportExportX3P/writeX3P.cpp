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

// Mex file to write X3P files to matlab
// 07-Dec-2008

// Preconditions:
//   ISO5436_2_XML.dll and lib must be available
//   xerces-c_2_7_vc80.dll and xerces 3.0 headers from CodeSynthesis must be installed.

// Compile from Matlab with 
// mex -L getenv('OPENGPS_LOCATION') -I [getenv(OPENGPS_LOCATION),'/../include/'] writeX3P.cpp
// Replace OPENGPS_LOCATION with your installation location
// mex -L"S:/openGPS/ISO5436_XML/install/lib/" -L"S:/openGPS/ISO5436_XML/install/bin/" -l"ISO5436_2_XML_S" -I"S:/openGPS/ISO5436_XML/install/include/" -I"C:\Programme\CodeSynthesis XSD 3.0\include"  COMPFLAGS="$COMPFLAGS /Zc:wchar_t" writeX3P.cpp
// For debugging use:
// mex -g -L"S:/openGPS/ISO5436_XML/install/lib/" -L"S:/openGPS/ISO5436_XML/install/bin/" -l"ISO5436_2_XML_S" -I"S:/openGPS/ISO5436_XML/install/include/" -I"C:\Programme\CodeSynthesis XSD 3.0\include"  COMPFLAGS="$COMPFLAGS /Zc:wchar_t" writeX3P.cpp

// We want to link to the iso5436_2.dll
#define SHARED_OPENGPS_LIBRARY
#define _UNICODE
#define UNICODE

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

// Check for incremental data in axis vector for arbitrary numeric type
// Returns 0 for non incremental axis
template <class T> double
AxisIncrement(const mxArray *x)
{
  double curr,increment=0;
  // Element pointer
  const T *ptr = (const T*)mxGetPr(x);
  // Number of elements
  mwSize numel = mxGetNumberOfElements(x);

  // estimate increment from first element
  increment = (*(ptr+1)) - (*ptr) ;
  while(numel-- > 0)
  {
    // Get current element
    curr= (*(ptr+1)) - (*ptr);
    // compare current increment to first one
    if (fabs(increment - curr) > std::numeric_limits<T>::epsilon())
      // This difference is too big, this is not an incremental axis
      return 0;
  }
  
  // This is an incremental axis. Return the increment
  return increment;
}

// Check for incremental x or y axis from x or y data vector
// Returns the increment or 0 for non incremental axis
bool
AxisIsIncremental(const mxArray *x, double &increment)
{
  mwSize ndims = mxGetNumberOfDimensions(x);
  const mwSize *dims = mxGetDimensions(x);

  // Preset increment
  increment=0;
  
  // Flag for coord vector
  bool isVector = false;
  
  // Check number of dimensions if only 1 or [1,m] or [n,1] it is a vector
  if ( ndims > 2)
    // This is not incremental
    return false;
  // Check for [n,1] or [1,m]
  if (ndims == 2)
  {
    if (((dims[0] == 1) && (dims[1] > 1))
      ||((dims[1] == 1) && (dims[0] > 1)))
      // This is a 1d coordinate vector an can be checked to be incremental
      isVector=true;
    else
      return false;
  }
  else if (ndims==1)
    // 1d vector
    isVector = true;
  else
    // Scalar or more dimensions
    return false;
  
  // If we found a 1d vector we can check it for constant step size between neighbouring elements:
  mwSize numel = mxGetNumberOfElements(x);
  
  // Get the increment
  // Do it for each type
  switch (mxGetClassID(x))
  {
    case mxDOUBLE_CLASS:
      increment = AxisIncrement<OGPS_Double>(x);
      break;
    case mxSINGLE_CLASS:
      increment = AxisIncrement<OGPS_Float>(x);
      break;
    case mxINT16_CLASS:
      increment = AxisIncrement<OGPS_Int16>(x);
      break;
    case mxINT32_CLASS:
      increment = AxisIncrement<OGPS_Int32>(x);
      break;
    default:
    {
      // Print full description
      ostrstream msg;
      // TODO: Error messages are not correctly printed yet!
      msg << "Error: Input arrays must be Double, Single, Int16 or Int32 type!" 
          << endl << ends;
      mexErrMsgIdAndTxt("openGPS:writeX3P:AxisType",msg.str());
      return false;
    }  
  }
  
  // Return the increment
  return true;
}


// Check meta structure for completeness
bool IsMetaComplete(const mxArray *meta)
{
  // Number of structure elements
  const unsigned int nelem=8;
  // Field names
  const char *fieldnames[nelem] = {"Date",/* Optional: "Creator", */ "Instrument_Manufacturer",
      "Instrument_Model","Instrument_Serial","Instrument_Version",
      "CalibrationDate","ProbingSystem_Type","ProbingSystem_Identification"
      /*, Optional: "Comment" */};
      
  // check all fields for existence
  for (unsigned int i=0; i<nelem; i++)
  {
    if ( NULL == mxGetField(meta, 0, fieldnames[i]))
    {
      // Print full description
      ostrstream msg;
      msg << "Error: Meta data record is incomplete, field\"" 
          << string(fieldnames[i]) << "\" is missing!" << endl << ends;
      mexErrMsgIdAndTxt("openGPS:writeX3P:MetaData",msg.str());
      return false;
    }
  }
  // All elements exist
  return true;
}


// Convert wstring to matlab string
mxArray *
ConvertWtoMStr(const std::wstring &inp)
{
  size_t len = inp.length();
  // String dimensions
  mwSize dims[2]={1,len};
  // Create a matlab string with length of input string
  mxArray *dest = mxCreateCharArray(2, dims);
  // Copy characters to string
  wchar_t *dptr = (wchar_t*)mxGetData(dest);
  for (unsigned int i=0; i<len ; i++)
    *(dptr++) = inp[i];

  // Return pointer to mxArray
  return dest;
}

// Convert matlab string to wstring
std::wstring &
ConvertMtoWStr(const mxArray *inp)
{
  // Destination string
  std::wstring *dest = new std::wstring();
  
  // Get len from
  mwSize len = mxGetNumberOfElements(inp);
  // Copy characters to string
  wchar_t *sptr = (wchar_t*)mxGetData(inp);
  // copy all characters to destination string
  for (int i=0; i<len ; i++)
    dest->push_back(*(sptr++));

  // Return reference to wstring
  // Todo: caller has to delete the result string
  return *dest;
}

// Get type of probing system from string
Record2Type::ProbingSystem_type::Type_type
GetProbingSystemTypeEnum(const mxArray *inp)
{
  // Input array must be a string
  // with one value of "Contacting", "NonContacting", "Software"
  wstring winp(ConvertMtoWStr(mxGetField(inp, 0,"ProbingSystem_Type")));
  
  if (winp == L"Contacting")
    return Record2Type::ProbingSystem_type::Type_type::Contacting;
  else if (winp == L"NonContacting")
    return Record2Type::ProbingSystem_type::Type_type::NonContacting;
  else if (winp == L"Software")
    return Record2Type::ProbingSystem_type::Type_type::Software;

  // Illegal value
  ostrstream msg;
  msg << "Error: Meta data field \"ProbingSystem_type\" has an illegal value\"" 
      << winp << "\". Possible values are one of \"Contacting\", \"NonContacting\" or \"Software\"." << endl << ends;
  mexErrMsgIdAndTxt("openGPS:writeX3P:ProbingSystem_type",msg.str());
  // This statement is never executed!
  return Record2Type::ProbingSystem_type::Type_type::Software;
}


/* The gateway function */
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
  bool bHasStruct=false;
  
  const wstring SyntaxHelp(  
     L"Call Syntax:\n"
     L"  pinfo = writeX3P(filename,x,y,z,meta)\n"
     L"    x     - 1,2 or 3d array of x coordinates in meter\n"
     L"    y     - 1,2 or 3d array of y coordinates in meter\n"
     L"    z     - 1,2 or 3d array of z coordinates in meter\n"
     L"    meta  - Meta data structure of the file with the following elements:\n"
     L"     .Date - Data set creation date of the form '2007-04-30T13:58:02.6+02:00'\n"
     L"     .Creator - optional name of the creator or empty array.\n"
     L"     .Instrument_Manufacturer - String with name of the manufacturer\n"
     L"     .Instrument_Model - String with instrument model or software name\n"
     L"     .Instrument_Serial - String with serial number of instrument or 'not available'.\n"
     L"     .Instrument_Version - Hardware and software version string of Instrument\n"
     L"     .CalibrationDate - Date of last calibration of the form '2007-04-30T13:58:02.6+02:00'\n"
     L"     .ProbingSystem_Type - one of  'Software','Contacting' or 'NonContacting'\n"
     L"     .ProbingSystem_Identification - String identifying lens, probe tip, etc.\n"
     L"     .Comment    - 'A user comment specific to this dataset'\n"          
     L"    pinfo - Info about data organisation\n");

  char FileName[1024];              /* filename */

  /* check for proper number of arguments */
  if(nrhs!=5)
  {
    ostrstream msg;
    msg << "writeX3P was called with " << nrhs << " input arguments!" << endl
        << SyntaxHelp << ends;
    mexErrMsgIdAndTxt("openGPS:writeX3P:nrhs",msg.str());
  }
  // Check number of output arguments
  if(nlhs!=1)
  {
    ostrstream msg;
    msg << "writeX3P was called with " << nlhs << " output arguments!" << endl
        << SyntaxHelp << ends;
    mexErrMsgIdAndTxt("openGPS:writeX3P:nlhs",msg.str());
  }
    
  /* make sure the first input argument is string */
  if( !mxIsChar(prhs[0]) || 
       mxGetNumberOfElements(prhs[0])<1 ) {
       mexErrMsgIdAndTxt("openGPS:writeX3P:notString","First argument must be a file name string");
  }

  /* get the filename  */
  if (mxGetString(prhs[0], FileName, sizeof(FileName)-1))
  {
    mexErrMsgIdAndTxt("openGPS:writeX3P:notString","Filename is too long.");
  }
  
  /* make sure the coordinate arrays are numeric */
  if( !mxIsNumeric(prhs[1]) || 
       mxGetNumberOfElements(prhs[1])<1 ) {
       mexErrMsgIdAndTxt("openGPS:writeX3P:notNumeric","Second input argument must be numeric");
  }
  if( !mxIsNumeric(prhs[2]) || 
       mxGetNumberOfElements(prhs[2])<1 ) {
       mexErrMsgIdAndTxt("openGPS:writeX3P:notNumeric","Third input argument must be numeric");
  }
  if( !mxIsNumeric(prhs[3]) || 
       mxGetNumberOfElements(prhs[3])<1 ) {
       mexErrMsgIdAndTxt("openGPS:writeX3P:notNumeric","Fourth input argument must be numeric");
  }
  
  /* make sure the fourth argument is a structure */
  if( !mxIsStruct(prhs[4]) || 
       mxGetNumberOfElements(prhs[4])<1 ) {
       mexErrMsgIdAndTxt("openGPS:writeX3P:notNumeric","Fifth input argument must be a meta data structure");
  }

  // Convert filename to wide character
  _TCHAR FileNameL[1024];
  for (unsigned int i=0; i<1024 ; i++)
    FileNameL[i] = FileName[i];

  // Get pointers to arrays
  const mxArray *inMatrixX=prhs[1];               /*input matrix with x-Values*/
  const mxArray *inMatrixY=prhs[2];               /*input matrix with y-Values*/
  const mxArray *inMatrixZ=prhs[3];               /*input matrix with z-Values*/
  const mxArray *meta=prhs[4];                    /* Meta data structure */

  // Check meta data for completeness
  if (false == IsMetaComplete(meta))
    return;
  
  // Get number of data dimensions
  mwSize ndimx = mxGetNumberOfDimensions(inMatrixX);
  mwSize ndimy = mxGetNumberOfDimensions(inMatrixY);
  mwSize ndimz = mxGetNumberOfDimensions(inMatrixZ);
  // Has to be one to 3 dimensions
  if ((ndimx < 1) || (ndimx>3) || (ndimy < 1) || (ndimy>3) || (ndimz < 1) || (ndimz>3))
  {
    // Print full description
    ostrstream msg;
    // TODO: Error messages are not correctly printed yet!
    msg << "Input arrays must have 1 to 3 dimensions but have [ndimx,ndimy,ndimz]=[" 
        << ndimx << ", " << ndimy << ", " << ndimz << "]!" << endl << ends;
    mexErrMsgIdAndTxt("openGPS:writeX3P:InputDimensions",msg.str());
    return;
  }  

  // Check for u,1,w dimensions: This is a profile feature
  // Other dimensions are a surface
  const mwSize *dim = mxGetDimensions(inMatrixZ);
  // check for profile data
  bool isProfile=false;
  if (ndimz == 1)
    // This is a profile
    isProfile = true;
  else if ((dim[0]==1) || (dim[1]==1))
    // This is also a profile if one dimension is 1
    isProfile = true;
  else
    // This is a surface feature
    isProfile = false;
  
  // Create records for document
  Record1Type::Revision_type revision(OGPS_ISO5436_2000_REVISION_NAME);
  
  // Set feature type
  Record1Type::FeatureType_type *featureType=NULL;
  if (isProfile)
    featureType = new Record1Type::FeatureType_type(OGPS_FEATURE_TYPE_PROFILE_NAME);
  else
    featureType = new Record1Type::FeatureType_type(OGPS_FEATURE_TYPE_SURFACE_NAME);
  
  // X and y are incremental if dimension of x are [n,1] or [1,n]
  double xIncrement=0;
  bool xIncremental;
  // Axis type
  Record1Type::Axes_type::CX_type::AxisType_type *xaxisType=NULL;

  if (AxisIsIncremental(inMatrixX, xIncrement))
  {
    xIncremental = true;
    // Make incremental axis
    xaxisType = new Record1Type::Axes_type::CX_type::AxisType_type(Record1Type::Axes_type::CX_type::AxisType_type::I); /* incremental */
  }
  else
  {
    xIncremental = false;
    // Absolute axis
    xaxisType = new Record1Type::Axes_type::CX_type::AxisType_type(Record1Type::Axes_type::CX_type::AxisType_type::A); /* incremental */
  }
  // Data type is allways double
  Record1Type::Axes_type::CX_type::DataType_type xdataType(Record1Type::Axes_type::CX_type::DataType_type::D); /* double */  
  // Create axis of specified type
  Record1Type::Axes_type::CX_type xaxis(*xaxisType);
  xaxis.DataType(xdataType);
  /* Set increment and offset. */
  if (xIncrement==0)
    xaxis.Increment(1.0);
  else
    xaxis.Increment(xIncrement);
  // Todo: Offset should be set to average of data min/max
  xaxis.Offset(0.0);
  // Delete the axis type 
  delete xaxisType;


    // X and y are incremental if dimension of x are [n,1] or [1,n]
  double yIncrement=0;
  bool yIncremental;
  // Axis type
  Record1Type::Axes_type::CY_type::AxisType_type *yaxisType=NULL;

  if (AxisIsIncremental(inMatrixY, yIncrement))
  {
    yIncremental = true;
    // Make incremental axis
    yaxisType = new Record1Type::Axes_type::CY_type::AxisType_type(Record1Type::Axes_type::CY_type::AxisType_type::I); /* incremental */
  }
  else
  {
    yIncremental = false;
    // Absolute axis
    yaxisType = new Record1Type::Axes_type::CY_type::AxisType_type(Record1Type::Axes_type::CY_type::AxisType_type::A); /* incremental */
  }
  // Data type is allways double
  Record1Type::Axes_type::CY_type::DataType_type ydataType(Record1Type::Axes_type::CY_type::DataType_type::D); /* double */  
  // Create axis of specified type
  Record1Type::Axes_type::CY_type yaxis(*yaxisType);
  yaxis.DataType(ydataType);
  /* Set increment and offset. */
  if (yIncrement==0)
    yaxis.Increment(1.0);
  else
    yaxis.Increment(yIncrement);

  // Todo: Offset should be set to average of data min/max
  yaxis.Offset(0.0);
  // Delete the axis type 
  delete yaxisType;


  // Setup z-axis, allways double absolute
  Record1Type::Axes_type::CZ_type::AxisType_type zaxisType(Record1Type::Axes_type::CZ_type::AxisType_type::A); /* absolute */
  Record1Type::Axes_type::CZ_type::DataType_type zdataType(Record1Type::Axes_type::CZ_type::DataType_type::D); /* double */
  Record1Type::Axes_type::CZ_type zaxis(zaxisType);
  zaxis.DataType(zdataType);
  zaxis.Increment(1.0);
  // Todo: Offset should be set to average of data min/max
  zaxis.Offset(0.0);

  // Create the final axis definition for all three axis
  Record1Type::Axes_type axis(xaxis, yaxis, zaxis);
  // Create Record1
  Record1Type record1(revision, *featureType, axis);
  // Delete feature type
  delete featureType;
  featureType=NULL;
   
  /* Create RECORD2 with information from meta data structure */
  // Data set creation date
  Record2Type::Date_type date(ConvertMtoWStr(mxGetField(meta, 0,"Date")));
  
  // Instrument manufacturer
  Record2Type::Instrument_type::Manufacturer_type 
          manufacturer(ConvertMtoWStr(mxGetField(meta, 0,"Instrument_Manufacturer")));
  
  // Instrument model
  Record2Type::Instrument_type::Model_type 
          model(ConvertMtoWStr(mxGetField(meta, 0,"Instrument_Model")));

  // Instrument serial number set to default if not specified
  Record2Type::Instrument_type::Serial_type *serial=NULL;

  if (mxGetNumberOfElements(mxGetField(meta, 0,"Instrument_Serial")) == 0)
  {
    serial = new Record2Type::Instrument_type::Serial_type(_T("not available"));
  }
  else
  {
    serial = new Record2Type::Instrument_type::Serial_type(
            ConvertMtoWStr(mxGetField(meta, 0,"Instrument_Serial")));
  }
  // Instrument hard and software version
  Record2Type::Instrument_type::Version_type 
          version(ConvertMtoWStr(mxGetField(meta, 0,"Instrument_Version")));
  // Assemble instrument information
  Record2Type::Instrument_type instrument(manufacturer, model, *serial, version);
  // Delete serial element
  delete serial;
  serial=NULL;
  
  // Calibration date
  Record2Type::CalibrationDate_type 
          calibrationDate(ConvertMtoWStr(mxGetField(meta, 0,"CalibrationDate")));

  // Type of probing system: Contacting, non Contacting, Software 
  Record2Type::ProbingSystem_type::Type_type type(GetProbingSystemTypeEnum(meta));

  // Type of probing system
  Record2Type::ProbingSystem_type::Identification_type id(ConvertMtoWStr(mxGetField(meta, 0,"ProbingSystem_Identification")));
  // Create the probing system type
  Record2Type::ProbingSystem_type probingSystem(type, id);

  // Create Record2 from collected data
  Record2Type record2(date, instrument, calibrationDate, probingSystem);

  // Look out for comment field
  mxArray *cfield = mxGetField(meta, 0,"Comment");
  if (cfield != NULL)
  {
    // Check for elements in comment string
    if (mxGetNumberOfElements(cfield)>0)
    {
      // Add comment to record 2
      record2.Comment(Record2Type::Comment_type(ConvertMtoWStr(cfield)));
    }
    else
      cfield=NULL;
  }
  
  if (cfield==NULL)
  {
    // Issu warning
    mexWarnMsgIdAndTxt("openGPS:writeX3P:Comment",
            "Data should contain a meaningful comment.");
  }

  // Check for creator field
  mxArray *crfield = mxGetField(meta, 0,"Creator");
  if (crfield != NULL)
  {
    // Check for elements in comment string
    if (mxGetNumberOfElements(crfield)>0)
    {
      // Add comment to record 2
      record2.Creator(Record2Type::Creator_type(ConvertMtoWStr(crfield)));
    }
  }

  /* Create MATRIX */
  unsigned long mdims[3];
  mdims[0] = dim[0];
  if (ndimz > 1)
    mdims[1] = dim[1];
  else
    mdims[1] = 1;
  if (ndimz > 2)
    mdims[2] = dim[2];
  else
    mdims[2] = 1;

  MatrixDimensionType matrix(mdims[0], mdims[1], mdims[2]);

  bool useBinary=false;
  // For more than 5000 Elements use binary encoding
  if (mxGetNumberOfElements(inMatrixZ)>=5000)
    useBinary=true;

  /* Create ISO5436_2 container, */
  OGPS_ISO5436_2Handle handle = ogps_CreateMatrixISO5436_2(FileNameL, NULL, record1, &record2, matrix, useBinary);

  /* Add data points */
  /* 1. Create point vector buffer */
  OGPS_PointVectorPtr vector = ogps_CreatePointVector();

  /* Matrix indices. */
  OGPS_ULong u = 0;
  OGPS_ULong v = 0;
  OGPS_ULong w = 0;

  // Data pointer to z
  OGPS_Double *pdbl=NULL;
  OGPS_Float *pflt=NULL;
  OGPS_Int16 *pshrt=NULL;
  OGPS_Int32 *plng=NULL;
  // Data type
  unsigned int dtype=0;
  if (mxIsDouble(inMatrixZ))
    dtype=1;
  else if (mxIsSingle(inMatrixZ))
    dtype=2;
  else if (mxIsUint16(inMatrixZ))
    dtype=3;
  else if (mxIsUint32(inMatrixZ))
    dtype=3;  
  
  // Get data pointer
  switch (dtype)
  {
    case 1:
      pdbl = (OGPS_Double*)mxGetPr(inMatrixZ);
      break;
    case 2:
      pflt = (OGPS_Float*)mxGetPr(inMatrixZ);
      break;
    case 3:
      pshrt = (OGPS_Int16*)mxGetPr(inMatrixZ);
      break;
    case 4:
      plng = (OGPS_Int32*)mxGetPr(inMatrixZ);
      break;
  }
  
  // Loop over array
  for (w=0; w<mdims[2] ; w++)
  {
    for (v=0; v<mdims[1]; v++)
    {
      for (u=0; u<mdims[0]; u++)
      {
        // Set z-coordinate
        switch (dtype)
        {
          case 1:
            ogps_SetDoubleZ(vector, *(pdbl++));
            break;
          case 2:
            ogps_SetFloatZ(vector, *(pflt++));
            break;
          case 3:
            ogps_SetInt16Z(vector, *(pshrt++));
            break;
          case 4:
            ogps_SetInt32Z(vector, *(plng++));
            break;
        }
          
        /* 2. if the z axis is of absolute type and the
        * other two are incremental, we simply set up just z
        * values and leave x an y values untouched (missing). */

        /* 3. Write into document */
        ogps_SetMatrixPoint(handle, u , v, w, vector);

        // TODO: Handle missing data points
        /* 2b/3b. We have missing point data, here. */
        //ogps_SetMatrixPoint(handle, u , v, w + 1, NULL);
      }
    }
  }
  /* Free buffer */
  ogps_FreePointVector(&vector);

   /* How to append vendorspecific data:
   OpenGPS::String vname(fileName.substr(0, fileName.find_last_of(_T("/\\")) + 1));
   vname.append(_T("vendor.tmp"));
   std::wofstream vendor_dat(vname.ToChar(), std::ios::trunc);
   vendor_dat << _T("Vendorspecific data.") << std::endl;
   vendor_dat.close();

   ogps_AppendVendorSpecific(handle, _T("http://www.example.com/format/version1"), vname.c_str());
   */
   
  /* Finally: write container to disk. */
  ogps_WriteISO5436_2(handle);
  ogps_CloseISO5436_2(&handle);

  ///***********************************************************************
  // Check for error
  if (ogps_HasError())
  {
    // Print full description
    ostrstream msg;
    msg << "Error writing X3P-file name \"" << wstring(FileNameL) << "\"!" << endl
        << ogps_GetErrorMessage() << endl
        << ogps_GetErrorDescription() << endl
        << ends;
    mexErrMsgIdAndTxt("openGPS:writeX3P:FileOpen",msg.str());
    return;
  }
}
