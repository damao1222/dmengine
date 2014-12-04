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
#include "dmrandom.h"
#include <stdlib.h>
#include "dmtime.h"

DM_USING_NAMESPACE;
void Random::shuffle()
{
    ::srand(Time::getSecond());
}

duint32 Random::getRandom()
{
    return ::rand();
}

duint32 Random::getRandom(duint32 nFrom, duint32 nTo)
{
    if (nFrom == nTo)
        return 0;

    if (nFrom < nTo)
        return (getRandom()%(nTo-nFrom+1)+nFrom);
    else
        return (getRandom()%(nFrom-nTo+1)+nTo);
}

dreal Random::getRandom_0_1()
{
    return((dreal)rand()/RAND_MAX);
}
