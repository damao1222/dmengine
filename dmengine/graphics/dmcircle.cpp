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
#include "dmcircle.h"
DM_BEGIN_NAMESPACE
dbool Circle::intersects(const Rect &rect) const
{
    Point center = rect.center();
    dint rx = p.x() - center.x();
    dint ry = p.y() - center.y();
    dint w = rect.width() >> 1;
    dint h = rect.height() >> 1;

    dint dx = Min(rx, w);
    dx = Max(dx, -w);

    dint dy = Min(ry, h);
    dy = Max(dy, -h);

    dint x = rx - dx;
    dint y = ry - dy;
    return x*x + y*y <= r*r;
}

dbool CircleF::intersects(const RectF &rect) const
{
    PointF center = rect.center();
    dreal rx = p.x() - center.x();
    dreal ry = p.y() - center.y();
    dreal w = rect.halfWidth();
    dreal h = rect.halfHeight();

    dreal dx = Min(rx, w);
    dx = Max(dx, -w);

    dreal dy = Min(ry, h);
    dy = Max(dy, -h);

    dreal x = rx - dx;
    dreal y = ry - dy;
    return x*x + y*y <= r*r;
}
DM_END_NAMESPACE