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
#ifndef DMBROADCAST_P_H
#define DMBROADCAST_P_H

#include "dmhash.h"
#include "thread/dmsinglelock.h"
#include "thread/dmloopthread.h"

DM_BEGIN_NAMESPACE  
class Variant;
class UtilString;
class Broadcast;
class BroadcastReceiver;

class BroadcastPrivate : public LoopThread
{
public:
    static BroadcastPrivate* instance;
    static Hash<UtilString, BroadcastReceiver*>& instanceHash();
    static Mutex& instanceLock();
    //internal

    dbool sendBroadcast(const UtilString &action, const Variant &msg);
    dbool sendBroadcast(const BroadcastFilter &filter, const Variant &msg);
    dbool sendBroadcastSync(const UtilString &action, const Variant &msg);
    dbool sendBroadcastSync(const BroadcastFilter &filter, const Variant &msg);
    dbool sendBroadcastAsync(const UtilString &action, const Variant &msg);
    dbool sendBroadcastAsync(const BroadcastFilter &filter, const Variant &msg);
protected:
    void handleEvent(Event *e, EventHandler::Result ret);

private:
    static dbool registReceiver(BroadcastReceiver *reciever);
    static dbool unregistReceiver(BroadcastReceiver *reciever);

    static dbool init(dbool multiThread);
    static dbool cleanup();

    friend class BroadcastReceiver;   
    friend class Application;

    Hash<UtilString, BroadcastReceiver*> recieverMap;
    Mutex mutex;
    dbool multiThread;
};

class BroadcastFilterPrivate
{
public:
    Vector<UtilString> actions;
    friend class BroadcastPrivate;
};

DM_END_NAMESPACE
#endif //DMBROADCAST_P_H
