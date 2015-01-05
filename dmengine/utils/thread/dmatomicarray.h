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

#ifndef DMATOMICARRAY_H
#define DMATOMICARRAY_H

#include "dmrefarray.h"

DM_BEGIN_NAMESPACE
DM_BEGIN_NS_THREAD
DM_PRIVATE_CLASS(RefArray);
class DM_DLL_EXPORT RefArray: public RefPtr<Threads::RefArray>
{
    DM_DECLARE_PRIVATE(RefArray)
public:
    RefArray();
    ~RefArray();

    void addObject(ArrayObject *p);
    dbool removeObject(ArrayObject *p);

    void removeAll();

    void append(ArrayObject *p);
    void prepend(ArrayObject *p);

    dint indexOf(ArrayObject *p);
    ArrayObject* at(dint index);
    void removeAt(dint index);

    dbool isEmpty() const;
    dint count() const;
    dint size() const;

    ArrayObject* first() const;
    ArrayObject* last() const;
    
    void removeFirst();
    void removeLast();

    ArrayObject* takeFirst();
    ArrayObject* takeLast();

    ArrayObject* operator[](dint i);

	RefArray* clone() const;
};

DM_END_NS_THREAD
DM_END_NAMESPACE
#endif // DMATOMICARRAY_H

