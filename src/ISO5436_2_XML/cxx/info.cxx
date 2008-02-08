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

#include <opengps/cxx/info.hxx>
#include <opengps/cxx/string.hxx>
#include <iostream>
#include "stdafx.hxx"

Info::Info()
{
}

Info::~Info()
{
}

void Info::GetCopyright(OpenGPS::String* const text)
{
   _ASSERT(text);

   *text = _T(

"Copyright by\
Johannes Herwig (Uni Duisburg-Essen) 2007\
Georg Wiora (NanoFocus AG) 2007\
Jörg Seewig (Uni Hannover) 2007\
Andreas Walther (NanoFocus AG) 2007\
Mark A. Weber (NanoFocus AG) 2007\
\
\
\
ADDITIONAL CONTRIBUTIONS are copyrighted by\
\
Ian Martins (ianxm@jhu.edu)\
Jean-loup Gailly and Mark Adler 1995-2004\
Christophe Devine 2006-2007\
"

   );
}

void Info::GetLicense(OpenGPS::String* const text)
{
   _ASSERT(text);

   *text = _T(

"Licence Declaration for the openGPS(R)[TM] software library\
This program is free software; you can redistribute it and/or modify\
it under the terms of the GNU Lesser General Public License (LGPL)\
as published by the Free Software Foundation; either version 3 of\
the License, or (at your option) any later version.\
for detail see the files \"licence_LGPL-3.0.txt\" and\
\"licence_GPL-3.0.txt\".\
\
openGPS is distributed in the hope that it will be useful,\
but WITHOUT ANY WARRANTY; without even the implied warranty of\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the\
GNU Lesser General Public License for more details.\
\
You should have received a copy of the GNU General Public License\
along with this program.If not, see <http://www.gnu.org/licenses/>.\
\
Further information on the openGPS project can be found at\
\
  http://www.opengps.eu/\
\
TRADEMARKS\
\
The name \"openGPS\" and the logo are registered as European\
trade mark No. 006178354 for\
\
  Physikalisch Technische Bundesanstalt (PTB)\
  Bundesallee 100\
  38116 Braunschweig\
  Germany\
\
  http://www.ptb.de/\
\
\
\
ADDITIONAL CONTRIBUTIONS\
\
The documentation is created using doxygen. To produce propper output\
from MATLAB code the filter \"mtoc\" from Ian Martins (ianxm@jhu.edu) is\
used. mtoc has been put under the GPL by Ian Martins.\
\
The project uses \"ZLIB DATA COMPRESSION LIBRARY\" V1.2.3\
(C) 1995-2004 Jean-loup Gailly and Mark Adler\
\
The project uses the md5 algorithms from the \"XYSSL Library\" V0.8\
Copyright (C) 2006-2007  Christophe Devine\
"

   );

}

void Info::PrintCopyright()
{
   OpenGPS::String text;
   GetCopyright(&text);

   std::cout << *text.ToChar() << std::endl;
}

void Info::PrintLicense()
{
   OpenGPS::String text;
   GetLicense(&text);

   std::cout << *text.ToChar() << std::endl;
}

void Info::GetName(OpenGPS::String* const text)
{
   _ASSERT(text);

   *text = _OPENGPS_NAME;
}

void Info::GetAbout(OpenGPS::String* const text)
{
   _ASSERT(text);

   *text = _OPENGPS_DESCRIPTION;
}

void Info::GetVersion(OpenGPS::String* const text)
{
   _ASSERT(text);

   *text = _OPENGPS_VERSIONSTRING;
}

void Info::PrintVersion()
{
   OpenGPS::String version(_OPENGPS_ID);

   std::cout << version.ToChar() << std::endl;
}
