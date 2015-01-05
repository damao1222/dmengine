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


#ifndef DMDIRECTION_H
#define DMDIRECTION_H
#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
enum DirectionValue
{
    eDirectionInvalid   = 0,
    eDirectionLeft      = 1,
    eDirectionRight     = 1<<1,
    eDirectionLeftRight = eDirectionLeft | eDirectionRight,
    eDirectionUp        = 1<<2,
    eDirectionLeftUp    = eDirectionLeft | eDirectionUp,
    eDirectionRightUp   = eDirectionRight | eDirectionUp,
    eDirectionDown      = 1<<3,
    eDirectionLeftDown  = eDirectionLeft | eDirectionDown,
    eDirectionRightDown = eDirectionRight | eDirectionDown,
    eDirectionUpDown    = eDirectionUp | eDirectionDown,
    eDirectionMask      = 0x0F
};

class DM_DLL_EXPORT Direction
{

public:
    inline Direction();
    inline Direction(dint value);
    inline Direction(const Direction &other);

    inline duint8 getData() const;
    inline dbool left() const;
    inline dbool right() const;
    inline dbool up() const;
    inline dbool down() const;

    inline void setLeft(dbool flag);
    inline void setRight(dbool flag);
    inline void setUp(dbool flag);
    inline void setDown(dbool flag);

    inline duint8 count() const;
    inline dbool valid() const;
    Direction reverse() const;
    Direction& reverse_self();

    inline dbool operator==(const Direction &other) const;
    inline dbool operator!=(const Direction &other) const;
    inline Direction& operator=(const Direction &other);
    inline Direction& operator=(dint value);
    inline Direction& operator&=(const Direction &other);
    inline Direction& operator|=(const Direction &other);

    inline Direction operator&(const Direction &other) const;
    inline Direction operator|(const Direction &other) const;

    static Direction getReverse(const Direction &other);
private:
    duint8 data;
};

DM_DECLARE_TYPEINFO(Direction, DM_MOVABLE_TYPE);

inline Direction::Direction() : data(0)
{}

inline Direction::Direction(dint value) : data(value & eDirectionMask)
{}

inline Direction::Direction(const Direction &other): data(other.data)
{}

inline duint8 Direction::getData() const
{
    return data;
}

inline dbool Direction::left() const
{
    return (data & eDirectionLeft) > 0;
}

inline dbool Direction::right() const
{
    return (data & eDirectionRight) > 0;
}

inline dbool Direction::up() const
{
    return (data & eDirectionUp) > 0;
}

inline dbool Direction::down() const
{
    return (data & eDirectionDown) > 0;
}

inline void Direction::setLeft(dbool flag)
{
    flag ? (data |= eDirectionLeft) : (data &= (~eDirectionLeft));
}

inline void Direction::setRight(dbool flag)
{
    flag ? (data |= eDirectionRight) : (data &= (~eDirectionRight));
}

inline void Direction::setUp(dbool flag)
{
    flag ? (data |= eDirectionUp) : (data &= (~eDirectionUp));
}

inline void Direction::setDown(dbool flag)
{
    flag ? (data |= eDirectionDown) : (data &= (~eDirectionDown));
}

inline duint8 Direction::count() const
{
    int i = 0;
    if (data & eDirectionLeft) ++i;
    if (data & eDirectionRight) ++i;
    if (data & eDirectionUp) ++i;
    if (data & eDirectionDown) ++i;

    return i;
}

inline dbool Direction::valid() const
{
    if (data == 0) return false;
    if (count() > 2) return false;

    return true;
}

inline dbool Direction::operator==(const Direction &other) const
{
    if (data == other.data)
        return true;
    else
        return false;
}

inline dbool Direction::operator!=(const Direction &other) const
{
    return !(*this == other);
}

inline Direction& Direction::operator=(const Direction &other)
{
    data = other.data;
    return (*this);
}

inline Direction& Direction::operator=(dint value)
{
    data = value & eDirectionMask;
    return (*this);
}

inline Direction& Direction::operator&=(const Direction &other)
{
    data &= other.data;
    return (*this);
}

inline Direction& Direction::operator|=(const Direction &other)
{
    data |= other.data;
    return (*this);
}

inline Direction Direction::operator&(const Direction &other) const
{
    Direction d(data);
    d.data &= other.data;
    return d;
}

inline Direction Direction::operator|(const Direction &other)const
{
    Direction d(data);
    d.data |= other.data;
    return d;
}
#if 0
inline dbool operator==(const Direction &d1, const Direction &d2)
{
    return d1 == d2;
}

inline dbool operator!=(const Direction &d1, const Direction &d2)
{
    return d1 != d2;
}
#endif
DM_END_NAMESPACE
#endif // DMDIRECTION_H
