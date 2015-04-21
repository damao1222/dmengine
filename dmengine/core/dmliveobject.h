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


#ifndef LIVEOBJECT_H
#define LIVEOBJECT_H

#include "dmobject.h"
#include "dmevent.h"

DM_BEGIN_NAMESPACE
class InteractiveObject;
DM_PRIVATE_CLASS(LiveObject);
class DM_DLL_EXPORT LiveObject: public Object
{
    DM_DECLARE_PRIVATE(LiveObject)
public:
    /**
     * Default destructor
     */
    virtual ~LiveObject();

    /** 
     * 判断该LiveObject是否接受输入
     * @return 接受输入，返回true，否则返回false
     */
    dbool isAcceptInput() const;
    
protected:
    friend class InteractiveObject;
    /**
     * Default constructor
     */
    LiveObject();
    LiveObject(LiveObjectPrivate *p);
    
private:
    DM_DISABLE_COPY(LiveObject)
};
DM_END_NAMESPACE
#endif // LIVEOBJECT_H
