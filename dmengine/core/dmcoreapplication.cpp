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

#include "dmcoreapplication.h"
#include "private/dmcoreapplication_p.h"
#include "dmlogger.h"
#include "thread/dmsinglelock.h"
#include "thread/dmthread.h"
#include "events/dmpostevent.h"
#include "dmtime.h"
#include "dmappconfig.h"

DM_BEGIN_NAMESPACE
CoreApplication *CoreApplication::self = 0;

static int global_init()
{
    Thread::globalInit();
    return 1;
}

DM_CONSTRUCTOR_FUNCTION(global_init);

CoreApplicationPrivate::~CoreApplicationPrivate()
{
    //do nothing
	//init thread
	Thread::globalInit();
}

dbool CoreApplicationPrivate::filterEvent(Object *obj, Event *e)
{
    for (int i=0; i<filterList.size(); ++i)
    {
        if (filterList.at(i)->filter(obj, e))
        {
            return true;
        }
    }

    return false;
}

dbool CoreApplicationPrivate::sendEvent(Object *obj, Event *e)
{
    e->retain();
    //默认丢弃此事件
    dbool ret = true;
    if (!filterEvent(obj, e))
    {
        ret = obj->event(e);
    }

	switch (e->type())
	{
	case eDeferredDeleteEvent:
		DM_SAFE_DELETE(obj);
		break;
	case eDeferredReleaseEvent:
		DM_SAFE_RELEASE(obj);
		break;
	default:
		break;
	}

	e->release();
    return ret;
}

dbool CoreApplicationPrivate::sendEvent(PostEvent *post)
{
	//默认丢弃此事件
	dbool ret = true;
	if (!filterEvent(post->o, post->e))
	{
		ret = post->o->event(post->e);
	}

	switch (post->e->type())
	{
	case eDeferredDeleteEvent:
		DM_SAFE_DELETE(post->o);
		post->o = NULL;
		break;
	case eDeferredReleaseEvent:
		DM_SAFE_RELEASE(post->o);
		post->o = NULL;
		break;
	default:
		break;
	}
	return ret;
}

void CoreApplicationPrivate::processPostEvent(float dt)
{
    Event *e = NULL;
    while (handler.getAndRetain(&e, 0) == EventHandler::eOK)
    {
        PostEvent *pe = (PostEvent*)e;
        sendEvent(pe);
        pe->release();
    }
}

CoreApplication::CoreApplication():
    C_D(CoreApplication)
{
    pdm->mainThreadId = Thread::currentThreadId();
    pdm->quit = false;
    pdm->stop = false;
    pdm->pausetime = 0;
    pdm->lastpause = 0;
    pdm->delta = DM_APP_DELTA;
    pdm->handler.start();
    //获得当前时间
    Time::getAppClockMillis();
}

CoreApplication::CoreApplication(CoreApplicationPrivate *data) :
    pdm(data)
{
    pdm->mainThreadId = Thread::currentThreadId();
    pdm->quit = false;
    pdm->stop = false;
    pdm->pausetime = 0;
    pdm->lastpause = 0;
    pdm->delta = DM_APP_DELTA;
    pdm->handler.start();
    //获得当前时间
    Time::getAppClockMillis();
}

CoreApplication::~CoreApplication()
{
    D_D(CoreApplication);
}

dbool CoreApplication::isMainThread()
{
    return Thread::isCurrentThread(self->d_func()->mainThreadId);
}

dbool CoreApplication::sendEvent(Object *obj, Event *e)
{
    DM_ASSERT(self && obj && e);
    return self->d_func()->sendEvent(obj, e);
}

void CoreApplication::postEvent(Object *obj, Event *e, dint priority /*= 0*/)
{
    DM_ASSERT(self && obj && e);

    //主线程直接发送
    if (isMainThread())
    {
        sendEvent(obj, e);
    }
    else
    {
        {
            SingleRefPtr<PostEvent> ref(new PostEvent(obj, e));
            self->pdm->handler.put(ref.get(), priority);
        }
    }
}

CoreApplication* CoreApplication::global()
{
    return self;
}

dbool CoreApplication::addEventFilter(EventFilter *filter)
{
    DM_ASSERT(filter);
    if (self->d_func()->filterList.indexOf(filter) > -1)
    {
        DM_LOGW("filter have been added in CoreApplication");
        return false;
    }
    self->d_func()->filterList.append(filter);
    return true;
}

dbool CoreApplication::removeEventFilter(EventFilter *filter)
{
    return self->d_func()->filterList.removeOne(filter);
}

void CoreApplication::start()
{ 
    pdm->stop = false; 
    pdm->lastpause = 0; 
    pdm->pausetime = 0; 
    pdm->handler.start();
}

void CoreApplication::stop()
{ 
    pdm->stop = true; 
    pdm->handler.stop();
}

void CoreApplication::pause(float timeout)
{ 
    pdm->lastpause = 0; 
    pdm->pausetime = timeout; 
}

dbool CoreApplication::isStopped() const
{ 
    return pdm->stop; 
}

dbool CoreApplication::isRunning() const
{ 
    return !pdm->stop && !isPaused(); 
}

dbool CoreApplication::isPaused() const
{ 
    return pdm->pausetime > 0 && pdm->lastpause < pdm->pausetime; 
}

void CoreApplication::frameMove(float dt)
{
    //应用每次得到时间片就将释放auto release的对象
    AutoReleaseManager::instance()->releaseAll();

    //handle post events
    pdm->processPostEvent(dt);

    if (!pdm->stop)
    {
        //pause
        if (pdm->pausetime > 0)
        {
            pdm->lastpause += dt;
            if (pdm->lastpause < pdm->pausetime)
                return ;
            else
                //We done
                pdm->pausetime = 0;
        }
        onframeMove(dt);
    }
}

dint CoreApplication::loop()
{
    duint lastTime, currentTime, delta;
    delta = 0;
    lastTime = currentTime = Time::getAppClockMillis();

    while (!pdm->quit) 
    {
        delta = currentTime - lastTime;
        if (lastTime != currentTime)
        {
            frameMove(static_cast<dreal>(delta)/1000);
        }
        lastTime = currentTime;

        //休眠一段时间，释放CPU
        if (delta < pdm->delta)
        {
#ifdef DM_DEBUG
//            DM_LOGI("sleep time : %d", pdm->delta - delta);
#endif
            Thread::sleep(pdm->delta - delta);
        }
        currentTime = Time::getAppClockMillis();
    }

    return pdm->quit ? 1 : 0;
}

void CoreApplication::quit()
{
    pdm->quit = true;
}

DM_END_NAMESPACE
