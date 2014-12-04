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
#ifndef DMMACRO_H
#define DMMACRO_H

#define DM_MEMBER_OFFSET(Class, Member) ((int)&((Class*)0)->Member)

#if 0
#include "dmmetadata.h"

#define DM_BIND_PROPERTY_BEGIN \
            void registProperties() {

#define DM_BIND_PROPERTY(Type, Name, Member) \
                DM::Object::registMetaData(Name, new DM::MetaDataTemp<Type>(Member))

#define DM_BIND_PROPERTY_END \
            }



#define DM_REGIST_PROPERTY(Class, Type, Name, Set, Get) \
                class MetaData##Name : public MetaData {\
                public : \
                virtual dbool set(const Variant &v) { obj->Set(v.value<Type>()); return true; } \
                virtual dbool get(Variant &dest) const { dest = Type(obj->Get()); return true; } \
                virtual dint id() const { dmHash(Name); } \
                Class *obj; MetaData##Name(Class *owner) : obj(owner) {}\
                }

#define DM_REGIST_PROPERTY(Type, Name, Set, Get) \
                DM::Object::registMetaData(Name, new MetaData##Name(this))
#endif

enum {
    eMetaCmd_Set = 1,
    eMetaCmd_Get = 2,
};

#if 0
#define DM_REGIST_PROPERTY_BEGIN \
                    dbool processMetadata(dint id, dint cmd, void *v) { \
                    switch (id) \
                    {
#define DM_REGIST_PROPERTY(Type, ID, Set, Get) \
                    case ID: { \
                        switch (cmd) { \
                        case eMetaCmd_Set: {\
                        Set((*static_cast<Variant*>(v)).value<Type>()); return true; }\
                        case eMetaCmd_Get: {\
                        (*static_cast<Variant*>(v)) = Get(); return true; }\
                        default break; }  \
                        } \
                    break; }

#define DM_REGIST_PROPERTY_SET(Type, ID, Set) \
                    case ID: { \
                        switch (cmd) { \
                        case eMetaCmd_Set: {\
                        Set((*static_cast<Variant*>(v)).value<Type>()); return true; }\
                        case eMetaCmd_Get: {\
                        return false; }\
                        default return false; }  \
                        } \
                    break; }

#define DM_REGIST_PROPERTY_Get(Type, ID, Set, Get) \
                    case ID: { \
                        switch (cmd) { \
                        case eMetaCmd_Set: {\
                        return false; }\
                        case eMetaCmd_Get: {\
                        (*static_cast<Variant*>(v)) = Get(); return true; }\
                        default return false }  \
                        } \
                    break; }
#define DM_REGIST_PROPERTY_END \
                    default : break;} return false;\
                }
#else
#define DM_REGIST_PROPERTY_BEGIN \
                    protected: dbool processMetadata(const UtilString &key, dint cmd, void *v) { 
                    
#define DM_REGIST_PROPERTY(Type, Name, Set, Get) \
                    if (key == Name) { \
                        switch (cmd) { \
                        case eMetaCmd_Set: {\
                        Set((*static_cast<Variant*>(v)).value<Type>()); return true; }\
                        case eMetaCmd_Get: {\
                        (*static_cast<Variant*>(v)).setValue<Type>(Get()); return true; }\
                        default: return false; \
                        } \
                    } else 

#define DM_REGIST_PROPERTY_SET(Type, Name, Set) \
                    if (key == Name) { \
                        switch (cmd) { \
                        case eMetaCmd_Set: {\
                        Set((*static_cast<Variant*>(v)).value<Type>()); return true; }\
                        case eMetaCmd_Get: {\
                        return false; }\
                        default: return false;\
                        } \
                    } else 

#define DM_REGIST_PROPERTY_Get(Type, Name, Get) \
                    if (key == Name) \
                        switch (cmd) { \
                        case eMetaCmd_Set: {\
                        return false; }\
                        case eMetaCmd_Get: {\
                        (*static_cast<Variant*>(v)).setValue<Type>(Get()); return true; }\
                        default: return false;\
                        } \
                    } else 
#define DM_REGIST_PROPERTY_END \
                    return false;\
                }

#define DM_REGIST_PROPERTY_SUPER_END(SuperClass) \
                    return SuperClass::processMetadata(key, cmd, v);\
                }
#endif
#endif // DMMACRO_H
