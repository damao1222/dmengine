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

#include "dmshareddata_test.h"
#include "dmshareddata.h"
#include "dmlogger.h"
#include "dmatomicint.h"

DM_BEGIN_NAMESPACE

class TestData
{
public:
    AtomicInt ref;
};

dbool SharedDataTest::init()
{
   return true;
}

void SharedDataTest::cleanup()
{

}

void SharedDataTest::run()
{
    TestData *data = new TestData;
	SharedData<TestData> d1(data);
    DM_LOGI("d1 is %d", d1->ref);
    SharedData<TestData> d2 = d1;
    DM_LOGI("d2 is %d", d2->ref);
    DM_LOGI("d1 d2 is same %d", d1 == d2);
    SharedData<TestData> d3;
    d3 = d1;
    DM_LOGI("d3 is %d", d3->ref);
    DM_LOGI("d1 d3 is same %d", d1 == d3);
    DM_LOGI("d2 d3 is same %d", d2 == d3);

}
DM_END_NAMESPACE

