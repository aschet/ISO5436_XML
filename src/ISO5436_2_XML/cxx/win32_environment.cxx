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

#ifdef _WIN32

#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "win32_environment.hxx"
#include "stream_types.hxx"
#include "stdafx.hxx"

Win32Environment::Win32Environment()
: Environment()
{
   //SetInstance(this);
}

Win32Environment::~Win32Environment()
{
}

OGPS_Boolean Win32Environment::m_InitRandom = TRUE;

OGPS_Character Win32Environment::GetDirectorySeparator() const
{
   return _T('\\');
}

OGPS_Character Win32Environment::GetAltDirectorySeparator() const
{
   return _T('/');
}

OpenGPS::String Win32Environment::GetPathName(const OpenGPS::String& path) const
{
   _ASSERT(path.length() > 0);

   // TODO: see .NET implementation
   return path;
}

OpenGPS::String Win32Environment::ConcatPathes(const OpenGPS::String& path1, const OpenGPS::String& path2) const
{
   _ASSERT(path1.length() > 0 || path2.length() > 0);

   // TODO: see .NET implementation
   
   if(path1.length() == 0)
   {
      return path2;
   }

   if(path2.length() == 0)
   {
      return path1;
   }

   OGPS_Boolean path2StartsWithSeparator = (path2[0] == GetDirectorySeparator() || path2[0] == GetAltDirectorySeparator());

   String path = path1;

   if(path1[path1.length() - 1] != GetDirectorySeparator() && path1[path1.length() - 1] != GetAltDirectorySeparator())
   {
      if(!path2StartsWithSeparator)
      {
         path += GetDirectorySeparator();
      }
   }
   else
   {
      if(path2StartsWithSeparator)
      {
         path += path2.substr(1, path2.length() - 1);
         return path;
      }
   }

   path += path2;

   return path;
}

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES          (DWORD)-1
#endif /* INVALID_FILE_ATTRIBUTES */

OGPS_Boolean Win32Environment::PathExists(const OpenGPS::String& file) const
{
   _ASSERT(file.length() > 0);

   const DWORD dwAttr = GetFileAttributes(file.c_str());
if (dwAttr == INVALID_FILE_ATTRIBUTES)
{
  const DWORD dwError = GetLastError();
  if (dwError == ERROR_FILE_NOT_FOUND)
  {
    // file not found
     return FALSE;
  }
  else if (dwError == ERROR_PATH_NOT_FOUND)
  {
    // path not found
     return FALSE;
  }
  else if (dwError == ERROR_ACCESS_DENIED)
  {
    // file or directory exists, but access is denied
     return TRUE;
  }
  else
  {
    // some other error has occured
     _ASSERT(FALSE);
     return FALSE;
  }
}
else
{
   /*
  if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
  {
    // this is a directory
  }
  else
  {
    // this is an ordinary file
  }
  */
   return TRUE;
}
_ASSERT(FALSE);
return FALSE;
}

OGPS_Boolean Win32Environment::RemoveFile(const OpenGPS::String& file) const
{
   _ASSERT(file.length() > 0);
   return (DeleteFile(file.c_str()) != 0);
}

#define GETRANDOM(min, max) ((rand()%(int)(((max) + 1)-(min)))+ (min))

OpenGPS::String Win32Environment::GetUniqueName() const
{
   if(m_InitRandom)
   {
      srand ( time(NULL) );
      m_InitRandom = FALSE;
   }

   // random positive number with max. 8 digits
   int randNum = GETRANDOM(0, 10000000);

   OpenGPS::OutStringStream os;
   os << randNum;

   return os.str();
}

OGPS_Boolean Win32Environment::CreateDir(const OpenGPS::String& path) const
{
   _ASSERT(path.length() > 0);
   return (CreateDirectory(path.c_str(), NULL) != 0);
}

OGPS_Boolean Win32Environment::RemoveDir(const OpenGPS::String& path) const
{
   _ASSERT(path.length() > 0);

   const String pattern = ConcatPathes(path, _T("*"));

   WIN32_FIND_DATA found;
   HANDLE handle = FindFirstFile(pattern.c_str(), &found);
   if(handle != INVALID_HANDLE_VALUE)
   {
      do
      {
         const String fname = found.cFileName;
         if(fname != _T(".") && fname != _T(".."))
         {
            const String fpath = ConcatPathes(path, fname);
            DeleteFile(fpath.c_str());
         }
      } while(FindNextFile(handle, &found));

      FindClose(handle);
   }

   return (RemoveDirectory(path.c_str()) != 0);
}

OpenGPS::String Win32Environment::GetTempDir() const
{
   unsigned long size = GetTempPath(0, NULL);

   OGPS_Character* buffer = new OGPS_Character[size];
   if(GetTempPath(size, buffer) + 1 == size)
   {
      String path(buffer);
      delete buffer;
      return path;
   }

   _ASSERT(FALSE);

   delete buffer;
   return _T("");
}

OGPS_Boolean Win32Environment::RenameFile(const String& src, const String& dst) const
{
#ifdef MOVEFILE_FAIL_IF_NOT_TRACKABLE
   return (MoveFileEx(src.c_str(), dst.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_FAIL_IF_NOT_TRACKABLE | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0);
#else
   return (MoveFileEx(src.c_str(), dst.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0);
#endif
}

Environment* Environment::CreateInstance()
{
   return new Win32Environment();
}

#endif /* _WIN32 */
