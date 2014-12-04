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

#include "dmexecutor_test.h"
#include "thread/dmexecutor.h"
#include "thread/dmexectask.h"
#include "thread/dmthread.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE

static Executor *executor = NULL;

class Task1 : public ExecTask
{
public:
	Task1(int id) : i(id), stop(false) {}
	virtual ExecResult run();
	virtual void onTaskDone(void *param, dbool bMainThread);
    virtual void abort(AbortType type);
	volatile ThreadIdentifier id;
	int i;
    volatile dbool stop;
};

ExecResult Task1::run()
{
    while (!stop)
    {
	    Thread::sleep(100);
    }
	id =  Thread::currentThreadId();
	return ExecResult(ExecResult::eCurrentAndMain);
}

void Task1::onTaskDone(void *param, dbool bMainThread)
{
	if (bMainThread)
	{
		DM_LOGI("%d return from main %d", i, id);
		delete this;
	}
	else
	{
		DM_LOGI("%d return from thread %d", i, id);
	}
}

void Task1::abort(AbortType type)
{
    if (type == ExecTask::eQuit)
        stop = true;
    if (type == ExecTask::eTimeout)
        stop = true;
}

dbool ExecutorTest::init()
{
    executor = &dmExecutor;
    executor->init(2);
	return true;
}

void ExecutorTest::cleanup()
{
	//delete executor;
}

void ExecutorTest::run()
{
	executor->run(new Task1(1), 1);
	executor->addTask(new Task1(2), 0, 1);
	executor->addTask(new Task1(3), 0, 1);
	executor->addTask(new Task1(4), 0, 1);

	//while(1)
	{
		Thread::sleep(3000);
        //delete executor;
	}
}
DM_END_NAMESPACE

