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

#include "dmatomicarray.h"
#include "dmvector.h"
#include "thread/dmsinglelock.h"

DM_BEGIN_NAMESPACE
DM_BEGIN_NS_THREAD
class RefArrayPrivate
{
public:
    DM_NS::RefArray *array;
    DM_NS::Mutex mutex;
};

RefArray::RefArray():
    C_D(RefArray)
{
    SingleLock lock(&pdm->mutex);
    pdm->array = new DM_NS::RefArray;
}

RefArray::~RefArray()
{
    {
        SingleLock lock(&pdm->mutex);
        DM_SAFE_RELEASE(pdm->array);
    }
    D_D(RefArray);
}

void RefArray::addObject(ArrayObject *p)
{
    SingleLock lock(&pdm->mutex);
    pdm->array->addObject(p);
}

dbool RefArray::removeObject(ArrayObject *p)
{
    SingleLock lock(&pdm->mutex);
    return pdm->array->removeObject(p);
}

void RefArray::append(ArrayObject *p)
{
    SingleLock lock(&pdm->mutex);
    pdm->array->append(p);
}

void RefArray::prepend(ArrayObject *p)
{
    SingleLock lock(&pdm->mutex);
    pdm->array->prepend(p);
}

void RefArray::removeAll()
{
    SingleLock lock(&pdm->mutex);
    pdm->array->removeAll();
}

dint RefArray::indexOf(ArrayObject *p)
{
    SingleLock lock(&pdm->mutex);
    return pdm->array->indexOf(p);
}

ArrayObject* RefArray::at(dint index)
{
    SingleLock lock(&pdm->mutex);
    return pdm->array->at(index);
}

void RefArray::removeAt(dint index)
{
    SingleLock lock(&pdm->mutex);
    pdm->array->removeAt(index);
}

dbool RefArray::isEmpty() const
{
    SingleLock lock(&pdm->mutex);
    return pdm->array->isEmpty();
}

dint RefArray::count() const
{
    SingleLock lock(&pdm->mutex);
    return pdm->array->count();
}

dint RefArray::size() const
{
    SingleLock lock(&pdm->mutex);
    return pdm->array->size();
}

ArrayObject* RefArray::takeFirst()
{
    SingleLock lock(&pdm->mutex);
    return pdm->array->takeFirst();
}

ArrayObject* RefArray::takeLast()
{
    SingleLock lock(&pdm->mutex);
    return pdm->array->takeLast();
}

ArrayObject* RefArray::first() const
{
    SingleLock lock(&pdm->mutex);
    return pdm->array->first();
}

ArrayObject* RefArray::last() const
{
    SingleLock lock(&pdm->mutex);
    return pdm->array->last();
}

void RefArray::removeFirst()
{
    SingleLock lock(&pdm->mutex);
    pdm->array->removeFirst();
}

void RefArray::removeLast()
{
    SingleLock lock(&pdm->mutex);
    pdm->array->removeLast();
}

ArrayObject* RefArray::operator[](dint i)
{
    SingleLock lock(&pdm->mutex);
    return (*pdm->array)[i];
}

RefArray* RefArray::clone() const
{
	SingleLock lock(&pdm->mutex);

	RefArray *other = new RefArray;
	ArrayObject* item;

	DM_FOREACH_ARRAY(item, pdm->array)
	{
		other->pdm->array->addObject(item);
	}

	return other;
}

DM_END_NS_THREAD
DM_END_NAMESPACE


