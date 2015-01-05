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
#include "dmobject.h"
#include "dmcoreapplication.h"
#include "dmevent.h"
#include "dmutilstring.h"
#include "dmvariant.h"
#include "dmbuiltin.h"

DM_BEGIN_NAMESPACE
#if 0
void *p = pFun(&A::foo);
union   
{    
    void (A::*pfoo)(void);    
    void* p;    
} u;    
   
u.pfoo = &A::foo;    
void* func = u.p;
#endif

class PropertyInfo{
public:
    PropertyInfo() : type(-1), ptr(0) {}
    PropertyInfo(int t, void *p) : type(t), ptr(p) {}
    dbool isEmpty() const { return ptr == 0; }
    int type;
    void *ptr;
};

class ObjectPrivate
{
public:
    struct ExtraData
    {
        ExtraData() {}
        Hash<UtilString, Variant> properties;
    };
    ObjectPrivate();
    ~ObjectPrivate();
    ExtraData *extraData;
    dint tag;
    //Hash<UtilString, MetaData*> propertyDatas;
    //Hash<UtilString, PropertyInfo*> propertyDatas;
    //void registProperty(const UtilString &name, dint type, void *v);
};

ObjectPrivate::ObjectPrivate() :
    extraData(NULL),
    tag(0)
{

}

ObjectPrivate::~ObjectPrivate()
{
    DM_SAFE_DELETE(extraData);
    /*
    MetaData *item;
    DM_FOREACH_HASH(item, propertyDatas, UtilString, MetaData*)
    {
        delete item;
    }
    propertyDatas.clear();
    */
}

/*
void ObjectPrivate::registProperty(const UtilString &name, dint type, void *v)
{
    DM_ASSERT_X(type > -1, "Unknown Type");
    propertyDatas.insert(name, new PropertyInfo(type, v));
}
*/

Object::Object():
    C_D(Object)
{
}

Object::~Object()
{
    D_D(Object);
}

dint Object::tag() const
{
    return pdm->tag;
}

void Object::setTag(dint tag)
{
    pdm->tag = tag;
}

void Object::sendEvent(Object *obj, Event *e)
{
    e->sender = this;
    CoreApplication::sendEvent(obj, e);
}

dbool Object::setProperty(const UtilString &name, const Variant &value)
{
    Variant v = value;
    if (Object::metacall(this, name, eMetaCmd_Set, &v))
        return true;
    else
    {
        if (pdm->extraData == NULL)
        {
            pdm->extraData = new ObjectPrivate::ExtraData;
        }

        pdm->extraData->properties.insert(name, value);
        return true;
    }

    return false;
}

Variant Object::property(const UtilString &name) const
{
    Variant v;
    if (Object::metacall(const_cast<Object*>(this), name, eMetaCmd_Get, &v))
        return v;
    else
    {
        if (pdm->extraData != NULL)
        {
            return pdm->extraData->properties.value(name);
        }
    }

    return Variant();
}

dbool Object::event(Event *e)
{
    DM_UNUSED(e);
    return false;
}

dbool Object::metacall(Object *o, const UtilString &key, dint cmd, void *v)
{
    return o->processMetadata(key, cmd, v);
}

dbool Object::processMetadata(const UtilString &key, dint cmd, void *v)
{
    return false;
}

DM_END_NAMESPACE
