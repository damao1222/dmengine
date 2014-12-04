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
#include "dmpoint.h"
DM_USING_NAMESPACE;
Point Point::bezier(const Point &p1, const Point &p2, const Point &p3, dreal t )
{
    Point target = (1-t)*(1-t)*p1 + 2*t*(1-t)*p2 + t*t*p3;
    return target;
}
