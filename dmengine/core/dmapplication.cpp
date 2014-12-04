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
    Vector<LiveObject*> objects;
    TouchNotifier *notifier;
    Size resolution;
};

ApplicationPrivate::ApplicationPrivate()
    :objects(100)
    ,notifier(NULL)
{
}

Application::Application():
    CoreApplication(new ApplicationPrivate)
{
}

Application::~Application()
{
    for (int i=0; i<d_func()->objects.size(); ++i)
    {
        d_func()->objects.at(i)->release();
    }
    d_func()->objects.clear();

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
	//init logger
	Logger::getInstance();

    DM_ASSERT_X(self == NULL, "Application cannot init twice");

    d_func()->config = config;
    d_func()->delta = config.frameDelta;

    self = this;
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

dbool Application::activateObject(LiveObject *obj)
{
    if (!this->isRunning())
    {
        DM_LOGW("Application is not running, but get a activate object request");
    }

    if (obj->isActivated())
    {
        DM_LOGI("Notice: Object have been activated!");
        return false;
    }
    //if (!objects.contains(obj))
    {
        //we set false here, let app to know this liveobject is in application and running
        obj->d_func()->__remove = false;
        obj->retain();
        d_func()->objects.append(obj);
        return true;
    }
}

dbool Application::deactivateObject(LiveObject *obj)
{
    /*
    int i = objects.indexOf(obj);
    if (i < 0)
    {
        return false;
    }
    objects.at(i)->d_func()->__remove = true;
    */

    if (!obj->isActivated())
    {
        DM_LOGI("Notice: Object have NOT been activated!");
        return false;
    }
    obj->d_func()->__remove = true;

    return true;
}

void Application::deactivateAll()
{
    for (int i=0; i<d_func()->objects.size(); ++i)
    {
        LiveObject *obj = d_func()->objects.at(i);
        obj->d_func()->__remove = true;
        obj->release();
    }
    d_func()->objects.clear();
}

dbool Application::sendBroadcast(const UtilString &filter, const Variant &msg)
{
    return Broadcast::sendBroadcast(filter, msg);
}

dbool Application::sendBroadcast(const BroadcastFilter &filter, const Variant &msg)
{
    return Broadcast::sendBroadcast(filter, msg);
}

void Application::onframeMove(float dt)
{
    //DM_CHECK_VALID_IT(d_func()->objects, LiveObject, isInActivated, update, dt);
    //检测是否有任务超时
    dmExecutor.checkTaskTimeout(dt);
    int i = 0;
    while (i < d_func()->objects.size()) { 
        LiveObject* t = d_func()->objects.at(i); 
        if (t->isDeactivated()) { 
            d_func()->objects.removeAt(i);
            DM_LOGI("Application::onStep release inactivated object : %p", t); 
            t->release(); 
            continue; 
        } 
        else { 
            t->frameMove(dt); 
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
