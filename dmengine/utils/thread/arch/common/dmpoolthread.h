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

#ifndef DMPOOLTHREAD_H
#define DMPOOLTHREAD_H
#include "dmrefptr.h"
#include "thread/dmthread.h"
#include "thread/dmtask.h"

DM_BEGIN_NAMESPACE
DM_BEGIN_NS_THREAD
DM_PRIVATE_CLASS(PoolThread);
class DM_DLL_EXPORT PoolThread : public Thread, public RefPtr<PoolThread>
{
    DM_DECLARE_PRIVATE(PoolThread);
public:
    PoolThread();
    ~PoolThread();

    void stop();
    dbool setTask(ITask *task);
    dbool isRunning() const;
    dbool isIdle() const;

protected:
    void run();
};
DM_END_NS_THREAD
DM_END_NAMESPACE
#endif // DMPOOLTHREAD_H
