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

#ifndef  DMDIR_H
#define  DMDIR_H

#include "dmutilstring.h"
#include "dmchar.h"
#include "dmfileinfo.h"

DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(Dir);
class DM_DECL_EXPORT Dir
{
	DM_DECLARE_PRIVATE(Dir);
public:
	/** 
     * 构造函数
     * @param path 目录路径
     */
	Dir(const UrlString &path);

	/** 
     * 析构函数
     */
	~Dir();

	/** 
     * 判断目录是否存在
     * @return 存在返回true，否则返回false
     */
	dbool exists() const;

	/** 
     * 删除目录
     * @return 成功返回true，否则返回false
     */
	dbool remove();

	/** 
     * 重命名目录
	 * @param newUrl 新目录名
     * @return 成功返回true，否则返回false
     */
	dbool rename(const UrlString &newUrl);

	/** 
     * 创建目录
     * @return 成功返回true，否则返回false
     */
	dbool create();

	/** 
     * 获得目录路径
     * @return 路径
     */
	const UrlString& path() const;

    /** 
     * 获得目录下所有文件列表
     * @param fileFilter 过滤文件类型，默认不过滤
     * @return 文件列表
     */
    FileInfoList fileInfoList(dint fileFilter = FileInfo::eUnknown) const;

    /** 
     * 删除此路径下的所有文件
     * @return 成功返回true，否则返回false
     */
    dbool removeAll();

	/** 
     * 获得目录分隔符
     * @return 分隔符
     */
	static Char separator();

    /** 
     * 增加末尾分隔符
     * @return 拥有末尾分隔符的字串
     */
	static UrlString& addSlashAtEnd(UrlString &url);

	/** 
     * 判断一个目录是否存在
     * @param path 目录路径
     * @return 存在返回true，否则返回false
     */
	static dbool exists(const UrlString &path);

	/** 
     * 创建一个目录
     * @param dir 目录路径
     * @return 成功返回true，否则返回false
     */
	static dbool mkdir(const UrlString &dir);

	/** 
     * 删除一个目录
     * @param dir 目录路径
     * @return 成功返回true，否则返回false
     */
	static dbool rmdir(const UrlString &dir);

	/** 
     * 逐级创建目录
     * @param path 目录路径
     * @return 成功返回true，否则返回false
     */
	static dbool mkpath(const UrlString &path);

	/** 
     * 逐级删除目录（父目录同时删除）
     * @param path 目录路径
     * @return 成功返回true，否则返回false
     */
	static dbool rmpath(const UrlString &path);

    /** 
     * 删除此路径下的所有文件
     * @param path 目录路径
     * @return 成功返回true，否则返回false
     */
    static dbool removeAll(const UrlString &path);
};

DM_END_NAMESPACE
#endif // DMDIR_H