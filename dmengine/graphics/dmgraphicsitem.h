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
#ifndef DMGRAPHICSITEM_H
#define DMGRAPHICSITEM_H
#include "dmobject.h"
#include "dmcollision.h"
#include "dmevent.h"
#include "dmrefptr.h"
#include "dmmetatype.h"

DM_BEGIN_NAMESPACE
#ifdef DM_GRAPHICS_USE_REAL
class LineF;
class RectF;
class CircleF;
class PaintEvent;

class DM_DLL_EXPORT GraphicsItem: public RefPtr<GraphicsItem>
#ifdef DM_GRAPHICS_OBJECT
        , public Object
#endif
{
public:
    /** 
     * 形状类型
     * @return 类型的枚举
     */
    virtual ShapeType type() const { return UnknowShape; }

    /** 
     * 设置形状中心位置
     * @param pos 位置
     */
    virtual void setCenter(const PointF &pos) = 0;

    /** 
     * 获得形状中心点
     * @return 中心位置
     */
    virtual PointF center() const = 0;

    /** 
     * 判断形状之间是否碰撞
     * @param other 判断碰撞的另一个形状
     * @return 碰撞返回true，否则返回false
     */
    virtual dbool isCollision(GraphicsItem *other) = 0;

    /** 
     * 形状的大小矩形
     * @return 形状矩形
     */
    virtual RectF boundingRect() const = 0;

    /** 
     * 判断一点是否在形状中
     * @param pos 判断的点
     * @return 在形状中返回true，否则返回false
     */
    virtual dbool contains(const PointF &pos) const = 0;

    /** 
     * 拷贝形状，调用此方法的代码负责释放拷贝对象
     * @return 拷贝对象指针
     */
    virtual GraphicsItem* clone() const = 0;

#ifdef DM_GRAPHICS_OBJECT
    dbool event(Event *e)
    {
        switch (e->type())
        {
        case Event::PaintEvent:
            onPaintEvent((PaintEvent*)e);
            break;
        default:
            break;
        }
        return Object::event(e);
    }

protected:
    virtual void onPaintEvent(PaintEvent *e) {}
#endif
};

template<class T>
class GraphicsItemImpl: public GraphicsItem
{
    friend class GraphicsLine;
    friend class GraphicsRect;
    friend class GraphicsCircle;
public:
    const T& shape() { return m_shape; }

    dbool isCollision(GraphicsItem *other);
protected:
    GraphicsItemImpl(const T &shape): m_shape(shape){}

private:
    T m_shape;
};

#define DM_GRAPHICSITEM_COLLISION(RET, ITEMA, ITEMB)  \
        do {                                    \
            RET = Collision::isCollision(ITEMA->shape(), ITEMB->shape()); \
        } while(0)

template<class T>
dbool GraphicsItemImpl<T>::isCollision(GraphicsItem *other)
{
    GraphicsItemImpl* item = static_cast<GraphicsItemImpl*>(other);
    dbool ret = false;
    DM_GRAPHICSITEM_COLLISION(ret, this, item);
    return ret;
}

#define DECLARE_CENTER_FUNC     \
        public:                 \
            void setCenter(const PointF &pos) { m_shape.setCenter(pos); } \
            PointF center() const { return m_shape.center(); }
/*
class DM_DLL_EXPORT GraphicsLine: public GraphicsObjectImpl<Line>
{
public:
    GraphicsLine(const Line &shape): GraphicsObjectImpl<Line>(shape) {}
    virtual ShapeType type() const { return LineShape; }

    DECLARE_CENTER_FUNC
};
*/
class DM_DLL_EXPORT GraphicsRect: public GraphicsItemImpl<RectF>
{
public:
    GraphicsRect(const RectF &shape): GraphicsItemImpl<RectF>(shape) {}
    GraphicsRect(dreal x, dreal y, dreal width, dreal height): GraphicsItemImpl<RectF>(RectF(x, y, width, height)) {}
    GraphicsRect(const GraphicsRect &other): GraphicsItemImpl<RectF>(other.m_shape) {}
    virtual ShapeType type() const { return RectShape; }

    RectF boundingRect() const { return m_shape; }
    dbool contains(const PointF &pos) const { return m_shape.contains(pos); }

    GraphicsItem* clone() const;

    DECLARE_CENTER_FUNC
};

class DM_DLL_EXPORT GraphicsCircle: public GraphicsItemImpl<CircleF>
{
public:
    GraphicsCircle(const CircleF &shape): GraphicsItemImpl<CircleF>(shape) {}
    GraphicsCircle(dreal x, dreal y, dreal radius): GraphicsItemImpl<CircleF>(CircleF(x, y, radius)) {}
    GraphicsCircle(const GraphicsCircle &other): GraphicsItemImpl<CircleF>(other.m_shape) {}
    virtual ShapeType type() const { return CircleShape; }

    RectF boundingRect() const { return m_shape.boundingRect(); }
    dbool contains(const PointF &pos) const { return m_shape.contains(pos); }

    GraphicsItem* clone() const;
    
    DECLARE_CENTER_FUNC
};
#else
class Line;
class Rect;
class Circle;
class PaintEvent;

class DM_DLL_EXPORT GraphicsItem: public RefPtr<GraphicsItem>
#ifdef DM_GRAPHICS_OBJECT
        , public Object
#endif
{
public:
    virtual ShapeType type() const { return UnknowShape; }

    virtual void setCenter(const Point &pos) = 0;
    virtual Point center() const = 0;
    virtual dbool isCollision(GraphicsItem *other) = 0;

    virtual Rect boundingRect() const = 0;
    virtual dbool contains(const Point &pos) const = 0;

#ifdef DM_GRAPHICS_OBJECT
    dbool event(Event *e)
    {
        switch (e->type())
        {
        case Event::PaintEvent:
            onPaintEvent((PaintEvent*)e);
            break;
        default:
            break;
        }
        return Object::event(e);
    }

protected:
    virtual void onPaintEvent(PaintEvent *e) {}
#endif
};

template<class T>
class DM_DLL_EXPORT GraphicsObjectImpl: public GraphicsItem
{
    friend class GraphicsLine;
    friend class GraphicsRect;
    friend class GraphicsCircle;
public:
    const T& shape() { return m_shape; }

    dbool isCollision(GraphicsItem *other);
protected:
    GraphicsObjectImpl(const T &shape): m_shape(shape){}

private:
    T m_shape;
};

#define DM_GRAPHICSOBJECT_COLLISION(RET, OBJA, OBJB)  \
        do {                                    \
            RET = Collision::isCollision(OBJA->shape(), OBJB->shape()); \
        } while(0)

template<class T>
dbool GraphicsObjectImpl<T>::isCollision(GraphicsItem *other)
{
    GraphicsObjectImpl* obj = static_cast<GraphicsObjectImpl*>(other);
    dbool ret = false;
    DM_GRAPHICSOBJECT_COLLISION(ret, this, obj);
    return ret;
}

#define DECLARE_CENTER_FUNC     \
        public:                 \
            void setCenter(const Point &pos) { m_shape.setCenter(pos); } \
            Point center() const { return m_shape.center(); }
/*
class DM_DLL_EXPORT GraphicsLine: public GraphicsObjectImpl<Line>
{
public:
    GraphicsLine(const Line &shape): GraphicsObjectImpl<Line>(shape) {}
    virtual ShapeType type() const { return LineShape; }

    DECLARE_CENTER_FUNC
};
*/
class DM_DLL_EXPORT GraphicsRect: public GraphicsObjectImpl<Rect>
{
public:
    GraphicsRect(const Rect &shape): GraphicsObjectImpl<Rect>(shape) {}
    GraphicsRect(dint x, dint y, dint width, dint height): GraphicsObjectImpl<Rect>(Rect(x, y, width, height)) {}
    GraphicsRect(const GraphicsRect &other): GraphicsObjectImpl<Rect>(other.m_shape) {}
    virtual ShapeType type() const { return RectShape; }

    Rect boundingRect() const { return m_shape; }
    dbool contains(const Point &pos) const { return m_shape.contains(pos); }

    DECLARE_CENTER_FUNC
};

class DM_DLL_EXPORT GraphicsCircle: public GraphicsObjectImpl<Circle>
{
public:
    GraphicsCircle(const Circle &shape): GraphicsObjectImpl<Circle>(shape) {}
    GraphicsCircle(int x, int y, int radius): GraphicsObjectImpl<Circle>(Circle(x, y, radius)) {}
    GraphicsCircle(const GraphicsCircle &other): GraphicsObjectImpl<Circle>(other.m_shape) {}
    virtual ShapeType type() const { return CircleShape; }

    Rect boundingRect() const { return m_shape.boundingRect(); }
    dbool contains(const Point &pos) const { return m_shape.contains(pos); }
    
    DECLARE_CENTER_FUNC
};
#endif
DM_END_NAMESPACE

DM_DECLARE_METATYPE(GraphicsItem*);
#endif //DMGRAPHICSITEM_H
