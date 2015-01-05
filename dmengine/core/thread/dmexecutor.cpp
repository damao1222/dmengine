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

#include "dmexecutor.h"
#include "dmlogger.h"
#include "dmvector.h"
#include "dmevent.h"
#include "thread/dmthread.h"
#include "thread/dmsinglelock.h"
#include "thread/dmcondition.h"
#include "dmexectask.h"
#include "dmcoreapplication.h"
#include "dmobject.h"
#include "dmendtime.h"
#include "dmdllsingleton.h"

DM_BEGIN_NAMESPACE
class ExecuteEvent : public Event
{
public:
    ExecuteEvent(ExecTask *task) : _task(task), param(NULL), timeout(0) {}

    EventType type() const { return eExecutorEvent; }

    ExecTask *_task;
    void *param;
    duint timeout;
};

class ExecObject : public Object
{
public:
    virtual dbool event(Event *e);
};

dbool ExecObject::event(Event *e)
{
    switch (e->type())
    {
    case eExecutorEvent:
        {
            ExecuteEvent *exec = static_cast<ExecuteEvent*>(e);
            exec->_task->onTaskDone(exec->param, true);
            return true;
        }
    default:
        return Object::event(e);
    }
}

class TaskItem
{
public:
    TaskItem() : 
          task(NULL),
          prio(0),
          timeout(0)
      {
      }

      TaskItem(ExecTask *pTask, dint priority, duint timeoutInMilli = 0) : 
          task(pTask),
          prio(priority),
          timeout(timeoutInMilli)
      {
      }

      TaskItem(const TaskItem &item) : 
          task(item.task),
          prio(item.prio),
          timeout(item.timeout)
      {
      }

      ~TaskItem()
      {
      }

      TaskItem& operator=(const TaskItem& item)
      {
          task = item.task;
          prio = item.prio;
          timeout = item.timeout;
          return *this;
      }

      ExecTask *task;
      dint prio;
      duint timeout;
};

class ExecThread : public Thread
{
public:
    ExecThread() : mCurrentTask(NULL), mCurrentTimeout(0), mStop(false), mTimeout(0) {}

    void setWaitTaskTime(duint iTimeoutInMilliSeconds);
    void addTask(ExecTask *task, dint priority = 0, duint timeout = 0);
    void checkTaskTimeout();
    dbool isBusy() ;
    duint runTime() const;
    void abort();
    dint taskCount() ;
    duint totalTime();

protected:
    void setTask(ExecTask *task, duint timeout);
    virtual void run();

private:
    Vector<TaskItem> mTaskList;
    ExecTask *volatile mCurrentTask;
    volatile duint mCurrentTimeout;
    volatile bool mStop;
    Mutex mMutex;
    WaitCondition mWait;
    EndTime mTimer;
    duint mTimeout;
};

void ExecThread::setWaitTaskTime(duint iTimeoutInMilliSeconds)
{
    SingleLock lock(&mMutex);
    mTimeout = iTimeoutInMilliSeconds;
}

void ExecThread::addTask(ExecTask *task, dint priority /*= 0*/, duint timeout /*= 0*/)
{
    SingleLock lock(&mMutex);

    if (mStop)
        return ;

    Vector<TaskItem>::iterator it = mTaskList.begin();
    while(it != mTaskList.constEnd())
    {
        if(priority <= it->prio)
            break;
        it++;
    }

    mTaskList.insert(it, TaskItem(task, priority, timeout));
    mWait.set();
}

void ExecThread::setTask(ExecTask *task, duint timeout) 
{ 
    if (task) 
        mTimer.resetTime(timeout);

    mCurrentTask = task; 
    mCurrentTimeout = timeout;
}

void ExecThread::checkTaskTimeout()
{
    SingleLock lock(&mMutex);
    if (mCurrentTask && mCurrentTimeout && mCurrentTimeout != UINT_MAX)
    {
        if (mTimer.isExpired())
        {
            mCurrentTask->abort(ExecTask::eTimeout);
        }
    }
}

dbool ExecThread::isBusy() 
{ 
    SingleLock lock(&mMutex);
    return mCurrentTask || mTaskList.size() > 0; 
}

duint ExecThread::runTime() const
{
    return mTimer.pastTime();
}

void ExecThread::abort()
{
    SingleLock lock(&mMutex);
    mStop = true;
    if (mCurrentTask != NULL)
        mCurrentTask->abort(ExecTask::eQuit);
    mTaskList.clear();
    mWait.set();
}

dint ExecThread::taskCount()
{
    SingleLock lock(&mMutex);
    return mTaskList.size() + (mCurrentTask ? 1 : 0);
}

duint ExecThread::totalTime()
{
    SingleLock lock(&mMutex);
    duint total = 0;
    for (dint i=0; i<mTaskList.size(); ++i)
    {
        const TaskItem& item = mTaskList.at(i);
        if (item.timeout == UINT_MAX)
            return UINT_MAX;

        total += item.timeout;
    }
    total += (mCurrentTask ? mCurrentTimeout : 0);
    return total;
}

void ExecThread::run()
{
    while (!mStop)
    {
        SingleLock lock(&mMutex);

        while (!mStop)
        {
            if (!mTaskList.isEmpty())
            {
                const TaskItem & item = mTaskList.takeFirst();
                ExecTask *task = item.task;
                setTask(task, item.timeout);
                lock.unlock();

                const ExecResult& ret = task->run();
                if (ret.type & ExecResult::eCurrentThread)
                {
                    task->onTaskDone(ret.param, false);
                }

                if (ret.type & ExecResult::eMainThread)
                {
                    ExecuteEvent *execEvent = new ExecuteEvent(task);
                    execEvent->param = ret.param;
                    ExecObject *obj = new ExecObject;
                    //返回主线程调用
                    CoreApplication::postEvent(obj, execEvent);
                    execEvent->release();
                    //删除调用object
                    DeferredDeleteEvent *deleteEvent = new DeferredDeleteEvent;
                    CoreApplication::postEvent(obj, deleteEvent);
                    deleteEvent->release();
                }
                lock.lock();
                setTask(NULL, 0);
                lock.unlock();
                break;
            }
            else if (!mTimeout)
            {
                Thread::yieldCurrentThread();
                break;
            }
            else
            {
                mWait.reset();
                lock.unlock();
                if (!mWait.waitMSec(mTimeout))
                    break;

                lock.lock();
            }
        }
    }    
}

class DM_DLL_EXPORT ExecutorPrivate
{
public:
    Vector<ExecThread*> threadPool;
    Mutex mutex;
    DM_DECLARE_SINGLETON(Executor);
};

Executor::Executor()
	:C_D(Executor)
{

}

Executor::~Executor()
{
    finishAndWait();
    D_D(Executor);
}

dbool Executor::init(duint nNum)
{
    SingleLock lock(&pdm->mutex);
    if (!pdm->threadPool.isEmpty())
        return false;

    for (duint i=0; i<nNum; ++i)
    {
        ExecThread *thread = new ExecThread;
        thread->setWaitTaskTime(10);
        pdm->threadPool.append(thread);
        thread->start();
    }

    return true;
}

dbool Executor::increase(duint nNum)
{
    SingleLock lock(&pdm->mutex);
    for (duint i=0; i<nNum; ++i)
    {
        ExecThread *thread = new ExecThread;
        thread->setWaitTaskTime(10);
        pdm->threadPool.append(thread);
        thread->start();
    }

    return true;
}

dint Executor::threadCount() const
{
    SingleLock lock(&pdm->mutex);
    return pdm->threadPool.size();
}

void Executor::finishAndWait()
{
    SingleLock lock(&pdm->mutex);
    for (dint i=0; i<pdm->threadPool.size(); ++i)
    {
        ExecThread *thread = pdm->threadPool.at(i);
        thread->abort();
        thread->wait();
        delete thread;
    }
    pdm->threadPool.clear();
}

dbool Executor::run(ExecTask *task, duint timeoutInMilliSec /*= 0*/)
{
    SingleLock lock(&pdm->mutex);
    if (!task) return false;

    if (pdm->threadPool.isEmpty())
        return false;

    //找空闲的线程
    for (dint i=0; i<pdm->threadPool.size(); ++i)
    {
        ExecThread *thread = pdm->threadPool.at(i);
        if (!thread->isBusy())
        {
            thread->addTask(task, 0, timeoutInMilliSec);
            return true;
        }
    }

    return false;
}

Executor::EAddTaskResult Executor::addTask(ExecTask *task, dint priority /*= 0*/, duint timeoutInMilliSec /*= 0*/)
{
    //优先找空闲的线程，在run中将检测当前任务是否超时
    if (run(task, timeoutInMilliSec))
        return eRun;

    SingleLock lock(&pdm->mutex);
    if (!task) return eTaskIsNull;

    if (pdm->threadPool.isEmpty())
        return eThreadPoolEmpty;

    if (priority < 0)
        priority = 0;
    /*
    //优先找等待任务最少的线程
    dint quickIndex = 0, waitNum = INT_MAX;
    for (dint i=0; i<pdm->threadPool.size(); ++i)
    {
        ExecThread *thread = pdm->threadPool.at(i);
        dint old = waitNum;
        waitNum = thread->taskCount();
        if (old > waitNum)
            quickIndex = i;
    }
    */
    //优先找等待任务最少且等待时间最少的线程
    dint quickIndex = -1, waitNum = INT_MAX;
    for (dint i=0; i<pdm->threadPool.size(); ++i)
    {
        ExecThread *thread = pdm->threadPool.at(i);
        dint old = waitNum;
        waitNum = thread->taskCount();
        duint timeout = thread->totalTime();
        //永远占用
        if (timeout == UINT_MAX)
            continue;

        if (old > waitNum)
            quickIndex = i;
    }

    //全部占用或没找到可用的线程
    if (quickIndex == -1)
        return eNoIdleThread;

    pdm->threadPool.at(quickIndex)->addTask(task, priority, timeoutInMilliSec);
    return eAdd;
}

#define EXECUTOR_CHECK_TIME_DELTA 0.02
void Executor::checkTaskTimeout(dreal delta)
{
    //控制检测时间频率
    static dreal s_current = 0;
    s_current += delta;
    if (s_current < EXECUTOR_CHECK_TIME_DELTA)
        return;
    s_current = 0;

    SingleLock lock(&pdm->mutex);
    for (dint i=0; i<pdm->threadPool.size(); ++i)
    {
        ExecThread *thread = pdm->threadPool.at(i);
        thread->checkTaskTimeout();
    }
}

DM_INTERNAL_SINGLETON(Executor)

DM_END_NAMESPACE
