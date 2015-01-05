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
#ifndef DMVECMAP_H
#define DMVECMAP_H
#include "dmpairlist.h"
DM_BEGIN_NAMESPACE
template <typename T1, typename T2>
/** 
 * @brief VecMap class
 * see PairList
 */
class VecMap : public PairList<T1, T2>
{ };
DM_END_NAMESPACE
#endif // DMVECMAP_H
