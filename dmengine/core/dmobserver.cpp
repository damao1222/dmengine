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

#include "dmobserver.h"
#include "dmvector.h"
#include "dmlogger.h"
DM_BEGIN_NAMESPACE
class SubjectPrivate
{
public:
    Vector<Observer*> observers;
};

Subject::Subject():
    C_D(Subject)
{

}

Subject::~Subject()
{
    pdm->observers.clear();
    D_D(Subject);
}

void Subject::update()
{
    for (dint i=0; i<pdm->observers.size(); ++i)
    {
        Observer* o = pdm->observers.at(i);
        if (o->isObserved())
        {
            o->update(this);
        }
    }
}

dbool Subject::attach(Observer *o)
{
    dint i = pdm->observers.indexOf(o);
    if (i == -1)
    {
        pdm->observers.append(o);
        o->onAttached(this);
        return true;
    }
    DM_LOGW("observer have been attached");
    return false;
}

dbool Subject::detach(Observer *o)
{
    dint i = pdm->observers.indexOf(o);
    if (i != -1)
    {
        pdm->observers.removeAt(i);
        o->onDetached(this);
        return true;
    }
    return false;
}

void Subject::cleanAll()
{
    for (dint i=0; i<pdm->observers.size(); ++i)
    {
        pdm->observers.at(i)->onDetached(this);
    }
    pdm->observers.clear();
}

dint Subject::observerSize() const
{
    return pdm->observers.size();
}

DM_END_NAMESPACE
