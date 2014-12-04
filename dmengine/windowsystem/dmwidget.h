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
#ifndef DMWIDGET_H
#define DMWIDGET_H

#include "dminteractiveobject.h"

DM_BEGIN_NAMESPACE
class ShowEvent;
class CloseEvent;
class FocusEvent;
class WindowSystem;
DM_PRIVATE_CLASS(Widget);
class DM_DLL_EXPORT Widget : public InteractiveObject
{
    DM_DECLARE_PRIVATE(Widget)
public:
    Widget();
    ~Widget();

    virtual void show();
    virtual void close();

    virtual void setVisible(dbool visible);
    virtual dbool isVisible() const;

    void requestFocus();
    dbool hasFocus() const;

    //internal
    virtual dbool event(Event *e);
protected:
    //onFocusInEvent -> onShowEvent -> onCloseEvent -> onFocusOutEvent
    /** 
     * 当Object获得焦点时调用
     * @param e  按键事件
     */
    virtual void onFocusInEvent(FocusEvent *e);

    /** 
     * 当Object失去焦点时调用
     * @param e  按键事件
     */
    virtual void onFocusOutEvent(FocusEvent *e);

    /** 
     * 当Object显示之后调用
     * @param e  按键事件
     */
    virtual void onShowEvent(ShowEvent *e);

    /** 
     * 当Object即将被关闭时调用
     * @param e  按键事件
     */
    virtual void onCloseEvent(CloseEvent *e);

    //input event from InteractiveObject
#if 0
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
#endif
};
DM_END_NAMESPACE
#endif // DMWIDGET_H
