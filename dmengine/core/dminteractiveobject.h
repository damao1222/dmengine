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


#ifndef DMINTERACTIVEOBJECT_H
#define DMINTERACTIVEOBJECT_H

#include "dmliveobject.h"

DM_BEGIN_NAMESPACE
class KeyEvent;
class TouchEvent;
DM_PRIVATE_CLASS(InteractiveObject);
class DM_DLL_EXPORT InteractiveObject: public LiveObject
{
    DM_DECLARE_PRIVATE_N(InteractiveObject)
public:
    /** 
     * 构造函数
     */
    InteractiveObject();

    /** 
     * 析构函数
     */
    ~InteractiveObject();

    /** 
     * 判断是否可触摸
     * @return 如果可触摸返回true，否则返回false
     */
    dbool isTouchable() const;

    /** 
     * 设置object是否可以接收触摸事件
     * @param flag  true表示可以接收，false表示不接收
     */
    void setTouchable(dbool flag);

    //internal
    virtual dbool event(Event *e);

protected:
    /** 
     * 当Object接收到按键按下事件时调用
     * @param e  按键事件
     * @return 如果需要丢弃掉该事件，其他Object不再处理，返回true
     */
    virtual dbool onKeyPress(KeyEvent *e);

    /** 
     * 当Object接收到按键抬起事件时调用
     * @param e  按键事件
     * @return 如果需要丢弃掉该事件，其他Object不再处理，返回true
     */
    virtual dbool onKeyRelease(KeyEvent *e);

    /** 
     * 当Object接收到开始触摸的触摸事件时调用
     * @param e  触摸事件
     * @return 如果需要丢弃掉该事件，其他Object不再处理，返回true
     */
    virtual dbool onTouchBegan(TouchEvent *e);

    /** 
     * 当Object接收到滑动触摸事件时调用
     * @param e  触摸事件
     * @return 如果需要丢弃掉该事件，其他Object不再处理，返回true
     */
    virtual dbool onTouchMoved(TouchEvent *e);

    /** 
     * 当Object接收到停止触摸的触摸事件时调用
     * @param e  触摸事件
     * @return 如果需要丢弃掉该事件，其他Object不再处理，返回true
     */
    virtual dbool onTouchEnded(TouchEvent *e);
    
private:
    DM_DISABLE_COPY(InteractiveObject)
};
DM_END_NAMESPACE
#endif // DMINTERACTIVEOBJECT_H
