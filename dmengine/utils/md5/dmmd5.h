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
     * ���캯��
     */
    MD5();

    /** 
     * ��������
     */
    ~MD5();

    /** 
     * ��������
     * @param inBuf  ���ݵ�ַ
     * @param inLen  ���ݳ���
     */
    void append(const void *inBuf, size_t inLen);

    /** 
     * ��������
     * @param str �����ִ�
     */
    void append(const UtilString &str);

    /** 
     * ���md5��
     * @param digest ����md5�������
     */
    void getDigest(unsigned char digest[16]) const;

    /** 
     * ���md5��
     * @param digest ����md5����ִ�
     */
    void getDigest(UtilString &digest) const;

    /** 
     * ���md5��
     * @return md5��
     */
    UtilString getDigest() const;
    
    /** 
     * �������ִ����md5��
     * @param text  ��������md5����ִ�
     * @return md5��
     */
    static UtilString getMD5(const UtilString &text);

    /** 
     * �����ݶλ��md5��
     * @param inBuf  ���ݵ�ַ
     * @param inLen  ���ݳ���
     * @return md5��
     */
    static UtilString getMD5(const void *inBuf, size_t inLen);
};
DM_END_NAMESPACE
#endif // DMMD5_H
