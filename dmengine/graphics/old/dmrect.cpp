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
Rect Rect::intersected(const Rect& other) const
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

RectF RectF::intersected(const RectF& other) const
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
DM_END_NAMESPACE