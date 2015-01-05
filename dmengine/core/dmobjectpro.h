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
#ifndef DMOBJECTPRO_H
#define DMOBJECTPRO_H
#include "dmhash.h"
#include "dmcallback.h"

#ifdef DM_SUPPORT_OBJECT_PRO
#define DM_COMMON
#ifndef DM_COMMON 
#include "dmobject.h"
DM_BEGIN_NAMESPACE
typedef struct _ObjectCBData{
    _ObjectCBData() : func(0), classPtr(0) {}
    dbool isEmpty() const { return (func==0 && classPtr==0); }

    CallFunc func;
    void *classPtr;
} ObjectCBData;
DM_END_NAMESPACE

#define DM_OBJECT \
        protected:  Hash<duint, ObjectCBData> m_object_cb_map;

#define DM_SIGNAL(FuncName) \
        private:    duint FuncName##FuncKey() const { return dmHash(#FuncName); } \
        public:     void FuncName(const Variant &v) { \
                        ObjectCBData d = m_object_cb_map.value(FuncName##FuncKey()); \
                        if (!d.isEmpty()) {\
                            (static_cast<Object*>(d.classPtr)->*(d.func))(v); \
                        }\
                    }


#define DM_CONNECT(O1, FuncName1, O2Type, O2, FuncName2)    \
        do { \
            DM_NS::ObjectCBData d; \
            d.func = (DM_NS::CallFunc)(&O2Type##::##FuncName2);\
            d.classPtr = O2; \
            O1->m_object_cb_map.insert(O1->FuncName1##FuncKey(), d);\ 
        } while (0)

#else
DM_BEGIN_NAMESPACE
class ObjectCBData{
public:
    ObjectCBData() : func(0), classPtr(0) {}
    dbool isEmpty() const { return (func==0 && classPtr==0); }
    dbool operator==(const ObjectCBData &other) const
    { return (func == other.func && classPtr == other.classPtr); }
    dbool operator!=(const ObjectCBData &other) const
    { return !(*this == other); }

    CallbackFunc func;
    void *classPtr;
};
DM_END_NAMESPACE
DM_USING_NAMESPACE;
#define DM_OBJECT \
        public: Hash<duint, ObjectCBData> m_object_cb_map; \
        public: dbool __isSignalConnected(dint key, const ObjectCBData &data) const {\
                    Vector<ObjectCBData> l = m_object_cb_map.values(key); \
                    if (!l.isEmpty()) {\
                        for (int i=0; i<l.size(); ++i) {\
                            if (l.at(i) == data) \
                                return true; \
                        } \
                    }\
                    return false; \
                } \
        public: dbool __removeSlot(dint key, const ObjectCBData &data) { \
                    Hash<duint, ObjectCBData>::Iterator it = m_object_cb_map.begin(); \
                    while (it != m_object_cb_map.constEnd()) {\
                        if (key == it.key() && data == it.value()) {\
                            it = m_object_cb_map.erase(it);  return true; \
                        } \
                        else\
                            ++it;\
                    }\
                    return false; \
                }
                    

#define DM_SIGNAL(FuncName) \
        public: duint FuncName##FuncKey() const { return dmHash(#FuncName); } \
        public: void FuncName(const Variant &v = Variant()) { \
                    Vector<ObjectCBData> l = m_object_cb_map.values(FuncName##FuncKey()); \
                    if (!l.isEmpty()) {\
                        for (int i=0; i<l.size(); ++i) {\
                            const ObjectCBData& d = l.at(i); \
                            (*(d.func))(d.classPtr, v); \
                        } \
                    }\
                }

#define DM_SLOT_DECLARE(FuncName) \
            public: void FuncName(const Variant &param);

#define DM_SIGNAL_INHERITABLE(ClassName, FuncName) \
    public: duint FuncName##FuncKey() const { return dmHash(#ClassName###FuncName); } \
    public: void FuncName(const Variant &v) { \
                Vector<ObjectCBData> l = m_object_cb_map.values(FuncName##FuncKey()); \
                if (!l.isEmpty()) {\
                    for (int i=0; i<l.size(); ++i) {\
                        const ObjectCBData& d = l.at(i); \
                        (*(d.func))(d.classPtr, v); \
                    } \
                }\
            }


#define DM_CONNECT(O1Type, O1, FuncName1, O2Type, O2, FuncName2)    \
        do { \
            ObjectCBData d; \
            d.func = DM_CALLBACK(O2Type, FuncName2);\
            d.classPtr = (O2); \
            O1Type *OT1 = static_cast<O1Type*>(O1); \
            if (!OT1->__isSignalConnected(OT1->FuncName1##FuncKey(), d)) \
                OT1->m_object_cb_map.insertMulti(OT1->FuncName1##FuncKey(), d); \
        } while (0)

#define DM_DISCONNECT(O1Type, O1, FuncName1, O2Type, O2, FuncName2)    \
        do { \
            ObjectCBData d; \
            d.func = DM_CALLBACK(O2Type, FuncName2);\
            d.classPtr = (O2); \
            O1Type *OT1 = static_cast<O1Type*>(O1); \
            OT1->__removeSlot(OT1->FuncName1##FuncKey(), d); \
        } while (0)
#endif

#else //DM_SUPPORT_OBJECT_PRO
#define DM_OBJECT
#define DM_SIGNAL(FuncName)
#define DM_SIGNAL_INHERITABLE(ClassName, FuncName)
#define DM_SLOT_DECLARE(FuncName)
#define DM_CONNECT(O1Type, O1, FuncName1, O2Type, O2, FuncName2)
#define DM_DISCONNECT(O1Type, O1, FuncName1, O2Type, O2, FuncName2)
#endif // DM_SUPPORT_OBJECT_PRO
#endif // DMOBJECTPRO_H
