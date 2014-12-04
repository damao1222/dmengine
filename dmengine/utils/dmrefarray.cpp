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
#include "dmrefarray.h"
#include "dmvector.h"

DM_BEGIN_NAMESPACE
class RefArrayPrivate
{
public:
    Vector<ArrayObject*> array;
};
RefArray::RefArray():
    C_D(RefArray)
{
    pdm->array.setSharable(false);
}

RefArray::~RefArray()
{
    removeAll();
    D_D(RefArray);
}

void RefArray::addObject(ArrayObject *p)
{
    DM_ASSERT(p);

    p->retain();
    pdm->array.append(p);
}

dbool RefArray::removeObject(ArrayObject *p)
{
    DM_ASSERT(p);

    dint index = pdm->array.indexOf(p);
    if (index > -1)
    {
        pdm->array.removeAt(index);
        p->release();
        return true;
    }

    return false;
}

void RefArray::append(ArrayObject *p)
{
    DM_ASSERT(p);

    p->retain();
    pdm->array.append(p);
}

void RefArray::prepend(ArrayObject *p)
{
    DM_ASSERT(p);

    p->retain();
    pdm->array.prepend(p);
}

void RefArray::removeAll()
{
    if (!pdm->array.isEmpty())
    {
        BaseRefPtr *p;
        DM_FOREACH(p, pdm->array)
        {
            p->release();
        }
        pdm->array.clear();
    }
}

dint RefArray::indexOf(ArrayObject *p)  const
{
    return pdm->array.indexOf(p);
}

dint RefArray::lastIndexOf(ArrayObject *p)  const
{
    return pdm->array.lastIndexOf(p);
}

ArrayObject* RefArray::at(dint index) const
{
    return pdm->array.at(index);
}

void RefArray::removeAt(dint index)
{
    ArrayObject *obj = pdm->array.at(index);
    pdm->array.removeAt(index);
    obj->release();
}

dbool RefArray::isEmpty() const
{
    return pdm->array.isEmpty();
}

dint RefArray::count() const
{
    return size();
}

dint RefArray::size() const
{
    return pdm->array.size();
}

ArrayObject* RefArray::takeFirst()
{
    return pdm->array.takeFirst();
}

ArrayObject* RefArray::takeLast()
{
    return pdm->array.takeLast();
}

ArrayObject* RefArray::first() const
{
    return pdm->array.at(0);
}

ArrayObject* RefArray::last() const
{
    return pdm->array.at(pdm->array.count() - 1);
}

void RefArray::removeFirst()
{
    ArrayObject* obj = pdm->array.takeFirst();
    obj->release();
}

void RefArray::removeLast()
{
    ArrayObject* obj = pdm->array.takeLast();
    obj->release();
}

ArrayObject* RefArray::operator[](dint i)
{
    return (pdm->array)[i];
}

ArrayObject* RefArray::operator[](dint i) const
{
    return (pdm->array)[i];
}

RefArray* RefArray::clone() const
{
    RefArray *other = new RefArray;
    ArrayObject* item;
    DM_FOREACH_ARRAY(item, this)
    {
        other->addObject(item);
    }
    return other;
}

DM_END_NAMESPACE


