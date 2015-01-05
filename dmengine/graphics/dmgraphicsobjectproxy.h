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


#ifndef DMGRAPHICSOBJECTPROXY_H
#define DMGRAPHICSOBJECTPROXY_H
#include "dmgraphicsitem.h"

DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT GraphicsObjectProxy
{
public:
    virtual ~GraphicsObjectProxy() 
    { DM_SAFE_RELEASE(m_graphItem); }

    inline GraphicsItem* graphicsItem() const 
    { return m_graphItem; }

    inline bool isCollision(GraphicsObjectProxy *other) const 
    { DM_ASSERT(m_graphItem && other); return m_graphItem->isCollision(other->graphicsItem()); }

    inline void setCenter(const PointF &pos) 
    { DM_ASSERT(m_graphItem); m_graphItem->setCenter(pos); onPaint(); }

    inline PointF center() const 
    { DM_ASSERT(m_graphItem); return m_graphItem->center(); }

    void setGraphicsItem(GraphicsItem *item);

protected:
    GraphicsObjectProxy(GraphicsItem *item): m_graphItem(item) { m_graphItem->retain(); }
    GraphicsObjectProxy(): m_graphItem(NULL) {}
    virtual void onPaint() {}

protected:
    GraphicsItem* m_graphItem;
};

DM_END_NAMESPACE
#endif // DMGRAPHICSOBJECTPROXY_H
