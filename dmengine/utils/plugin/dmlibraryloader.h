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
#ifndef DMLIBRARYLOADER_H
#define DMLIBRARYLOADER_H

#include "dmutilstring.h"

enum{
    eLibResolveFailed = 0,
    eLibResolveSuccess = 1,
};

DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(LibraryLoader);
class DM_DLL_EXPORT LibraryLoader
{
    DM_DECLARE_PRIVATE(LibraryLoader)
public:
    /** 
     * ���캯��
     */
    LibraryLoader(const UrlString &libraryFile);

    /** 
     * ��������
     */
    virtual ~LibraryLoader();

    /** 
     * װ�ؿ�
     * @return �ɹ�����true�����򷵻�false
     */
    virtual dbool load() = 0;

    /** 
     * ж�ؿ�
     */
    virtual void unload() = 0;

    /** 
     * ���symbol�ĺ���ָ��
     * @param symbol  ������
     * @param ptr  ƥ���ַ
     * @param logging  �Ƿ��ӡdebug��Ϣ
     * @return �ɹ�����1��ʧ�ܷ���0
     */
    virtual dint resolveExport(const char* symbol, void** ptr, dbool logging = true) = 0;

    /** 
     * �Ƿ��з���
     */
    virtual dbool hasSymbols() = 0;

    /** 
     * ��ÿ�����eg "libplugin.dll"
     * @return ����
     */
    UrlString getName() const;

    /** 
     * ��ÿ�����·����"c://app/libplugin.dll"
     * @return ��·��
     */
    const UrlString& getFullPath() const; 

    /** 
     * ��ÿ���Ŀ¼��"c://app/"
     * @return Ŀ¼
     */
    UrlString getDir() const; // 
};
DM_END_NAMESPACE

#endif // DMLIBRARYLOADER_H
