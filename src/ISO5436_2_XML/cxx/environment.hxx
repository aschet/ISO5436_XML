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

#ifndef _OPENGPS_ENVIRONMENT_HXX
#define _OPENGPS_ENVIRONMENT_HXX

#ifndef _OPENGPS_CXX_OPENGPS_HXX
#  include <opengps/cxx/opengps.hxx>
#endif

namespace OpenGPS
{
   class Environment
   {
   public:
      static const Environment* const GetInstance();
      static void Reset();

      virtual OGPS_Character GetDirectorySeparator() const = 0;
      virtual OGPS_Character GetAltDirectorySeparator() const = 0;
      virtual OGPS_Boolean GetPathName(const OpenGPS::String& path, OpenGPS::String& clean_path) const = 0;
      virtual OpenGPS::String ConcatPathes(const OpenGPS::String& path1, const OpenGPS::String& path2) const = 0;
      virtual OGPS_Boolean PathExists(const OpenGPS::String& file) const = 0;
      virtual OGPS_Boolean RemoveFile(const OpenGPS::String& file) const = 0;
      virtual OpenGPS::String GetUniqueName() const = 0;
      virtual OGPS_Boolean CreateDir(const OpenGPS::String& path) const = 0;
      virtual OGPS_Boolean RemoveDir(const OpenGPS::String& path) const = 0;
      virtual OpenGPS::String GetTempDir() const = 0;
      virtual OGPS_Boolean RenameFile(const OpenGPS::String& src, const OpenGPS::String& dst) const = 0;
      virtual OGPS_Boolean GetVariable(const OpenGPS::String& varName, OpenGPS::String& value) const = 0;

      virtual OGPS_Boolean IsLittleEndian() const;

      OpenGPS::UnsignedBytePtr ByteSwap(const short* const value, OpenGPS::UnsignedBytePtr dst) const;
      void ByteSwap(const OpenGPS::UnsignedBytePtr src, short* const value) const;

      OpenGPS::UnsignedBytePtr ByteSwap16(const short* const value, OpenGPS::UnsignedBytePtr dst) const;
      void ByteSwap16(const OpenGPS::UnsignedBytePtr src, short* const value) const;

      OpenGPS::UnsignedBytePtr ByteSwap(const int* const value, OpenGPS::UnsignedBytePtr dst) const;
      void ByteSwap(const OpenGPS::UnsignedBytePtr src, int* const value) const;

      OpenGPS::UnsignedBytePtr ByteSwap32(const int* const value, OpenGPS::UnsignedBytePtr dst) const;
      void ByteSwap32(const OpenGPS::UnsignedBytePtr src, int* const value) const;

      OpenGPS::UnsignedBytePtr ByteSwap(const float* const value, OpenGPS::UnsignedBytePtr dst) const;
      void ByteSwap(const OpenGPS::UnsignedBytePtr src, float* const value) const;

      OpenGPS::UnsignedBytePtr ByteSwap32(const float* const value, OpenGPS::UnsignedBytePtr dst) const;
      void ByteSwap32(const OpenGPS::UnsignedBytePtr src, float* const value) const;

      OpenGPS::UnsignedBytePtr ByteSwap(const double* const value, OpenGPS::UnsignedBytePtr dst) const;
      void ByteSwap(const OpenGPS::UnsignedBytePtr src, double* const value) const;

      OpenGPS::UnsignedBytePtr ByteSwap64(const double* const value, OpenGPS::UnsignedBytePtr dst) const;
      void ByteSwap64(const OpenGPS::UnsignedBytePtr src, double* const value) const;

      // TODO: Statt #define _OPENGPS_ENV_ISO5436_LOCATION _T("OPENGPS_ISO5436_XSD_PATH")
      //class EnvironmentVariables;

   protected:
      Environment();
      virtual ~Environment();

      //static void SetInstance(Environment* instance);
      static Environment* CreateInstance();

   private:
      static Environment* m_Instance;
   };
}

#endif /* _OPENGPS_ENVIRONMENT_HXX */
