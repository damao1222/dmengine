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
#ifndef DMFILE_H
#define DMFILE_H
#include "dmifile.h"
DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(File);
class DM_DLL_EXPORT File
{
    DM_DECLARE_PRIVATE(File)
public:
    /** 
     * 构造函数
     */
    File();

    /** 
     * 构造函数
     * @param url 文件路径
     */
    explicit File(const UrlString& url);

    /** 
     * 析构函数
     */
    ~File();

    /** 
     * 打开文件
     * @param mode 打开模式，见枚举EOpenMode的定义
     * @return 成功返回true，否则返回false
     */
    dbool open(EOpenMode mode = eO_ReadOnly);

    /** 
     * 打开文件
     * @param url 文件路径
     * @param mode 打开模式，见枚举EOpenMode的定义
     * @return 成功返回true，否则返回false
     */
    dbool open(const UrlString& url, EOpenMode mode = eO_ReadOnly);

    /** 
     * 判断文件是否存在
     * @return 成功返回true，否则返回false
     */
    dbool exists() const;

    /** 
     * 获得文件状态
     * @param url 文件路径
     * @param buffer 状态结构体
     * @return 执行成功返回0，失败返回-1
     */
    dint stat(const UrlString& url, struct __stat64* buffer);

    /** 
     * 获得文件状态
     * @param buffer 状态结构体
     * @return 执行成功返回0，失败返回-1
     */
    dint stat(struct __stat64* buffer);

    /** 
     * 读文件内容
     * @param lpBuf 存储缓存地址
     * @param uiBufSize 缓存大小
     * @return 实际读取内容的大小
     */
    duint read(void* lpBuf, dint64 uiBufSize);

    /** 
     * 向文件中写内容
     * @param lpBuf 内容地址
     * @param uiBufSize 内容大小
     * @return windows 成功返回1，失败返回0，linux 返回实际写入的大小
     */
    dint write(const void* lpBuf, dint64 uiBufSize);

    /** 
     * 按文本读取文件内容
     * @param szLine 存储内容地址
     * @param iLineLength 缓存大小
     * @return 成功返回true，失败返回false
     */
    dbool readString(char *szLine, dint iLineLength);

    /** 
     * 跳转
     * @param iFilePosition 偏移量
     * @param iWhence 偏移位置
                SEEK_SET： 文件开头
                SEEK_CUR： 当前位置
     　　       SEEK_END： 文件结尾
     * @return 成功返回true，失败返回false
     */
    dint64 seek(dint64 iFilePosition, dint iWhence = SEEK_SET);

    /** 
     * 关闭文件
     */
    void close();

    /** 
     * 获得当前文件偏移量
     * @return 偏移量
     */
    dint64 getPosition() const;

    /** 
     * 获得文件大小
     * @return 文件大小
     */
    dint64 getLength() const;

    /** 
     * 块大小（无效）
     * @return 
     */
    dint getChunkSize() const;

    /** 
     * 刷新内部缓存，将RAM中内容写入磁盘
     */
    void flush();

    /** 
     * 删除文件
     * @return 成功返回true，失败返回false
     */
    dbool remove();

    /** 
     * 重命名文件
     * @param newUrl 新文件名
     * @return 成功返回true，失败返回false
     */
    dbool rename(const UrlString& newUrl);

    /** 
     * 隐藏文件
     * @param hidden 设置为true隐藏文件，false显示文件
     * @return 成功返回true，失败返回false
     */
    dbool setHidden(dbool hidden);

    /** 
     * IO控制
     * @param request 请求
     * @param param 参数
     * @return 成功返回0，否则返回其他值
     */
    dint ioctl(EIoControl request, void* param);

    /** 
     * 获取内容（目前无效）
     * @return 文件内容
     */
    UtilString getContent() const;

    /** 
     * 获取文件路径
     * @return 路径
     */
    const UrlString& path() const;

    /** 
     * 获取文件所有内容
     * @param pBuf 缓存地址
     * @param uSize 缓存大小
     * @return 成功返回true，否则返回false
     */
    dbool getAllDatas(dbyte *&pBuf, dint64 &uSize);

    /** 
     * 判定文件是否存在
     * @param url 文件路径
     * @return 存在返回true，否则返回false
     */
    static dbool exists(const UrlString& url);

    /** 
     * 删除文件
     * @param url 文件路径
     * @return 成功返回true，否则返回false
     */
    static dbool remove(const UrlString& url);

    /** 
     * 重命名文件
     * @param url 文件路径
     * @param newUrl 新文件名
     * @return 成功返回true，否则返回false
     */
    static dbool rename(const UrlString& url, const UrlString& newUrl);

    /** 
     * 设置文件隐藏属性
     * @param url 文件路径
     * @param hidden true为隐藏，false为非隐藏
     * @return 成功返回true，否则返回false
     */
    static dbool setHidden(const UrlString& url, dbool hidden);
};
DM_END_NAMESPACE

#endif // DMFILE_H
