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

#ifndef _OPENGPS_ZIP_STREAM_BUFFER_HXX
#define _OPENGPS_ZIP_STREAM_BUFFER_HXX

#include <ostream>

/* zlib/minizip */
#include <zip.h>

#include "../xyssl/md5.h"

#ifndef _OPENGPS_CXX_OPENGPS_HXX
#  include <opengps/cxx/opengps.hxx>
#endif

namespace OpenGPS
{
   class ZipStreamBuffer : public std::streambuf
   {
      typedef std::streambuf BaseType;

   public:
      ZipStreamBuffer(zipFile handle, const OGPS_Boolean enable_md5);
      ~ZipStreamBuffer();

      OGPS_Boolean GetMd5(OpenGPS::UnsignedByte md5[16]);

   protected:
      virtual std::streamsize xsputn( const char * s, std::streamsize n );

   private:
      zipFile m_Handle;
      md5_context *m_Md5Context;
   };

   class ZipOutputStream : public std::ostream
   {
      typedef std::ostream BaseType;

   public:
      ZipOutputStream(ZipStreamBuffer& buffer);
      ~ZipOutputStream();
   };
}

#endif /* _OPENGPS_ZIP_STREAM_BUFFER_HXX */
