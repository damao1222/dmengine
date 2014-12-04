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

#ifndef COCOSOBSERVER_H
#define COCOSOBSERVER_H
#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
class Observer;
DM_PRIVATE_CLASS(Subject);
class DM_DLL_EXPORT Subject
{
    DM_DECLARE_PRIVATE(Subject)
public:
    Subject();
    virtual ~Subject();

    virtual dbool attach(Observer *o);
    virtual dbool detach(Observer *o);

    void cleanAll();
    dint observerSize() const;

    virtual void update();
};

class DM_DLL_EXPORT Observer
{
    friend class Subject;
public:
    Observer() : m_observed(true) {}
    virtual ~Observer() {}

    void setObserve(dbool flag) { m_observed = flag; }
    dbool isObserved() const { return m_observed; }

protected:
    virtual void update(Subject *s) { DM_UNUSED(s); }
    virtual void onAttached(Subject *s) { DM_UNUSED(s); }
    virtual void onDetached(Subject *s) { DM_UNUSED(s); }

protected:
    dbool m_observed;
};

DM_END_NAMESPACE
#endif // COCOSOBSERVER_H
