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

#include "dmrefcount.h"
#include "dmlogger.h"
#include <new>

DM_BEGIN_NAMESPACE
RefData::Data RefData::shared_null = { DM_BASIC_ATOMIC_INITIALIZER(1), NULL };

RefData::Data *RefData::setExc(void *p)
{
    RefData::Data *x = d;
    if (p)
    {
        RefData::Data* t = NULL;
        DM_TRY
        {
            t = new RefData::Data;
            t->ptr = p;
            t->ref = 1;

            d = t;
        }
        DM_CATCH (const std::bad_alloc& ba)
        {
            DM_FATAL("%s", ba.what());
            delete t;
        }
    }
    else
    {
        d = &RefData::shared_null;
        d->ref.ref();
    }
    return x;
}

RefData::Data *RefData::create(void *p)
{
    if (p)
    {
        RefData::Data* t = NULL;
        DM_TRY
        {
            t = new RefData::Data;
            t->ptr = p;
            t->ref = 1;
        }
        DM_CATCH (const std::bad_alloc& ba)
        {
            DM_FATAL("%s", ba.what());
            delete t;
        }
        return t;
    }
    else
    {
        RefData::shared_null.ref.ref();
        return &RefData::shared_null;
    }
}

DM_END_NAMESPACE
