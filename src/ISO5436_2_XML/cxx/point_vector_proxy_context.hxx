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

#ifndef _OPENGPS_POINT_VECTOR_PROXY_CONTEXT_HXX
#define _OPENGPS_POINT_VECTOR_PROXY_CONTEXT_HXX

namespace OpenGPS
{
   // TODO: context für matrix und list, builder bauen
   // bei List muss U=V=W sein
   // Rename: VectorBufferContext
   class PointVectorProxyContext
   {
   public:
      PointVectorProxyContext();
      virtual ~PointVectorProxyContext();

      virtual void SetU(const unsigned long value);
      virtual void SetV(const unsigned long value);
      virtual void SetW(const unsigned long value);

      virtual unsigned long GetU() const;
      virtual unsigned long GetV() const;
      virtual unsigned long GetW() const;

   private:
      unsigned long m_U;
      unsigned long m_V;
      unsigned long m_W;
   };
}

#endif /* _OPENGPS_POINT_VECTOR_PROXY_CONTEXT_HXX */
