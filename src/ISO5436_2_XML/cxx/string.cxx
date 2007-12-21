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

#include <opengps/opengps.hxx>
#include "stdafx.hxx"

String::String() :
   BaseType()
   {
      m_Chars = NULL;
   }

   String::String(const BaseType& s) : BaseType(s)
{
   m_Chars = NULL;
}
     String::String(const OGPS_Character* s) : BaseType(s)
{
   m_Chars = NULL;
}

   String::~String()
   {
#ifdef _UNICODE
      if(m_Chars)
      {
         delete[] m_Chars;
      }
#endif
   }

   const char* String::ToChar()
   {

   #ifdef _UNICODE
      if(m_Chars)
      {
         delete[] m_Chars;
      }

      const size_t len = length();
      m_Chars = new char[len + 1];
      wcstombs(m_Chars, c_str(), len);
      m_Chars[len] = 0;

      return m_Chars;
#else
      return c_str();
   #endif
   }
