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

#include "dmprobabilityutil.h"
#include "dmrandom.h"
#include "dmlogger.h"
DM_BEGIN_NAMESPACE

void ProbabilityUtil::shuffle()
{
    Random::shuffle();
}

dint ProbabilityUtil::choose(const dint *array, dint size)
{
    dint total = 0;
    for(int i=0; i<size; ++i)
    {
        total += array[i];
    }

    dint random = Random::getRandom(1, total);
    for (int i=0; i<size; ++i)
    {
        if (random <= array[i])
        {
            return i;
        }
        else
        {
            random -= array[i];
        }
    }
    return size-1;
}

dint ProbabilityUtil::choose(const dint *array, dint size, dint total)
{
#ifdef DM_DEBUG
    dint nothing = total;
    for(int i=0; i<size; ++i)
    {
        nothing -= array[i];
    }

    DM_ASSERT_X(nothing >= 0, "Probability count is bigger than total");
#endif
    dint random = Random::getRandom(1, total);
    for (int i=0; i<size; ++i)
    {
        if (random <= array[i])
        {
            return i;
        }
        else
        {
            random -= array[i];
        }
    }
    return size;
}

dint ProbabilityUtil::choose(const dreal *array, dint size)
{
#ifdef DM_DEBUG
    dreal nothing = 1;
    for(int i=0; i<size; ++i)
    {
        nothing -= array[i];
    }

    DM_ASSERT_X(nothing >= 0, "Probability count is bigger than total");
#endif
    dreal random = Random::getRandom_0_1();
    //if (random < 0.001) DM_LOGI("Probability is %g", random);
    for (int i=0; i<size; ++i)
    {
        if (random <= array[i])
        {
            return i;
        }
        else
        {
            random -= array[i];
        }
    }
    return size;
}

DM_END_NAMESPACE
