/*
   Copyright (C) 2012-2014 Xiongfa Li, <damao1222@live.com>
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
class DM_DLL_EXPORT Rect
{
public:
    Rect():
        m_x1(0),
        m_y1(0),
        m_x2(0),
        m_y2(0)
    {
    }

    Rect(dint nX, dint nY, dint nWidth, dint nHeight)
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

    dint x() const { return m_x1; }
    dint y() const { return m_y1; }
    dint width() const { return (m_x2 - m_x1); }
    dint height() const { return (m_y2 - m_y1); }
    dint top() const { return m_y1; }
    dint bottom() const { return m_y2; }
    dint left() const { return m_x1; }
    dint right() const { return m_x2; }

    Point center() const;
    Point topLeft() const;
    Point topRight() const;
    Point bottomLeft() const;
    Point bottomRight() const;

    void setX(dint nX);
    void setY(dint nY);
    void setWidth(dint nWidth);
    void setHeight(dint nHeight);
    void set(dint nX, dint nY, dint nWidth, dint nHeight);
    void setCenter(const Point &pos);

    dbool contains(dint nX, dint nY, dbool proper = false) const;
    dbool contains(const Point &point, dbool proper = false) const;
    dbool contains(const Rect &other, dbool proper = false) const;

    dbool intersects(const Rect& other) const;
    Rect intersected(const Rect &other) const;

    dbool isEmpty() const;
    dbool isNull() const;
    void clean();

    Rect& operator=(const Rect &other);
    dbool operator==(const Rect &other) const;
    dbool operator!=(const Rect &other) const;
    const Rect operator&(const Rect &other) const;

    friend class RectF;
private:
    dint m_x1;
    dint m_y1;
    dint m_x2;
    dint m_y2;
};

DM_DECLARE_TYPEINFO(Rect, DM_MOVABLE_TYPE);

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

inline void Rect::setX(dint nX)
{
    m_x2 += (nX - m_x1);
    m_x1 = nX;
}

inline void Rect::setY(dint nY)
{
    m_y2 += (nY - m_y1);
    m_y1 = nY;
}

inline void Rect::setWidth(dint nWidth)
{
    if (nWidth > 0)
    {
        m_x2 = m_x1 + nWidth;
    }
    else
    {
        m_x2 = m_x1;
        m_x1 += nWidth;
    }
}

inline void Rect::setHeight(dint nHeight)
{
    if (nHeight > 0)
    {
        m_y2 = m_y1 + nHeight;
    }
    else
    {
        m_y2 = m_y1;
        m_y1 += nHeight;
    }
}

inline void Rect::set(dint nX, dint nY, dint nWidth, dint nHeight)
{
    if (nWidth > 0)
    {
        m_x1 = nX; m_x2 = nX + nWidth;
    }
    else
    {
        m_x2 = nX; m_x1 = nX + nWidth;
    }

    if (nHeight > 0)
    {
        m_y1 = nY; m_y2 = nY + nHeight;
    }
    else
    {
        m_y2 = nY; m_y1 = nY + nHeight;
    }
}

inline void Rect::setCenter(const Point& pos)
{
    dint w = m_x2 - m_x1;
    dint h = m_y2 - m_y1;
    m_x1 = pos.x() - (w>>1);
    m_y1 = pos.y() - (h>>1);
    m_x2 = m_x1 + w;
    m_y2 = m_y1 + h;
}

inline dbool Rect::contains(dint nX, dint nY, dbool proper/* = false*/) const
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
    if (!proper)
    {
        if(nX < m_x1 || nX > m_x2 || nY < m_y1 || nY > m_y2)
        {
            return false;
        }
    }
    else
    {
        if(nX <= m_x1 || nX >= m_x2 || nY <= m_y1 || nY >= m_y2)
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
    if (!proper)
    {
        if (other.m_x1 < m_x1 || other.m_x2 > m_x2)
            return false;
    }
    else
    {
        if (other.m_x1 <= m_x1 || other.m_x2 >= m_x2)
            return false;
    }

    if (!proper)
    {
        if (other.m_y1 < m_y1 || other.m_y2 > m_y2)
            return false;
    }
    else
    {
        if (other.m_y1 <= m_y1 || other.m_y2 >= m_y2)
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

inline dbool Rect::isEmpty() const
{
    if (m_x2 == m_x1 && m_y2 == m_y1)
        return true;
    else
        return false;
}

inline dbool Rect::isNull() const
{
    return isEmpty();
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

class DM_DLL_EXPORT RectF
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
    dreal halfWidth() const { return width()/2; }
    dreal halfHeight() const { return height()/2; }

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

    dbool isEmpty() const;
    dbool isNull() const;
    void clean();

    Rect toRect() const;

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

DM_DECLARE_TYPEINFO(RectF, DM_MOVABLE_TYPE);

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
    if (rWidth > 0)
    {
        m_x2 = m_x1 + rWidth;
    }
    else
    {
        m_x2 = m_x1;
        m_x1 += rWidth;
    }
}

inline void RectF::setHeight(dreal rHeight)
{
    if (rHeight > 0)
    {
        m_y2 = m_y1 + rHeight;
    }
    else
    {
        m_y2 = m_y1;
        m_y1 += rHeight;
    }
}

inline void RectF::set(dreal rX, dreal rY, dreal rWidth, dreal rHeight)
{
    if (rWidth > 0)
    {
        m_x1 = rX; m_x2 = rX + rWidth;
    }
    else
    {
        m_x2 = rX; m_x1 = rX + rWidth;
    }

    if (rHeight > 0)
    {
        m_y1 = rY; m_y2 = rY + rHeight;
    }
    else
    {
        m_y2 = rY; m_y1 = rY + rHeight;
    }
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
    if (!proper)
    {
        if(nX < m_x1 || nX > m_x2 || nY < m_y1 || nY > m_y2)
        {
            return false;
        }
    }
    else
    {
        if(nX <= m_x1 || nX >= m_x2 || nY <= m_y1 || nY >= m_y2)
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
    if (!proper)
    {
        if (other.m_x1 < m_x1 || other.m_x2 > m_x2)
            return false;
    }
    else
    {
        if (other.m_x1 <= m_x1 || other.m_x2 >= m_x2)
            return false;
    }

    if (!proper)
    {
        if (other.m_y1 < m_y1 || other.m_y2 > m_y2)
            return false;
    }
    else
    {
        if (other.m_y1 <= m_y1 || other.m_y2 >= m_y2)
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

inline dbool RectF::isEmpty() const
{
    if (width() > 0 || height() > 0)
        return false;
    else
        return true;
}

inline dbool RectF::isNull() const
{
    return isEmpty();
}

inline void RectF::clean()
{
    m_x1 = 0;
    m_y1 = 0;
    m_x2 = 0;
    m_y2 = 0;
}

inline Rect RectF::toRect() const
{
    Rect r;
    r.m_x1 = static_cast<dint>(m_x1);
    r.m_y1 = static_cast<dint>(m_y1);
    r.m_x2 = static_cast<dint>(m_x2);
    r.m_y2 = static_cast<dint>(m_y2);
    return r;
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
    if (IsEqual(m_x1, other.m_x1) &&           \
        IsEqual(m_y1, other.m_y1) &&           \
        IsEqual(m_x2, other.m_x2) &&           \
        IsEqual(m_y2, other.m_y2)              \
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
