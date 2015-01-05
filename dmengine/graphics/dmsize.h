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

#ifndef DMSIZE_H
#define DMSIZE_H
#include "dmnamespace.h"
#include "dmmath.h"

DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT Size
{
public:
    inline Size(): w(0), h(0) {}
    inline Size(int nWidth, int nHeight): w(nWidth), h(nHeight) {}
    inline Size(const Size &other): w(other.w), h(other.h) {}

    inline int width() const;
    inline int height() const;

    inline void setWidth(dint nWidth);
    inline void setHeight(dint nHeight);

    inline dbool isEmpty() const;

    dbool operator==(const Size &other) const
    {
        if (w == other.w && \
            h == other.h)
            return true;
        else
            return false;
    }

    dbool operator!=(const Size &other) const
    {
        return !(*this == other);
    }

    Size& operator=(const Size &other)
    {
        w = other.w;
        h = other.h;

        return (*this);
    }

    Size &operator+=(const Size &);
    Size &operator-=(const Size &);
    Size &operator*=(dreal c);
    Size &operator/=(dreal c);

    friend inline const Size operator+(const Size &, const Size &);
    friend inline const Size operator-(const Size &, const Size &);
    friend inline const Size operator*(const Size &, dreal);
    friend inline const Size operator*(dreal, const Size &);
    friend inline const Size operator/(const Size &, dreal);
    friend class SizeF;
private:
    dint w;
    dint h;
};

DM_DECLARE_TYPEINFO(Size, DM_MOVABLE_TYPE);

inline int Size::width() const
{
    return w;
}

inline int Size::height() const
{
    return h;
}

inline void Size::setWidth(dint nWidth)
{
    w = nWidth;
}

inline void Size::setHeight(dint nHeight)
{
    h = nHeight;
}

inline dbool Size::isEmpty() const
{
    return (w == 0 && h == 0);
}

inline Size &Size::operator+=(const Size &s)
{ w += s.w; h += s.h; return *this; }

inline Size &Size::operator-=(const Size &s)
{ w -= s.w; h -= s.h; return *this; }

inline Size &Size::operator*=(dreal c)
{ w = Round(w*c); h = Round(h*c); return *this; }

inline const Size operator+(const Size & s1, const Size & s2)
{ return Size(s1.w + s2.w, s1.h + s2.h); }

inline const Size operator-(const Size &s1, const Size &s2)
{ return Size(s1.w - s2.w, s1.h - s2.h); }

inline const Size operator*(const Size &s, dreal c)
{ return Size(Round(s.w*c), Round(s.h*c)); }

inline const Size operator*(dreal c, const Size &s)
{ return Size(Round(s.w*c), Round(s.h*c)); }

inline Size &Size::operator/=(dreal c)
{
    DM_ASSERT(!FuzzyIsNull(c));
    w = Round(w/c); h = Round(h/c);
    return *this;
}

inline const Size operator/(const Size &s, dreal c)
{
    DM_ASSERT(!FuzzyIsNull(c));
    return Size(Round(s.w/c), Round(s.h/c));
}

class DM_DLL_EXPORT SizeF
{
public:
    inline SizeF(): w(0), h(0) {}
    inline SizeF(dreal rWidth, dreal rHeight): w(rWidth), h(rHeight) {}
    inline SizeF(const Size &other): w(static_cast<dreal>(other.w)), h(static_cast<dreal>(other.h)) {}
    inline SizeF(const SizeF &other): w(other.w), h(other.h) {}

    inline dreal width() const;
    inline dreal height() const;

    inline void setWidth(dreal rWidth);
    inline void setHeight(dreal rHeight);

    inline dbool isEmpty() const;

    inline Size toSize() const;

    dbool operator==(const SizeF &other) const
    {
        if (IsEqual(w, other.w) && \
            IsEqual(h, other.h))
            return true;
        else
            return false;
    }

    dbool operator!=(const SizeF &other) const
    {
        return !(*this == other);
    }

    SizeF& operator=(const Size &other)
    {
        w = static_cast<dreal>(other.w);
        h = static_cast<dreal>(other.h);

        return (*this);
    }

    SizeF& operator=(const SizeF &other)
    {
        w = other.w;
        h = other.h;

        return (*this);
    }

    SizeF &operator+=(const SizeF &);
    SizeF &operator-=(const SizeF &);
    SizeF &operator*=(dreal c);
    SizeF &operator/=(dreal c);

    friend inline const SizeF operator+(const SizeF &, const SizeF &);
    friend inline const SizeF operator-(const SizeF &, const SizeF &);
    friend inline const SizeF operator*(const SizeF &, dreal);
    friend inline const SizeF operator*(dreal, const SizeF &);
    friend inline const SizeF operator/(const SizeF &, dreal);
private:
    dreal w;
    dreal h;
};

DM_DECLARE_TYPEINFO(SizeF, DM_MOVABLE_TYPE);

inline dreal SizeF::width() const
{
    return w;
}

inline dreal SizeF::height() const
{
    return h;
}

inline void SizeF::setWidth(dreal rWidth)
{
    w = rWidth;
}

inline void SizeF::setHeight(dreal rHeight)
{
    h = rHeight;
}

inline dbool SizeF::isEmpty() const
{
    return (FuzzyIsNull(w) && FuzzyIsNull(h));
}

inline Size SizeF::toSize() const
{
    return Size(static_cast<dint>(w), static_cast<dint>(h));
}

inline SizeF &SizeF::operator+=(const SizeF &s)
{ w += s.w; h += s.h; return *this; }

inline SizeF &SizeF::operator-=(const SizeF &s)
{ w -= s.w; h -= s.h; return *this; }

inline SizeF &SizeF::operator*=(dreal c)
{ w = w*c; h = h*c; return *this; }

inline const SizeF operator+(const SizeF & s1, const SizeF & s2)
{ return SizeF(s1.w + s2.w, s1.h + s2.h); }

inline const SizeF operator-(const SizeF &s1, const SizeF &s2)
{ return SizeF(s1.w - s2.w, s1.h - s2.h); }

inline const SizeF operator*(const SizeF &s, dreal c)
{ return SizeF(s.w*c, s.h*c); }

inline const SizeF operator*(dreal c, const SizeF &s)
{ return SizeF(s.w*c, s.h*c); }

inline SizeF &SizeF::operator/=(dreal c)
{
    DM_ASSERT(!FuzzyIsNull(c));
    w = w/c; h = h/c;
    return *this;
}

inline const SizeF operator/(const SizeF &s, dreal c)
{
    DM_ASSERT(!FuzzyIsNull(c));
    return SizeF(s.w/c, s.h/c);
}
DM_END_NAMESPACE

#endif // DMSIZE_H
