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

#include "X3PUtilities.h"

#include <strstream>
#include <iomanip>
#include <ios>
#include <opengps/cxx/info.hxx>

#include <time.h>

using namespace std;
using namespace OpenGPS::Schemas::ISO5436_2;

// Helper functions for X3P import export in MATLAB
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
std::wstring 
ConvertMtoWStr(const mxArray *inp)
{
  // Destination string
  std::wstring dest;
  
  // Get len from
  mwSize len = mxGetNumberOfElements(inp);
  // Copy characters to string
  wchar_t *sptr = (wchar_t*)mxGetData(inp);
  // copy all characters to destination string
  for (int i=0; i<len ; i++)
    dest.push_back(*(sptr++));

  // Return reference to wstring
  return dest;
}


// Get point info structure returned by openX3P and writeX3P
mxArray *
GetPointInfoStructure(OGPS_ISO5436_2Handle handle)
{
  // Result
  mxArray *res=NULL;

  // Get document 
  const ISO5436_2Type* const document = ogps_GetDocument(handle);
  if (ogps_HasError())
  {
    mexErrMsgIdAndTxt("openGPS:X3P:XMLDocument",
            "Could not access XML document in current x3p file!");
    return NULL;
  }
  // Get a reference to record 1
  const OpenGPS::Schemas::ISO5436_2::ISO5436_2Type::Record1_type &r1 = document->Record1();

  // Create point info structure
  // Number of structure elements
  const unsigned int nelem=8;
  // Field names
  const char *fieldnames[nelem] = {"Revision","FeatureType","IsMatrix","IsList",
                                 "isXIncremental","isYIncremental","isZIncremental","DLL_Version"};
  // Create the structure
  res = mxCreateStructMatrix(1, 1, nelem, &(fieldnames[0]));

  // Get file format revision
  mxSetField(res, 0, "Revision",  ConvertWtoMStr(r1.Revision()));

  // Get Feature type "PRF", "SUR", "PCL"
  mxSetField(res, 0, "FeatureType",  ConvertWtoMStr(r1.FeatureType()));

  // Check for matrix organisation
  mxSetField(res, 0, "IsMatrix",  mxCreateLogicalScalar(ogps_IsMatrix(handle)));
  // Check for list organisation
  mxSetField(res, 0, "IsList",  mxCreateLogicalScalar(ogps_IsMatrix(handle) ? false:true));
  // Check for axis types
  mxSetField(res, 0, "isXIncremental",  mxCreateLogicalScalar(
          r1.Axes().CX().AxisType() == Record1Type::Axes_type::CX_type::AxisType_type::I 
          ? true : false));
  mxSetField(res, 0, "isYIncremental",  mxCreateLogicalScalar(
          r1.Axes().CY().AxisType() == Record1Type::Axes_type::CY_type::AxisType_type::I 
          ? true : false));
  mxSetField(res, 0, "isZIncremental",  mxCreateLogicalScalar(
          r1.Axes().CZ().AxisType() == Record1Type::Axes_type::CZ_type::AxisType_type::I 
          ? true : false));    

  // Set dll version
  mxSetField(res, 0, "DLL_Version",  ConvertWtoMStr(GetX3P_Dll_ID()));
  // Return pointer to structure
  return res;
}


// Get the name and version from X3P dll
std::wstring 
GetX3P_Dll_ID(void)
{  
  // Get dll name
  OpenGPS::String Name;
  OpenGPS::Info::GetName(&Name);
  // Get Version info
  OpenGPS::String Version;
  OpenGPS::Info::GetVersion(&Version);

  // Create output string
  std::wstring msg(Name);
  msg.append(L" V");
  msg.append(Version);

  // Return result string
  return msg;
}


/*!
  @brief Helper function to return the current time properly formated.
  
  @return A string containing the time stamp from now.

  @note There is only a windows implementation yet. In other cases
  return a dummy. That is enough for testing purposes.
*/
#ifdef _WIN32
OpenGPS::String TimeStamp(void)
{
  time_t ltime;
  struct tm lt;
  // Time zone offset
  long tzoff;
  // Set timezone
  _tzset();
  // Get time zone offset
  _get_timezone(&tzoff);
  // Offset ours and minutes
  int tzoff_h,tzoff_m;
  tzoff_h = -(int)floor(((double)tzoff)/3600.);
  tzoff_m = -(int)floor(((double)tzoff)/60. + 0.5) - tzoff_h*60;

  // Get current time
  time( &ltime );
  // get local time
  localtime_s(&lt,&ltime);
  
  // Correct tz offset by dst
  if (lt.tm_isdst > 0)
    tzoff_h++;

  // Absolute offset for printing
  int tzoff_habs = abs(tzoff_h);
  OGPS_Character tzoffsign = tzoff_h<0 ? _T('-') : _T('+');

  // Create a string of pattern "2007-04-30T13:58:02.6+02:00"
  wostringstream sout;
  sout << std::setfill(_T('0')) << setw(4) << (lt.tm_year+1900) << _T('-') << setw(2) << lt.tm_mon << _T('-') << setw(2) << lt.tm_mday 
      << _T('T') << setw(2) << lt.tm_hour << _T(':') << setw(2) << lt.tm_min << _T(':') << setw(2) << lt.tm_sec << _T(".0")
      << tzoffsign << setw(2) << tzoff_habs << _T(':') << setw(2) << tzoff_m << ends;

  return sout.str();
}
#else
// There is only a windows implementation yet.
//In other cases return a dummy. That is enough for testing purposes.
OpenGPS::String TimeStamp(void)
{
  return _T("2000-01-01T00:00:00.0+00:00");
}

#endif

/*!
  @brief Helper function to return the current time properly formated.
  
  @return A date_time structure containing the time from now.

  @note There is only a windows implementation yet. In other cases
  return a dummy. That is enough for testing purposes.
*/
#ifdef _WIN32
Record2Type::Date_type *TimeStampCS(void)
{
  Record2Type::Date_type *date=NULL;

  time_t ltime;
  struct tm lt;
  // Time zone offset
  long tzoff;
  // Set timezone
  _tzset();
  // Get time zone offset
  _get_timezone(&tzoff);
  // Offset ours and minutes
  int tzoff_h,tzoff_m;
  tzoff_h = -(int)floor(((double)tzoff)/3600.);
  tzoff_m = -(int)floor(((double)tzoff)/60. + 0.5) - tzoff_h*60;

  // Get current time
  time( &ltime );
  // get local time
  localtime_s(&lt,&ltime);
  
  // Correct tz offset by dst
  if (lt.tm_isdst > 0)
    tzoff_h++;

  // Use current date to fill structure
  date = new Record2Type::Date_type(lt.tm_year+1900, lt.tm_mon, lt.tm_mday,
           lt.tm_hour, lt.tm_min, lt.tm_sec,
           tzoff_h, tzoff_m);

  return date;
}
#else
// There is only a windows implementation yet.
//In other cases return a dummy. That is enough for testing purposes.
Record2Type::Date_type *TimeStampCS(void)
{
  Record2Type::Date_type *date=NULL;

  //return _T("2000-01-01T00:00:00.0+00:00");
  date = new Record2Type::Date_type(2000, 1, 1, 0, 0, 0, 0, 0);
}

#endif


/*!
  @brief Helper function to convert a matlab vector timestamp to the X3P Date_time type.

  @argument vecTime is a matlab array either as string with 27 elements or as datevec().

  @return A date_time structure containing the time.

  @note There is only a windows implementation yet. In other cases
  return a dummy. That is enough for testing purposes.
*/
Record2Type::Date_type *VectorTimeToX3PTime(const mxArray *vecTime)
{
  // Resulting date structure
  Record2Type::Date_type *date=NULL;

  // No Input, no output
  if (vecTime == NULL)
    return NULL;

  // Get number of elements in datevector
  mwSize nElem = mxGetNumberOfElements(vecTime);

  // Check input type
  if( ((nElem==6) || (nElem==8)) && mxIsNumeric(vecTime) && mxIsDouble(vecTime))
  {
    // Get a pointer to the date array vector
    double *vec = mxGetPr(vecTime);
    // read array elements and write to data structure
    if (nElem == 6)
    {
      // Ignore time zone offset since this is unknown to matlab
      date = new Record2Type::Date_type((unsigned short)floor(vec[0]), (unsigned short)floor(vec[1]),
                                        (unsigned short)floor(vec[2]), (unsigned short)floor(vec[3]),
                                        (unsigned short)floor(vec[4]), vec[5]);
    }
    else
    {
      // Use timezone information
      date = new Record2Type::Date_type((unsigned short)floor(vec[0]), (unsigned short)floor(vec[1]),
                                        (unsigned short)floor(vec[2]), (unsigned short)floor(vec[3]),
                                        (unsigned short)floor(vec[4]), vec[5],
                                        (short)floor(vec[6]),(short)floor(vec[7])); // Timezone
    }
    // Return result
    return date;
  }
  else if ((nElem==27) && mxIsChar(vecTime))
  {
    // Parse text date in the format '2007-04-30T13:58:02.6+02:00'
    // Create a stringstream from MatlabString
    wistringstream dstr(ConvertMtoWStr(vecTime));
    int year,month,day,hours,minutes;
    double seconds;
    int tzhours,tzmins;
    wchar_t c; // Dummy character to eat separation chars
    // Parse the string
    dstr >> year >> c >> month >> c >> day
         >> c >> hours >> c >> minutes >> c >> seconds >> tzhours >> c >> tzmins;
    // Convert timezone to UTC
    hours -= tzhours;
    minutes -= tzmins;
    // Create date structure
    date = new Record2Type::Date_type(year, month, day, hours, minutes, seconds);
    return date;
  }
  else if (nElem>1)
  {
    // if less than 2 elements we assume that the field has intentionally been left blank
    // Not a valid date format
      mexWarnMsgIdAndTxt("openGPS:X3PUtilitites:Date",
              "Dates must have a length of 27 characters and a format\n"
              "similar to '2007-04-30T13:58:02.6+02:00' or must be a date vector as\n"
              "returned by datevec() function. Now using the current date as\n"
              "data set creation date.\n");
  }
  // return a NULL pointer
  return NULL;
}


/*!
  @brief Helper function to convert the X3P Date_time type to a matlab datevec().

  @argument vecTime is a matlab datevec array with 6 or 8 elements. In the 8 element case the hours and minutes of the time zone offset are appended at the end.

  @return A matlab datevec() array plus time zone offsets.
*/
mxArray *X3PTimeToVectorTime(const Record2Type::Date_type &x3pTime)
{
  // No Input, no output
  //if (x3pTime == NULL)
  //  return NULL;

  // Create a matrix with 8 elements
  mxArray *vectime = mxCreateDoubleMatrix(1,8,mxREAL);
  if (vectime == NULL)
  {
      mexWarnMsgIdAndTxt("openGPS:X3PUtilitites:X3PTimeToVectorTime",
              "Could not allocate 8 element vector for datevec().\n");
      return NULL;
  }

  // Set the elements of datevec
  double *ptr = mxGetPr(vectime);
  // Set year of datevec
  *(ptr++) = x3pTime.year();
  // Set month of datevec
  *(ptr++) = x3pTime.month();
  // Set day of datevec
  *(ptr++) = x3pTime.day();
  // Set hours of datevec
  *(ptr++) = x3pTime.hours();
  // Set minutes of datevec
  *(ptr++) = x3pTime.minutes();
  // Set seconds of datevec
  *(ptr++) = x3pTime.seconds();
  // Set time zone offset hours
  *(ptr++) = x3pTime.zone_hours();
  // Set time zone offset minutes
  *(ptr++) = x3pTime.zone_minutes();

  return vectime;
}
