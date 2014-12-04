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

#include "dmdatabuffer.h"
#include <new>

DM_BEGIN_NAMESPACE
DataBufferData::Data DataBufferData::shared_null = { DM_BASIC_ATOMIC_INITIALIZER(1), true, NULL, 0, false};

DataBufferData::Data* DataBufferData::creat(void* ptr, dint64 uSize, dbool bCopy)
{
    DataBufferData::Data* t = NULL;
    DM_TRY
    {
        t = new Data;
        t->ref = 1;
        t->sharable = true;
        t->ptr = ptr;
        t->uSize = uSize;
        t->bCopy = bCopy;
    }
    DM_CATCH (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t;
    }
    return t;
}

DataBufferData::Data* DataBufferData::detach()
{
    Data *x = d;
    Data* t = NULL;
    DM_TRY
    {
        t = new Data;

        t->ref = 1;
        t->sharable = true;
        t->ptr = x->ptr;
        t->uSize = x->uSize;
        t->bCopy = x->bCopy;

        d = t;
    }
    DM_CATCH (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t;
    }
    return x;
}

DataBufferData::Data* DataBufferData::detach(void* ptr, dint64 uSize, dbool bCopy)
{
    Data *x = d;
    Data* t = NULL;
    DM_TRY
    {
        t = new Data;

        t->ref = 1;
        t->sharable = true;
        t->ptr = ptr;
        t->uSize = uSize;
        t->bCopy = bCopy;

        d = t;
    }
    DM_CATCH (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t;
    }
    return x;
}

DM_END_NAMESPACE
