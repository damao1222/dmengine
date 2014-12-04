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
     * ��Object��ý���ʱ����
     * @param e  �����¼�
     */
    virtual void onFocusInEvent(FocusEvent *e);

    /** 
     * ��Objectʧȥ����ʱ����
     * @param e  �����¼�
     */
    virtual void onFocusOutEvent(FocusEvent *e);

    /** 
     * ��Object��ʾ֮�����
     * @param e  �����¼�
     */
    virtual void onShowEvent(ShowEvent *e);

    /** 
     * ��Object�������ر�ʱ����
     * @param e  �����¼�
     */
    virtual void onCloseEvent(CloseEvent *e);

    //input event from InteractiveObject
#if 0
    /** 
     * ��Object���յ����������¼�ʱ����
     * @param e  �����¼�
     * @return �����Ҫ���������¼�������Object���ٴ�������true
     */
    virtual dbool onKeyPress(KeyEvent *e);

    /** 
     * ��Object���յ�����̧���¼�ʱ����
     * @param e  �����¼�
     * @return �����Ҫ���������¼�������Object���ٴ�������true
     */
    virtual dbool onKeyRelease(KeyEvent *e);

    /** 
     * ��Object���յ���ʼ�����Ĵ����¼�ʱ����
     * @param e  �����¼�
     * @return �����Ҫ���������¼�������Object���ٴ�������true
     */
    virtual dbool onTouchBegan(TouchEvent *e);

    /** 
     * ��Object���յ����������¼�ʱ����
     * @param e  �����¼�
     * @return �����Ҫ���������¼�������Object���ٴ�������true
     */
    virtual dbool onTouchMoved(TouchEvent *e);

    /** 
     * ��Object���յ�ֹͣ�����Ĵ����¼�ʱ����
     * @param e  �����¼�
     * @return �����Ҫ���������¼�������Object���ٴ�������true
     */
    virtual dbool onTouchEnded(TouchEvent *e);
#endif
};
DM_END_NAMESPACE
#endif // DMWIDGET_H
