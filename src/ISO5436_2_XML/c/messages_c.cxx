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

#include <opengps/messages.h>
#include "messages_c.hxx"

const OGPS_Character* ogps_GetErrorMessage()
{
   return OpenGPS::ExceptionHistory::GetLastErrorMessage();
}

const OGPS_Character* ogps_GetErrorDescription()
{
   return OpenGPS::ExceptionHistory::GetLastErrorDescription();
}

OGPS_ExceptionId ogps_GetErrorId()
{
   return OpenGPS::ExceptionHistory::GetLastErrorId();
}

OGPS_Boolean ogps_HasError()
{
   return OpenGPS::ExceptionHistory::GetLastErrorId() != OGPS_ExNone;
}

void OpenGPS::ExceptionHistory::SetLastException(const OpenGPS::Exception& ex)
{
   OpenGPS::String msg;
   msg.FromChar(ex.what());

   m_LastErrorMessage = msg;
   m_LastErrorDescription = ex.details();
   m_LastErrorId = ex.id();
}

void OpenGPS::ExceptionHistory::SetLastException(const std::exception& ex)
{
   OpenGPS::String msg;
   msg.FromChar(ex.what());

   m_LastErrorMessage = msg;
   m_LastErrorDescription.clear();
   m_LastErrorId = OGPS_ExGeneral;
}

void OpenGPS::ExceptionHistory::SetLastException()
{
   /* An exception occured, but no further details are known. */
   m_LastErrorMessage.clear();
   m_LastErrorDescription.clear();

   /* At least there must have been an exception... */
   m_LastErrorId = OGPS_ExGeneral;
}


void OpenGPS::ExceptionHistory::Reset()
{
   m_LastErrorMessage.clear();
   m_LastErrorDescription.clear();
   m_LastErrorId = OGPS_ExNone;
}

const OGPS_Character* OpenGPS::ExceptionHistory::GetLastErrorMessage()
{
   return m_LastErrorMessage.empty() ? NULL : m_LastErrorMessage.c_str();
}

const OGPS_Character* OpenGPS::ExceptionHistory::GetLastErrorDescription()
{
   return m_LastErrorDescription.empty() ? NULL : m_LastErrorDescription.c_str();
}

OGPS_ExceptionId OpenGPS::ExceptionHistory::GetLastErrorId()
{
   return m_LastErrorId;
}

OpenGPS::ExceptionHistory::ExceptionHistory()
{
}

OpenGPS::ExceptionHistory::~ExceptionHistory()
{
}

OpenGPS::String OpenGPS::ExceptionHistory::m_LastErrorMessage;
OpenGPS::String OpenGPS::ExceptionHistory::m_LastErrorDescription;
OGPS_ExceptionId OpenGPS::ExceptionHistory::m_LastErrorId = OGPS_ExNone;
