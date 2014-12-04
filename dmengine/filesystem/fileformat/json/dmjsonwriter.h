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
#ifndef DMJSONWRITER_H
#define DMJSONWRITER_H

#include "dmglobal.h"
#include "dmutilstring.h"

DM_BEGIN_NAMESPACE
class Variant;
DM_PRIVATE_CLASS(JsonWriter);
class DM_DLL_EXPORT JsonWriter
{
    DM_DECLARE_PRIVATE(JsonWriter)
public:
    /** 
     * ���캯��
     * @param indentation  Ԫ�صļ����
     */
    JsonWriter(const UtilString &indentation = "\t");

    /** 
     * ��������
     */
    ~JsonWriter();

    /** 
     * дjson�ļ�
     * @param filePath  Ŀ����·��
     * @param root  json�ĸ��ڵ�
     */
    void write(const char *filePath, const Variant &root );

    /** 
     * ��variantת��Ϊstring
     * @param root  json�ĸ��ڵ�
     * @return ת�����ִ�
     */
    static std::string toString(const Variant &root);
};

DM_END_NAMESPACE

#endif // DMJSONWRITER_H
