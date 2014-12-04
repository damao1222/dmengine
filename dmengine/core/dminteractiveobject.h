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
     * ���캯��
     */
    InteractiveObject();

    /** 
     * ��������
     */
    ~InteractiveObject();

    /** 
     * �ж��Ƿ�ɴ���
     * @return ����ɴ�������true�����򷵻�false
     */
    dbool isTouchable() const;

    /** 
     * ����object�Ƿ���Խ��մ����¼�
     * @param flag  true��ʾ���Խ��գ�false��ʾ������
     */
    void setTouchable(dbool flag);

    //internal
    virtual dbool event(Event *e);

protected:
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
    
private:
    DM_DISABLE_COPY(InteractiveObject)
};
DM_END_NAMESPACE
#endif // DMINTERACTIVEOBJECT_H
