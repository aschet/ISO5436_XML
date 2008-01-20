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

#ifndef _OPENGPS_EXCEPTIONS_HXX
#define _OPENGPS_EXCEPTIONS_HXX

#ifndef _OPENGPS_CXX_OPENGPS_HXX
#  include <opengps/cxx/opengps.hxx>
#endif

#ifndef _OPENGPS_CXX_EXCEPTIONS_HXX
#  include <opengps/cxx/exceptions.hxx>
#endif

namespace OpenGPS
{
   class ExceptionHistory
   {
   public:
      static void SetLastException(const Exception& ex);
      static void SetLastException();

      static void Reset();

      static const OGPS_Character* GetLastErrorMessage();
      static OGPS_ExceptionId GetLastExceptionId();

   private:
      ExceptionHistory();
      ~ExceptionHistory();

      static OpenGPS::String m_LastErrorMessage;
      static OGPS_ExceptionId m_LastExceptionId;
   };
}

#define _OPENGPS_GENERIC_EXCEPTION_HANDLER_RETVALBOOL(STATEMENT) \
   OpenGPS::ExceptionHistory::Reset(); \
   try \
   { \
      return (STATEMENT); \
   } \
   catch(const OpenGPS::Exception& ex) \
   { \
      OpenGPS::ExceptionHistory::SetLastException(ex); \
   } \
   catch(...) \
   { \
      OpenGPS::ExceptionHistory::SetLastException(); \
   } \
   return FALSE;


#define _OPENGPS_GENERIC_EXCEPTION_HANDLER(STATEMENT, CLEANUP_STATEMENT) \
   OpenGPS::ExceptionHistory::Reset(); \
   try \
   { \
      STATEMENT; \
   } \
   catch(const OpenGPS::Exception& ex) \
   { \
      CLEANUP_STATEMENT; \
      OpenGPS::ExceptionHistory::SetLastException(ex); \
   } \
   catch(...) \
   { \
      CLEANUP_STATEMENT; \
      OpenGPS::ExceptionHistory::SetLastException(); \
   }

#define _OPENGPS_GENERIC_EXCEPTION_HANDLER_TRYONLY(STATEMENT) \
   OpenGPS::ExceptionHistory::Reset(); \
   try \
   { \
      STATEMENT; \
   } \
   catch(const OpenGPS::Exception& ex) \
   { \
      OpenGPS::ExceptionHistory::SetLastException(ex); \
   } \
   catch(...) \
   { \
      OpenGPS::ExceptionHistory::SetLastException(); \
   }

#endif /* _OPENGPS_EXCEPTIONS_HXX */
