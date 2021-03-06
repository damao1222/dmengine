/*
   Copyright (C) 2012-2014 Xiongfa Li
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

#include "dmvarlenarray_test.h"
#include "dmvarlenarray.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE

dbool VarLenArrayTest::init()
{
    return true;
}

void VarLenArrayTest::cleanup()
{

}

void VarLenArrayTest::run()
{
    VarLenArray<char> arr(32);

    arr.append("aaa", 4);
//    arr.append(0);
    DM_LOGI("%s", arr.constData());

    arr.prepend("bbb", 2);
    DM_LOGI("%s", arr.constData());

    arr.remove(3);
    DM_LOGI("%s", arr.constData());

    arr.replace(4, 'c');
    DM_LOGI("%s", arr.constData());

    arr.insert("xxx", 1, 3);
    DM_LOGI("%s", arr.constData());

    arr.remove(2, 2);
    DM_LOGI("%s", arr.constData());
}
DM_END_NAMESPACE

