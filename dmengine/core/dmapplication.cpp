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

#include "dmapplication.h"
#include "private/dmcoreapplication_p.h"
#include "dmappconfig.h"
#include "dmvector.h"
#include "dmautoreleasemgr.h"
#include "private/dmliveobject_p.h"
#include "dmlogger.h"
#include "dminteractiveobject.h"
#include "private/dmbroadcast_p.h"
#include "dmbroadcast.h"
#include "thread/dmthread.h"
#include "thread/dmexecutor.h"
#include "dmwindowsystem.h"
#include "dmtimer.h"
#define DM_HAVE_HTTP
#ifdef DM_HAVE_HTTP
#include "http/dmhttpclient.h"
#endif

DM_BEGIN_NAMESPACE
class ApplicationPrivate : public CoreApplicationPrivate
{
public:
    ApplicationPrivate();
    AppConfig config;
    Vector<TimerProxy*> timers;
    TouchNotifier *notifier;
    Size resolution;
};

ApplicationPrivate::ApplicationPrivate()
    :timers(100)
    ,notifier(NULL)
{
}

Application::Application():
    CoreApplication(new ApplicationPrivate)
{
}

Application::~Application()
{
    for (int i=0; i<d_func()->timers.size(); ++i)
    {
        d_func()->timers.at(i)->release();
    }
    d_func()->timers.clear();

    DM_INTERNAL_FUNCNAME(final)();
    D_D(Application);
}

void Application::setResolution(const Size &res)
{
    d_func()->resolution = res;
}

void Application::init()
{
    init(AppConfig());
}

void Application::init(const AppConfig &config)
{
    static dbool _g_inited = false;
    DM_ASSERT_X(!_g_inited, "Application only can initial once");
    _g_inited = true;
	//init logger
	Logger::getInstance();

    d_func()->config = config;
    d_func()->delta = config.frameDelta;

    AutoReleaseManager::getInstance();

    //是否启动广播线程
    BroadcastPrivate::init(config.enableBroadcastMultiThread);
    WindowSystem::getInstance();

    Executor::getInstance()->init(config.executorThreadNumber);

#ifdef DM_HAVE_HTTP
    HttpClient::getInstance();
#endif
}

void Application::DM_INTERNAL_FUNCNAME(final)()
{
#ifdef DM_HAVE_HTTP
    HttpClient::releaseInstance();
#endif
    Executor::releaseInstance();
    WindowSystem::releaseInstance();
    BroadcastPrivate::cleanup();
    AutoReleaseManager::releaseInstance();
	Logger::releaseInstance();
}

dbool Application::activateTimer(const Timer &timer)
{
    if (!isMainThread())
    {
        DM_LOGW("start timer must in main thread!");
        return false;
    }

    if (!this->isRunning())
    {
        DM_LOGW("Application is not running, but get a activate object request");
    }

    TimerProxy *proxy = timer.d_func()->proxy;
    if (!proxy || !proxy->remove_mark)
    {
        return false;
    }

    proxy->remove_mark = false;
    proxy->retain();
    d_func()->timers.append(proxy);
    return true;
}

dbool Application::deactivateTimer(const Timer &timer)
{
    if (!isMainThread())
    {
        DM_LOGW("stop timer must in main thread!");
        return false;
    }

    TimerProxy *proxy = timer.d_func()->proxy;
    if (!proxy || proxy->remove_mark)
    {
        return false;
    }

    proxy->remove_mark = true;
    proxy->timer = NULL;
    return true;
}

void Application::deactivateAllTimers()
{
    for (int i=0; i<d_func()->timers.size(); ++i)
    {
        TimerProxy *timer = d_func()->timers.at(i);
        timer->remove_mark = true;
        timer->timer = NULL;
    }
}

dbool Application::sendBroadcast(const UtilString &filter, const Variant &msg)
{
    return Broadcast::sendBroadcast(filter, msg);
}

dbool Application::sendBroadcast(const BroadcastFilter &filter, const Variant &msg)
{
    return Broadcast::sendBroadcast(filter, msg);
}

Application* Application::getInstance()
{
    if (self == NULL)
    {
        self = new Application;
    }
    return (Application*)self;
}

Application* Application::instance()
{
    return (Application*)self;
}

void Application::releaseInstance()
{
    DM_SAFE_DELETE(self);
}

void Application::onframeMove(float dt)
{
    //DM_CHECK_VALID_IT(d_func()->objects, LiveObject, isInActivated, update, dt);
    //检测是否有任务超时
    dmExecutor.checkTaskTimeout(dt);
    int i = 0;
    while (i < d_func()->timers.size()) { 
        TimerProxy *timer = d_func()->timers.at(i); 
        if (timer->remove_mark) { 
            d_func()->timers.removeAt(i);
            timer->release(); 
            continue; 
        } 
        else { 
            timer->checkTime(dt);
            ++i; 
        }
    }; 
}

const Size& Application::resolution() const
{
    return d_func()->resolution;
}

dint Application::width() const
{
    return d_func()->resolution.width();
}

dint Application::height() const
{
    return d_func()->resolution.height();
}

void Application::setTouchDispatcher(TouchNotifier *notifier)
{
    if (d_func()->notifier != notifier)
    {
        d_func()->notifier = notifier;

        if (d_func()->notifier)
            d_func()->notifier->registDispatcher(this);
    }
}

void Application::pushDisplayLayer(DisplayLayer *layer)
{
    g_windowSystem.pushDisplayLayer(layer);
}

void Application::popDisplayLayer()
{
    g_windowSystem.popDisplayLayer();
}

void Application::processTouchEvent(TouchEvent *e)
{
    g_windowSystem.handleTouchEvent(e);
}

void Application::processKeyEvent(KeyEvent *e)
{
    g_windowSystem.handleKeyEvent(e);
}

#if 0
void Application::processTouchEvent(TouchEvent *e)
{
    int i = 0;
    while (i < d_func()->objects.size()) { 
        LiveObject* t = d_func()->objects.at(i); 
        if (t->isAcceptInput())
        {
            InteractiveObject *io = (InteractiveObject*)t;
            if (io->isTouchable())
            {
                //CoreApplication API, Include event filter.
                if (sendEvent(t, e))
                    break;
            }
        }
        ++i; 
    }; 
}
#endif

/*
static void app_cleanup()
{
    Application::releaseInstance();
}
DM_DESTRUCTOR_FUNCTION(app_cleanup)
*/
DM_END_NAMESPACE
