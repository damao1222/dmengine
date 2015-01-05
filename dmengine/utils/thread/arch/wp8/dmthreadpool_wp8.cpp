/*
   Copyright (C) 2012-2013 Xiongfa Li
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

#include "thread/dmthreadpool.h"
#include <windows.h>

using namespace Windows::Foundation;
using namespace Windows::System::Threading;

DM_BEGIN_NAMESPACE
DM_BEGIN_NS_THREAD
class ThreadPoolPrivate
{
};

ThreadPool::ThreadPool()
{

}

ThreadPool::~ThreadPool()
{

}

dbool ThreadPool::initWithThreadSize(dint size)
{
    return true;
}

dbool ThreadPool::runTask(ITask *task)
{
    DM_ASSERT(task);
    // Create a handle that will be signalled when the thread has completed.
    HANDLE threadHandle = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);

    if (!threadHandle)
        return nullptr;

    // Make a copy of the handle for internal use. This is necessary because
    // the caller is responsible for closing the handle returned by CreateThread,
    // and they may do that before or after the thread has finished running.
    HANDLE completionEvent;
        
    if (!DuplicateHandle(GetCurrentProcess(), threadHandle, GetCurrentProcess(), &completionEvent, 0, false, DUPLICATE_SAME_ACCESS))
    {
        CloseHandle(threadHandle);
        return nullptr;
    }

    auto workItemHandler = ref new WorkItemHandler([=](IAsyncAction^)
    {
        // Run the user callback.
        try
        {
            task->run();
        }
        catch (...) { }

        // Signal that the thread has completed.
        SetEvent(completionEvent);
        CloseHandle(completionEvent);
    }, CallbackContext::Any);

    ThreadPool::RunAsync(workItemHandler, WorkItemPriority::Normal, WorkItemOptions::TimeSliced);
}
DM_END_NS_THREAD
DM_END_NAMESPACE

