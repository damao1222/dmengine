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
#ifndef DMMD5_H
#define DMMD5_H
#include "dmnamespace.h"
#include "dmutilstring.h"

DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(MD5);
class DM_DLL_EXPORT MD5
{
    DM_DECLARE_PRIVATE(MD5)
public:
    /** 
     * 构造函数
     */
    MD5();

    /** 
     * 析构函数
     */
    ~MD5();

    /** 
     * 增加数据
     * @param inBuf  数据地址
     * @param inLen  数据长度
     */
    void append(const void *inBuf, size_t inLen);

    /** 
     * 增加数据
     * @param str 数据字串
     */
    void append(const UtilString &str);

    /** 
     * 获得md5码
     * @param digest 保存md5码的数组
     */
    void getDigest(unsigned char digest[16]) const;

    /** 
     * 获得md5码
     * @param digest 保存md5码的字串
     */
    void getDigest(UtilString &digest) const;

    /** 
     * 获得md5码
     * @return md5码
     */
    UtilString getDigest() const;
    
    /** 
     * 由数据字串获得md5码
     * @param text  用来生成md5码的字串
     * @return md5码
     */
    static UtilString getMD5(const UtilString &text);

    /** 
     * 由数据段获得md5码
     * @param inBuf  数据地址
     * @param inLen  数据长度
     * @return md5码
     */
    static UtilString getMD5(const void *inBuf, size_t inLen);
};
DM_END_NAMESPACE
#endif // DMMD5_H
