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
#include "dmmetatype.h"
#include "dmutilstring.h"
#include "thread/dmreadwritelock.h"
DM_BEGIN_NAMESPACE
#if !defined(DM_HAVE_THREAD)

template <typename T>
class GlobalStatic
{
public:
    T *pointer;
    inline GlobalStatic(T *p) : pointer(p) { }
    inline ~GlobalStatic() { pointer = 0; }
};

#define DM_GLOBAL_STATIC(TYPE, NAME)                                  \
    static TYPE *NAME()                                              \
    {                                                                \
        static TYPE thisVariable;                                    \
        static GlobalStatic<TYPE > thisGlobalStatic(&thisVariable); \
        return thisGlobalStatic.pointer;                             \
    }
#endif

class CustomTypeInfo
{
public:
    CustomTypeInfo() : typeName(), constr(0), destr(0)
    {}

    UtilString typeName;
    MetaType::Constructor constr;
    MetaType::Destructor destr;
    int alias;
};

DM_GLOBAL_STATIC(Vector<CustomTypeInfo>, customTypes)
DM_GLOBAL_STATIC(ReadWriteLock, customTypesLock)


#define DM_ADD_STATIC_METATYPE(STR, TP) \
    { STR, sizeof(STR) - 1, TP }

/* Note: these MUST be in the order of the enums */
static const struct { const char * typeName; int typeNameLength; int type; } types[] = {

    /* All Core types */
    DM_ADD_STATIC_METATYPE("void", MetaType::eInvalid),
    DM_ADD_STATIC_METATYPE("bool", MetaType::eBool),
    DM_ADD_STATIC_METATYPE("int", MetaType::eInt),
    DM_ADD_STATIC_METATYPE("uint", MetaType::eUInt),
    DM_ADD_STATIC_METATYPE("longlong", MetaType::eLongLong),
    DM_ADD_STATIC_METATYPE("ulonglong", MetaType::eULongLong),
    DM_ADD_STATIC_METATYPE("float", MetaType::eFloat),
    DM_ADD_STATIC_METATYPE("double", MetaType::eDouble),
    DM_ADD_STATIC_METATYPE("char", MetaType::eChar),
    DM_ADD_STATIC_METATYPE("pointer", MetaType::eVoidPtr),
    DM_ADD_STATIC_METATYPE("String", MetaType::eString),
    DM_ADD_STATIC_METATYPE("Rect", MetaType::eRect),
    DM_ADD_STATIC_METATYPE("RectF", MetaType::eRectF),
    DM_ADD_STATIC_METATYPE("Size", MetaType::eSize),
    DM_ADD_STATIC_METATYPE("SizeF", MetaType::eSizeF),
    DM_ADD_STATIC_METATYPE("Line", MetaType::eLine),
    DM_ADD_STATIC_METATYPE("LineF", MetaType::eLineF),
    DM_ADD_STATIC_METATYPE("Point", MetaType::ePoint),
    DM_ADD_STATIC_METATYPE("PointF", MetaType::ePointF),
    DM_ADD_STATIC_METATYPE("Circle", MetaType::eCircle),
    DM_ADD_STATIC_METATYPE("CircleF", MetaType::eCircleF),
    DM_ADD_STATIC_METATYPE("real", MetaType::eReal),
    DM_ADD_STATIC_METATYPE("WString", MetaType::eWString),
    {0, 0, MetaType::eInvalid}
};

static inline int MetaTypeStaticType(const char *typeName, int length)
{
    int i = 0;
    while (types[i].typeName && ((length != types[i].typeNameLength)
                                 || strcmp(typeName, types[i].typeName))) {
        ++i;
    }
    return types[i].type;
}

static int MetaTypeCustomType_unlocked(const char *typeName, int length)
{
    const Vector<CustomTypeInfo> * const ct = customTypes();
    if (!ct)
        return 0;

    for (int v = 0; v < ct->count(); ++v) {
        const CustomTypeInfo &customInfo = ct->at(v);
        if ((length == customInfo.typeName.size())
            && !strcmp(typeName, customInfo.typeName.toCharStr())) {
            if (customInfo.alias >= 0)
                return customInfo.alias;
            return v + MetaType::eUser;
        }
    }
    return 0;
}

int MetaType::registerTypedef(const char* typeName, int aliasId)
{
    Vector<CustomTypeInfo> *ct = customTypes();
    if (!ct || !typeName)
        return -1;

    UtilString normalizedTypeName = typeName;

    int idx = MetaTypeStaticType(normalizedTypeName.toCharStr(), 
                                    normalizedTypeName.size());
    if (idx) {
        DM_ASSERT(idx == aliasId);
        return idx;
    }

    WriteLocker locker(customTypesLock());
    idx = MetaTypeCustomType_unlocked(normalizedTypeName.toCharStr(),
                                        normalizedTypeName.size());

    if (idx)
        return idx;

    CustomTypeInfo inf;
    inf.typeName = normalizedTypeName;
    inf.alias = aliasId;
    inf.constr = 0;
    inf.destr = 0;
    ct->append(inf);
    return aliasId;
}

void *MetaType::construct(int type, const void *copy)
{
    if (type == eVoidPtr)
        return new void *(*static_cast<void* const *>(copy));

    Constructor constr = 0;
    
    const Vector<CustomTypeInfo> * const ct = customTypes();
    ReadLocker locker(customTypesLock());
    if (type < eUser || !ct || ct->count() <= type - eUser)
        return 0;
    if (ct->at(type - eUser).typeName.isEmpty())
        return 0;
    constr = ct->at(type - eUser).constr;

    return constr(copy);
}

/*!
    Destroys the \a data, assuming it is of the \a type given.

    \sa construct(), isRegistered(), Type
*/
void MetaType::destroy(int type, void *data)
{
    if (!data)
        return;

    if (type == eVoidPtr)
        delete static_cast<void**>(data);

    const Vector<CustomTypeInfo> * const ct = customTypes();
    Destructor destr = 0;
    ReadLocker locker(customTypesLock());
    if (type < eUser || !ct || ct->count() <= type - eUser)
        return;
    if (ct->at(type - eUser).typeName.isEmpty())
        return;
    destr = ct->at(type - eUser).destr;
    destr(data);
}

int MetaType::registerType(const char *typeName, Destructor destructor,
                            Constructor constructor)
{
    Vector<CustomTypeInfo> *ct = customTypes();
    if (!ct || !typeName || !destructor || !constructor)
        return -1;

    UtilString normalizedTypeName = typeName;

    int idx = MetaTypeStaticType(normalizedTypeName.toCharStr(),
                                  normalizedTypeName.size());

    if (!idx) {
        WriteLocker locker(customTypesLock());
        idx = MetaTypeCustomType_unlocked(normalizedTypeName.toCharStr(),
                                           normalizedTypeName.size());
        if (!idx) {
            CustomTypeInfo inf;
            inf.typeName = normalizedTypeName;
            inf.constr = constructor;
            inf.destr = destructor;
            inf.alias = -1;
            idx = ct->size() + eUser;
            ct->append(inf);
        }
    }
    return idx;
}
DM_END_NAMESPACE