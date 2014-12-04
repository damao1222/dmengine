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

#ifndef DMTEST_H
#define DMTEST_H

#include "dmnamespace.h"

DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT Test
{
public:
    virtual dbool init() = 0;
    virtual void cleanup() = 0;

    virtual void run() = 0;
};
DM_END_NAMESPACE
#endif // DMTEST_H
