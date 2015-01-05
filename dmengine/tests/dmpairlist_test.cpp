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

#include "dmpairlist_test.h"
#include "dmutilstring.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE

dbool PairListTest::init()
{
   return true;
}

void PairListTest::cleanup()
{

}



void PairListTest::run()
{
    VecMap<int, UtilString> list;
    list.insert(1, "test: 1");
    list.insert(2, "test: 2");
    list.insert(3, "test: 3");
    list.insert(4, "test: 4");
    list.insert(3, "tttt: 3");

    DM_LOGI("%d", list.size());
    DM_LOGI("%d", list.count());
    DM_LOGI("%d", list.isEmpty());

    DM_LOGI("%s", list.value(1).toCharStr());
    DM_LOGI("%s", list.value(3).toCharStr());
    DM_LOGI("%s", list.value(2).toCharStr());
    DM_LOGI("%s", list.value(5).toCharStr());
    DM_LOGI("%s", list.value(5, "error").toCharStr());
    DM_LOGI("%s", list.value(4).toCharStr());

    DM_LOGI("%d", list.key("test: 4"));
    DM_LOGI("%d", list.key("test: 2"));
    DM_LOGI("%d", list.key("test: 1"));
    DM_LOGI("%d", list.key("xx", -1));
    DM_LOGI("%d", list.key("xx"));
    DM_LOGI("%d", list.key("test: 3"));

    Vector<int> keys = list.keys();
    Vector<UtilString> values = list.values();
    for (int i=0; i<keys.size(); ++i)
    {
        DM_LOGI("keys: %d", keys.at(i));
    }

    for (int i=0; i<values.size(); ++i)
    {
        DM_LOGI("values: %s", values.at(i).toCharStr());
    }

    DM_LOGI("%s", list[1].toCharStr());
    DM_LOGI("%s", list[7].toCharStr());

    VecMap<int, UtilString> list2;
    list2.insert(1, "test: 1");
    list2.insert(2, "test: 2");
    list2.insert(3, "test: 3");
    list2.insert(4, "test: 4");
    list2.insert(5, "test: 4");
    list2.insertMulti(4, "multi: 4");
    DM_LOGI("%d", list2.size());
    Vector<UtilString> multiValues = list2.values(4);
    for (int i=0; i<multiValues.size(); ++i)
    {
        DM_LOGI("multi values: %s", multiValues.at(i).toCharStr());
    }

    //list2.removeOneByKey(4);
    //list2.removeAllByKey(4);
    list2.removeOneByValue("test: 4");
    //list2.removeAllByValue("test: 4");
    for (int i=0; i<list2.size(); ++i)
    {
        DM_LOGI("multi values: %d %s", list2.at(i).first, list2.at(i).second.toCharStr());
    }

    VecMap<int, UtilString> list3;
    list3 = list;

    VecMap<int, UtilString> list4 = list;

    DM_LOGI("is same : %s", list == list2 ? "true" : "false");
    DM_LOGI("is same : %s", list == list3 ? "true" : "false");
    DM_LOGI("is same : %s", list != list4 ? "true" : "false");

    list.clear();
    DM_LOGI("size after clear %d", list.size());
}
DM_END_NAMESPACE

