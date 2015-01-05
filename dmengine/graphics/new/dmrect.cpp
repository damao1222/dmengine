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

#include "dmrect.h"

DM_BEGIN_NAMESPACE

dbool RectF::contains(dreal nX, dreal nY, dbool proper/* = false*/) const
{
    dreal x1 = left();
    dreal y1 = top();
    dreal x2 = right();
    dreal y2 = bottom();
    if (proper)
    {
        if(nX <= x1 || nX >= x2 || nY <= y1 || nY >= y2)
        {
            return false;
        }
    }
    else
    {
        if(nX < x1 || nX > x2 || nY < y1 || nY > y2)
        {
            return false;
        }
    }

    return true;
}

dbool RectF::contains(const RectF& other, dbool proper/* = false*/) const
{
    dreal x1 = left();
    dreal y1 = top();
    dreal x2 = right();
    dreal y2 = bottom();

    dreal o_x1 = other.left();
    dreal o_y1 = other.top();
    dreal o_x2 = other.right();
    dreal o_y2 = other.bottom();
    if (proper)
    {
        if (o_x1 <= x1 || o_x2 >= x2)
            return false;
    }
    else
    {
        if (o_x1 < x1 || o_x2 > x2)
            return false;
    }

    if (proper)
    {
        if (o_y1 <= y1 || o_y2 >= y2)
            return false;
    }
    else
    {
        if (o_y1 < y1 || o_y2 > y2)
            return false;
    }

    return true;
}

dbool RectF::intersects(const RectF& other) const
{
    if (left() > other.right() || right() < other.left())
    {
        return false;
    }

    if (top() > other.bottom() || bottom() < other.top())
    {
        return false;
    }

    return true;
}

RectF RectF::intersected(const RectF& other) const
{
    if (!intersects(other))
    {
        return RectF();
    }

    dreal x1 = Max(left(), other.left());
    dreal y1 = Max(top(), other.top());
    dreal x2 = Min(right(), other.right());
    dreal y2 = Min(bottom(), other.bottom());

    RectF tmp(x1, y1, x2 - x1, y2 - y1);

    return tmp;
}

DM_END_NAMESPACE

