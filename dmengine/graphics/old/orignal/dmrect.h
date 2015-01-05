/*
   Copyright (C) 2012-2013 Xiongfa Li
   All rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef DMRECT_H
#define DMRECT_H

#include "dmpoint.h"
#include "dmnamespace.h"

DM_BEGIN_NAMESPACE
class DM_DECL_EXPORT Rect
{
public:
    Rect():
        m_x1(0),
        m_y1(0),
        m_x2(0),
        m_y2(0)
    {
    }

    Rect(dint32 nX, dint32 nY, dint32 nWidth, dint32 nHeight)
    {
        set(nX, nY, nWidth, nHeight);
    }

    Rect(const Rect& other):
        m_x1(other.m_x1),
        m_y1(other.m_y1),
        m_x2(other.m_x2),
        m_y2(other.m_y2)
    {
    }

    dint32 x() const { return m_x1; }
    dint32 y() const { return m_y1; }
    dint32 width() const { return (m_x2 - m_x1); }
    dint32 height() const { return (m_y2 - m_y1); }
    dint32 top() const { return m_y1; }
    dint32 bottom() const { return m_y2; }
    dint32 left() const { return m_x1; }
    dint32 right() const { return m_x2; }

    Point center() const;
    Point topLeft() const;
    Point topRight() const;
    Point bottomLeft() const;
    Point bottomRight() const;

    void setX(dint32 nX);
    void setY(dint32 nY);
    void setWidth(dint32 nWidth);
    void setHeight(dint32 nHeight);
    void set(dint32 nX, dint32 nY, dint32 nWidth, dint32 nHeight);
    void setCenter(const Point &pos);

    dbool contains(dint32 nX, dint32 nY, dbool proper = false) const;
    dbool contains(const Point &point, dbool proper = false) const;
    dbool contains(const Rect &other, dbool proper = false) const;

    dbool intersects(const Rect& other) const;
    Rect intersected(const Rect &other) const;

    dbool isEmpty();
    void clean();

    Rect& operator=(const Rect &other);
    dbool operator==(const Rect &other) const;
    dbool operator!=(const Rect &other) const;
    const Rect operator&(const Rect &other) const;

    friend class RectF;
private:
    dint32 m_x1;
    dint32 m_y1;
    dint32 m_x2;
    dint32 m_y2;
};

inline Point Rect::center()const
{
    return Point((m_x1+m_x2)>>1, (m_y1+m_y2)>>1);
}

inline Point Rect::topLeft()const
{
    return Point(m_x1, m_y1);
}

inline Point Rect::topRight() const
{
    return Point(m_x2, m_y1);
}

inline Point Rect::bottomLeft() const
{
    return Point(m_x1, m_y2);
}

inline Point Rect::bottomRight() const
{
    return Point(m_x2, m_y2);
}

inline void Rect::setX(dint32 nX)
{
    m_x2 += (nX - m_x1);
    m_x1 = nX;
}

inline void Rect::setY(dint32 nY)
{
    m_y2 += (nY - m_y1);
    m_y1 = nY;
}

inline void Rect::setWidth(dint32 nWidth)
{
    if (nWidth < 0)
    {
        m_x2 = m_x1;
        m_x1 += nWidth;
    }
    else
    {
        m_x2 = m_x1 + nWidth;
    }
}

inline void Rect::setHeight(dint32 nHeight)
{
    if (nHeight < 0)
    {
        m_y2 = m_y1;
        m_y1 += nHeight;
    }
    else
    {
        m_y2 = m_y1 + nHeight;
    }
}

inline void Rect::set(dint32 nX, dint32 nY, dint32 nWidth, dint32 nHeight)
{
    setX(nX);
    setY(nY);
    setWidth(nWidth);
    setHeight(nHeight);
}

inline void Rect::setCenter(const Point& pos)
{
    dint32 w = m_x2 - m_x1;
    dint32 h = m_y2 - m_y1;
    m_x1 = pos.x() - (w>>1);
    m_y1 = pos.y() - (h>>1);
    m_x2 = m_x1 + w;
    m_y2 = m_y1 + h;
}

inline dbool Rect::contains(dint32 nX, dint32 nY, dbool proper/* = false*/) const
{
#if 0
    if (nX >= m_x1 && nX <= m_x2 && nY >= m_y1 && nY <= m_y2)
    {
        return true;
    }
    else
    {
        return false;
    }
#endif
    if (proper)
    {
        if(nX <= m_x1 || nX >= m_x2 || nY <= m_y1 || nY >= m_y2)
        {
            return false;
        }
    }
    else
    {
        if(nX < m_x1 || nX > m_x2 || nY < m_y1 || nY > m_y2)
        {
            return false;
        }
    }

    return true;
}

inline dbool Rect::contains(const Point& point, dbool proper/* = false*/) const
{
    return contains(point.x(), point.y(), proper);
}

inline dbool Rect::contains(const Rect& other, dbool proper/* = false*/) const
{
    if (proper)
    {
        if (other.m_x1 <= m_x1 || other.m_x2 >= m_x2)
            return false;
    }
    else
    {
        if (other.m_x1 < m_x1 || other.m_x2 > m_x2)
            return false;
    }

    if (proper)
    {
        if (other.m_y1 <= m_y1 || other.m_y2 >= m_y2)
            return false;
    }
    else
    {
        if (other.m_y1 < m_y1 || other.m_y2 > m_y2)
            return false;
    }

    return true;
}

inline dbool Rect::intersects(const Rect& other) const
{
    if (m_x1 > other.m_x2 || m_x2 < other.m_x1)
    {
        return false;
    }

    if (m_y1 > other.m_y2 || m_y2 < other.m_y1)
    {
        return false;
    }

    return true;
}

inline Rect Rect::intersected(const Rect& other) const
{
    if (m_x1 > other.m_x2 || m_x2 < other.m_x1)
    {
        return Rect();
    }

    if (m_y1 > other.m_y2 || m_y2 < other.m_y1)
    {
        return Rect();
    }

    Rect tmp;
    tmp.m_x1 = Max(m_x1, other.m_x1);
    tmp.m_y1 = Max(m_y1, other.m_y1);
    tmp.m_x2 = Min(m_x2, other.m_x2);
    tmp.m_y2 = Min(m_y2, other.m_y2);

    return tmp;
}

inline dbool Rect::isEmpty()
{
    if ((m_x2-m_x1)!=0 || (m_y2-m_y1)!=0)
        return false;
    else
        return true;
}

inline void Rect::clean()
{
    m_x1 = 0;
    m_y1 = 0;
    m_x2 = 0;
    m_y2 = 0;
}

inline Rect& Rect::operator=(const Rect& other)
{
    m_x1 = other.m_x1;
    m_y1 = other.m_y1;
    m_x2 = other.m_x2;
    m_y2 = other.m_y2;

    return (*this);
}

inline dbool Rect::operator==(const Rect& other) const
{
    if (m_x1 == other.m_x1 &&           \
        m_y1 == other.m_y1 &&           \
        m_x2 == other.m_x2 &&           \
        m_y2 == other.m_y2              \
    )
        return true;
    else
        return false;
}

inline dbool Rect::operator!=(const Rect& other) const
{
    return (!(*this == other));
}

inline const Rect Rect::operator&(const Rect& other) const
{
    return intersected(other);
}

class DM_DECL_EXPORT RectF
{
public:
    RectF():
        m_x1(0),
        m_y1(0),
        m_x2(0),
        m_y2(0)
    {
    }

    RectF(dreal rX, dreal rY, dreal rWidth, dreal rHeight)
    {
        set(rX, rY, rWidth, rHeight);
    }

    RectF(const Rect& other):
        m_x1(static_cast<dreal>(other.m_x1)),
        m_y1(static_cast<dreal>(other.m_y1)),
        m_x2(static_cast<dreal>(other.m_x2)),
        m_y2(static_cast<dreal>(other.m_y2))
    {
    }

    RectF(const RectF& other):
        m_x1(other.m_x1),
        m_y1(other.m_y1),
        m_x2(other.m_x2),
        m_y2(other.m_y2)
    {
    }

    dreal x() const { return m_x1; }
    dreal y() const { return m_y1; }
    dreal width() const { return (m_x2 - m_x1); }
    dreal height() const { return (m_y2 - m_y1); }
    dreal top() const { return m_y1; }
    dreal bottom() const { return m_y2; }
    dreal left() const { return m_x1; }
    dreal right() const { return m_x2; }

    PointF center() const;
    PointF topLeft() const;
    PointF topRight() const;
    PointF bottomLeft() const;
    PointF bottomRight() const;

    void setX(dreal rX);
    void setY(dreal rY);
    void setWidth(dreal rWidth);
    void setHeight(dreal rHeight);
    void set(dreal rX, dreal rY, dreal rWidth, dreal rHeight);
    void setCenter(const PointF &pos);

    dbool contains(dreal rX, dreal rY, dbool proper = false) const;
    dbool contains(const PointF &point, dbool proper = false) const;
    dbool contains(const RectF &other, dbool proper = false) const;

    dbool intersects(const RectF& other) const;
    RectF intersected(const RectF &other) const;

    dbool isEmpty();
    void clean();

    RectF& operator=(const Rect &other);
    RectF& operator=(const RectF &other);
    dbool operator==(const RectF &other) const;
    dbool operator!=(const RectF &other) const;
    const RectF operator&(const RectF &other) const;

private:
    dreal m_x1;
    dreal m_y1;
    dreal m_x2;
    dreal m_y2;
};

inline PointF RectF::center()const
{
    return PointF((m_x1+m_x2)/2, (m_y1+m_y2)/2);
}

inline PointF RectF::topLeft()const
{
    return PointF(m_x1, m_y1);
}

inline PointF RectF::topRight() const
{
    return PointF(m_x2, m_y1);
}

inline PointF RectF::bottomLeft() const
{
    return PointF(m_x1, m_y2);
}

inline PointF RectF::bottomRight() const
{
    return PointF(m_x2, m_y2);
}

inline void RectF::setX(dreal rX)
{
    m_x2 += (rX - m_x1);
    m_x1 = rX;
}

inline void RectF::setY(dreal rY)
{
    m_y2 += (rY - m_y1);
    m_y1 = rY;
}

inline void RectF::setWidth(dreal rWidth)
{
    if (rWidth < 0)
    {
        m_x2 = m_x1;
        m_x1 += rWidth;
    }
    else
    {
        m_x2 = m_x1 + rWidth;
    }
}

inline void RectF::setHeight(dreal rHeight)
{
    if (rHeight < 0)
    {
        m_y2 = m_y1;
        m_y1 += rHeight;
    }
    else
    {
        m_y2 = m_y1 + rHeight;
    }
}

inline void RectF::set(dreal rX, dreal rY, dreal rWidth, dreal rHeight)
{
    setX(rX);
    setY(rY);
    setWidth(rWidth);
    setHeight(rHeight);
}

inline void RectF::setCenter(const PointF& pos)
{
    dreal w = m_x2 - m_x1;
    dreal h = m_y2 - m_y1;
    m_x1 = pos.x() - w/2;
    m_y1 = pos.y() - h/2;
    m_x2 = m_x1 + w;
    m_y2 = m_y1 + h;
}

inline dbool RectF::contains(dreal nX, dreal nY, dbool proper/* = false*/) const
{
#if 0
    if (nX >= m_x1 && nX <= m_x2 && nY >= m_y1 && nY <= m_y2)
    {
        return true;
    }
    else
    {
        return false;
    }
#endif
    if (proper)
    {
        if(nX <= m_x1 || nX >= m_x2 || nY <= m_y1 || nY >= m_y2)
        {
            return false;
        }
    }
    else
    {
        if(nX < m_x1 || nX > m_x2 || nY < m_y1 || nY > m_y2)
        {
            return false;
        }
    }

    return true;
}

inline dbool RectF::contains(const PointF& point, dbool proper/* = false*/) const
{
    return contains(point.x(), point.y(), proper);
}

inline dbool RectF::contains(const RectF& other, dbool proper/* = false*/) const
{
    if (proper)
    {
        if (other.m_x1 <= m_x1 || other.m_x2 >= m_x2)
            return false;
    }
    else
    {
        if (other.m_x1 < m_x1 || other.m_x2 > m_x2)
            return false;
    }

    if (proper)
    {
        if (other.m_y1 <= m_y1 || other.m_y2 >= m_y2)
            return false;
    }
    else
    {
        if (other.m_y1 < m_y1 || other.m_y2 > m_y2)
            return false;
    }

    return true;
}

inline dbool RectF::intersects(const RectF& other) const
{
    if (m_x1 > other.m_x2 || m_x2 < other.m_x1)
    {
        return false;
    }

    if (m_y1 > other.m_y2 || m_y2 < other.m_y1)
    {
        return false;
    }

    return true;
}

inline RectF RectF::intersected(const RectF& other) const
{
    if (m_x1 > other.m_x2 || m_x2 < other.m_x1)
    {
        return RectF();
    }

    if (m_y1 > other.m_y2 || m_y2 < other.m_y1)
    {
        return RectF();
    }

    RectF tmp;
    tmp.m_x1 = Max(m_x1, other.m_x1);
    tmp.m_y1 = Max(m_y1, other.m_y1);
    tmp.m_x2 = Min(m_x2, other.m_x2);
    tmp.m_y2 = Min(m_y2, other.m_y2);

    return tmp;
}

inline dbool RectF::isEmpty()
{
    if ((m_x2-m_x1)!=0 || (m_y2-m_y1)!=0)
        return false;
    else
        return true;
}

inline void RectF::clean()
{
    m_x1 = 0;
    m_y1 = 0;
    m_x2 = 0;
    m_y2 = 0;
}

inline RectF& RectF::operator=(const Rect& other)
{
    m_x1 = static_cast<dreal>(other.m_x1);
    m_y1 = static_cast<dreal>(other.m_y1);
    m_x2 = static_cast<dreal>(other.m_x2);
    m_y2 = static_cast<dreal>(other.m_y2);

    return (*this);
}


inline RectF& RectF::operator=(const RectF& other)
{
    m_x1 = other.m_x1;
    m_y1 = other.m_y1;
    m_x2 = other.m_x2;
    m_y2 = other.m_y2;

    return (*this);
}

inline dbool RectF::operator==(const RectF& other) const
{
    if (m_x1 == other.m_x1 &&           \
        m_y1 == other.m_y1 &&           \
        m_x2 == other.m_x2 &&           \
        m_y2 == other.m_y2              \
    )
        return true;
    else
        return false;
}

inline dbool RectF::operator!=(const RectF& other) const
{
    return (!(*this == other));
}

inline const RectF RectF::operator&(const RectF& other) const
{
    return intersected(other);
}

template<class A, class B>
A convertRect(const B &b)
{
    return A(b.x(), b.y(), b.width(), b.height());
}

DM_END_NAMESPACE
#endif // DMRECT_H
