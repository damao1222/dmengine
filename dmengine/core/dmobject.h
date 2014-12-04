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
#ifndef DMOBJECT_H
#define DMOBJECT_H
#include "dmnamespace.h"
#include "dmrefptr.h"
#include "dmmacro.h"

DM_BEGIN_NAMESPACE  

class Event;
class UtilString;
class Variant;
class MetaData;
DM_PRIVATE_CLASS(Object);

class DM_DLL_EXPORT Object: public RefPtr<Object>
{
    DM_DECLARE_PRIVATE(Object)
public:
    /**
     * Default constructor
     */
    Object();
    /**
     * Default destructor
     */
    virtual ~Object();

    /** 
     * Get the tag of this object
     * @return Tag of Object
     */
    virtual dint tag() const;
    /** 
     * Set the tag of this object
     * @param tag  Tag of Object
     */
    void setTag(dint tag);

    /** 
     * @brief Send a event to the specified object
     * @param obj  The object which one will get the event
     * @param e  The event which one will send to the object
     */
    void sendEvent(Object *obj, Event *e);

    /** 
     * @brief Set the property with a new value
     * @param name  The name of the property
     * @param value  The new value
     * @return Return true if success
     */
    dbool setProperty(const UtilString &name, const Variant &value);

    /** 
     * @brief Get the property value
     * @param name  The name of the property
     * @return Return a valid value if object have the property
     */
    Variant property(const UtilString &) const;

    //internal
    virtual dbool event(Event *e);
protected:
    static dbool metacall(Object *o, const UtilString &key, dint cmd, void *v);
    virtual dbool processMetadata(const UtilString &key, dint cmd, void *v);
private:
    DM_DISABLE_COPY(Object)
};

typedef void (Object::*CallFunc)(const Variant &param);

DM_END_NAMESPACE

#define DM_CALLFUNC(Func) (DM::CallFunc)(&Func)
//#include "dmobjectpro.h"
#endif //DMOBJECT_H
