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
#ifndef DMDEBUG_H
#define DMDEBUG_H

#include "dmlogger.h"

#ifndef DM_DEBUG
#define DMDEBUG(a, b...)
#else
//#include <stdio.h>
//#define DMDEBUG(a, b...) printf("%s <%s:%d> "a, __FUNCTION__, __FILE__, __LINE__, ##b)
//#define DMDEBUG(a, b...) DM_LOG(LOGDEBUG, a, ##b)
#define DMDEBUG(FORMAT, ...) DM_LOG(DM_NS::LOGDEBUG, FORMAT, ##__VA_ARGS__)
#endif


#endif // DMDEBUG_H
