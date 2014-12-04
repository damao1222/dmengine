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

#ifndef  DMFILEINFO_H
#define  DMFILEINFO_H

#include "dmutilstring.h"
#include "dmvector.h"

DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(FileInfo);
class DM_DECL_EXPORT FileInfo
{
	DM_DECLARE_PRIVATE(FileInfo);
public:
    enum EFileType{
        eUnknown = 0,
        eDir = 1,
        eFile = 1<<1,
        eAll = eDir | eFile
    };

	/** 
     * 构造函数
     * @param path 目录路径
     */
	FileInfo(const UrlString &path, EFileType type);

    /** 
     * 拷贝构造函数
     * @param other 拷贝对象
     */
    FileInfo(const FileInfo &other);

	/** 
     * 析构函数
     */
	~FileInfo();

    /** 
     * 判断是否为目录
     * @return 是目录返回true，否则返回false
     */
    dbool isDir() const;

    /** 
     * 获得文件路径
     * @return 路径
     */
    const UrlString& getPath() const;

    /** 
     * 赋值运算符重载
     * @param other 赋值对象
     * @return 赋值后当前对象的引用
     */
    FileInfo& operator=(const FileInfo &other);

    /** 
     * 等于运算符重载
     * @param other 比较对象
     * @return 相等返回true，否则返回false
     */
    dbool operator==(const FileInfo &other) const;

     /** 
     * 不等运算符重载
     * @param other 比较对象
     * @return 不相等返回true，否则返回false
     */
    dbool operator!=(const FileInfo &other) const;
};

typedef Vector<FileInfo> FileInfoList;

DM_END_NAMESPACE
#endif // DMFILEINFO_H