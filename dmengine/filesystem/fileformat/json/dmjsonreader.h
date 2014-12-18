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
     * 构造函数
     */
    JsonReader();

    /** 
     * 析构函数
     */
    ~JsonReader();

    /** 
     * 解析文件
     * @param fileName  文件路径
     * @return 成功返回true，失败返回false
     */
    dbool parse(const char *fileName);

    /** 
     * 解析文件
     * @param fileName  文件路径
     * @return 成功返回true，失败返回false
     */
    dbool parse(const UrlString &fileName);

	/** 
     * 解析内存json数据
     * @param pData  内存块
	 * @param size  内存块大小
     * @return 成功返回true，失败返回false
     */
    dbool parse(void *pData, dint64 size);

    /** 
     * 获得解析后的根节点
     * @return 根节点
     */
    const Variant& root() const;

    /** 
     * 清理reader中缓存的数据
     */
    void cleanup();
};

DM_END_NAMESPACE

#endif // DMJSONREADER_H
