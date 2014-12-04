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

#include "dmdirection.h"
DM_BEGIN_NAMESPACE

Direction Direction::reverse() const
{
    Direction d(*this);
    d.reverse_self();
    return d;
}

Direction& Direction::reverse_self()
{
    if (count() > 1)
    {
        data = (~data) & eDirectionMask;
    }
    else
    {
        duint8 d = ~data;
        //up or down
        if (data > eDirectionLeftRight)
        {
            data = d & 0x0C;
        }
        //left or right
        else
        {
            data = d & 0x03;
        }
    }
    return (*this);
}

Direction Direction::getReverse(const Direction &other)
{
    return other.reverse();
}

DM_END_NAMESPACE

