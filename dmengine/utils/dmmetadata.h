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
#ifndef DMMETADATA_H
#define DMMETADATA_H

#include "dmvariant.h"

DM_BEGIN_NAMESPACE
class MetaData
{
public:
    virtual dbool set(const Variant &v) = 0;
    virtual dbool get(Variant &dest) const = 0;
    virtual dint type() const { return -1; }
    virtual dint id() const { return -1; }
};

template <typename T>
class MetaDataTemp : public MetaData
{
public:
    MetaDataTemp(T &prop) : d(prop), t(dmMetaTypeId<T>()) {}
    dbool set(const Variant &v) { d = v.value<T>(); return true; }
    dbool get(Variant &dest) const { dest = Variant(d); return true; }

    dint type() const { return t; }

private:
    T &d;
    dint t;
};
DM_END_NAMESPACE

#endif // DMMETADATA_H
