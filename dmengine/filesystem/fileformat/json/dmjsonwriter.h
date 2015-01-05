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
     * 构造函数
     * @param indentation  元素的间隔符
     */
    JsonWriter(const UtilString &indentation = "\t");

    /** 
     * 析构函数
     */
    ~JsonWriter();

    /** 
     * 写json文件
     * @param filePath  目标存放路径
     * @param root  json的根节点
     */
    void write(const char *filePath, const Variant &root );

    /** 
     * 讲variant转换为string
     * @param root  json的根节点
     * @return 转换的字串
     */
    static std::string toString(const Variant &root);
};

DM_END_NAMESPACE

#endif // DMJSONWRITER_H
