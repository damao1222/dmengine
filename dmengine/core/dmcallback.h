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

#ifndef DMCALLBACK_H
#define DMCALLBACK_H

#include "dmvariant.h"
DM_BEGIN_NAMESPACE

typedef void (*CallbackFunc)(void *obj, const Variant &param);

template<class T, void(T::*fn)(const Variant &param)>
void Callback(void *obj, const Variant &param)
{
    (static_cast<T*>(obj)->*fn)(param);
}

typedef struct _CallbackData{
    _CallbackData() : func(0), classPtr(0) {}
    dbool isEmpty() const { return (func==0 && classPtr==0); }

    CallbackFunc func;
    void *classPtr;
} CallbackData;

DM_END_NAMESPACE

#define DM_CALLBACK(CLASS, MEMBER_FUNC_NAME) \
            DM::Callback<CLASS, &CLASS::MEMBER_FUNC_NAME>

#define DM_TEMPLATE_CALLBACK(TEMPLATE, CLASS, MEMBER_FUNC_NAME) \
            TEMPLATE<CLASS, &CLASS::MEMBER_FUNC_NAME>
#endif // DMCALLBACK_H
