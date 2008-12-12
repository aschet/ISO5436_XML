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

using namespace std;

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
