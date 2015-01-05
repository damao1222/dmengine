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
#ifndef DMJSONREADER_H
#define DMJSONREADER_H

#include "dmvariant.h"
#include "dmdatabuffer.h"
#include "dmutilstring.h"

DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(JsonReader);
class DM_DLL_EXPORT JsonReader
{
    DM_DECLARE_PRIVATE(JsonReader)
public:
    /** 
     * ���캯��
     */
    JsonReader();

    /** 
     * ��������
     */
    ~JsonReader();

    /** 
     * �����ļ�
     * @param fileName  �ļ�·��
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool parse(const char *fileName);

    /** 
     * �����ļ�
     * @param fileName  �ļ�·��
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool parse(const UrlString &fileName);

	/** 
     * �����ڴ�json����
     * @param pData  �ڴ��
	 * @param size  �ڴ���С
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool parse(void *pData, dint64 size);

    /** 
     * ��ý�����ĸ��ڵ�
     * @return ���ڵ�
     */
    const Variant& root() const;

    /** 
     * ����reader�л��������
     */
    void cleanup();
};

DM_END_NAMESPACE

#endif // DMJSONREADER_H
