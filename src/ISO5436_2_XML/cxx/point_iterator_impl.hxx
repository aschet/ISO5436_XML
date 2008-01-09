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

#ifndef _OPENGPS_POINT_ITERATOR_IMPL_HXX
#define _OPENGPS_POINT_ITERATOR_IMPL_HXX

#ifndef _OPENGPS_CXX_POINT_ITERATOR_HXX
#  include <opengps/cxx/point_iterator.hxx>
#endif

#ifndef _OPENGPS_ISO5436_2_CONTAINER_HXX
#  include "iso5436_2_container.hxx"
#endif

namespace OpenGPS
{
   class PointIteratorImpl : public PointIterator
  {
  public:
     PointIteratorImpl(ISO5436_2Container * const handle,
   const OGPS_Boolean isForward,
   const OGPS_Boolean isMatrix);
    virtual ~PointIteratorImpl();
    
    virtual OGPS_Boolean HasNext() const;
    virtual OGPS_Boolean HasPrev() const;

    virtual OGPS_Boolean MoveNext();
    virtual OGPS_Boolean MovePrev();

    // TODO: virtual OGPS_Boolean CreateNext();

    virtual void ResetNext();
    virtual void ResetPrev();

    virtual OGPS_Boolean GetCurrent(PointVector& vector);

    virtual OGPS_Boolean SetCurrent(const PointVector* const vector);
    
    virtual OGPS_Boolean GetPosition(unsigned long* const index) const;
    virtual OGPS_Boolean GetPosition(
            unsigned long* const u,
            unsigned long* const v,
            unsigned long* const w) const;

    PointIteratorImpl& operator=(const PointIteratorImpl& src);
    
  private:
    ISO5436_2Container * const m_Handle;
            
    /* Forward- or backward iterator. */
    OGPS_Boolean m_IsForward;

    OGPS_Boolean m_IsReset;
    OGPS_Boolean m_IsMatrix;

    PointVector* m_Buffer;
  
    /* Current index. */
    unsigned long m_U; /* Matrix or list index. */
    unsigned long m_V; /* Matrix only. */
    unsigned long m_W; /* Matrix only. */
  };
}

#endif /* _OPENGPS_POINT_ITERATOR_IMPL_HXX */