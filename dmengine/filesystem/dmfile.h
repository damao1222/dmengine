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
     * ���캯��
     */
    File();

    /** 
     * ���캯��
     * @param url �ļ�·��
     */
    explicit File(const UrlString& url);

    /** 
     * ��������
     */
    ~File();

    /** 
     * ���ļ�
     * @param mode ��ģʽ����ö��EOpenMode�Ķ���
     * @return �ɹ�����true�����򷵻�false
     */
    dbool open(EOpenMode mode = eO_ReadOnly);

    /** 
     * ���ļ�
     * @param url �ļ�·��
     * @param mode ��ģʽ����ö��EOpenMode�Ķ���
     * @return �ɹ�����true�����򷵻�false
     */
    dbool open(const UrlString& url, EOpenMode mode = eO_ReadOnly);

    /** 
     * �ж��ļ��Ƿ����
     * @return �ɹ�����true�����򷵻�false
     */
    dbool exists() const;

    /** 
     * ����ļ�״̬
     * @param url �ļ�·��
     * @param buffer ״̬�ṹ��
     * @return ִ�гɹ�����0��ʧ�ܷ���-1
     */
    dint stat(const UrlString& url, struct __stat64* buffer);

    /** 
     * ����ļ�״̬
     * @param buffer ״̬�ṹ��
     * @return ִ�гɹ�����0��ʧ�ܷ���-1
     */
    dint stat(struct __stat64* buffer);

    /** 
     * ���ļ�����
     * @param lpBuf �洢�����ַ
     * @param uiBufSize �����С
     * @return ʵ�ʶ�ȡ���ݵĴ�С
     */
    duint read(void* lpBuf, dint64 uiBufSize);

    /** 
     * ���ļ���д����
     * @param lpBuf ���ݵ�ַ
     * @param uiBufSize ���ݴ�С
     * @return windows �ɹ�����1��ʧ�ܷ���0��linux ����ʵ��д��Ĵ�С
     */
    dint write(const void* lpBuf, dint64 uiBufSize);

    /** 
     * ���ı���ȡ�ļ�����
     * @param szLine �洢���ݵ�ַ
     * @param iLineLength �����С
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool readString(char *szLine, dint iLineLength);

    /** 
     * ��ת
     * @param iFilePosition ƫ����
     * @param iWhence ƫ��λ��
                SEEK_SET�� �ļ���ͷ
                SEEK_CUR�� ��ǰλ��
     ����       SEEK_END�� �ļ���β
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dint64 seek(dint64 iFilePosition, dint iWhence = SEEK_SET);

    /** 
     * �ر��ļ�
     */
    void close();

    /** 
     * ��õ�ǰ�ļ�ƫ����
     * @return ƫ����
     */
    dint64 getPosition() const;

    /** 
     * ����ļ���С
     * @return �ļ���С
     */
    dint64 getLength() const;

    /** 
     * ���С����Ч��
     * @return 
     */
    dint getChunkSize() const;

    /** 
     * ˢ���ڲ����棬��RAM������д�����
     */
    void flush();

    /** 
     * ɾ���ļ�
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool remove();

    /** 
     * �������ļ�
     * @param newUrl ���ļ���
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool rename(const UrlString& newUrl);

    /** 
     * �����ļ�
     * @param hidden ����Ϊtrue�����ļ���false��ʾ�ļ�
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool setHidden(dbool hidden);

    /** 
     * IO����
     * @param request ����
     * @param param ����
     * @return �ɹ�����0�����򷵻�����ֵ
     */
    dint ioctl(EIoControl request, void* param);

    /** 
     * ��ȡ���ݣ�Ŀǰ��Ч��
     * @return �ļ�����
     */
    UtilString getContent() const;

    /** 
     * ��ȡ�ļ�·��
     * @return ·��
     */
    const UrlString& path() const;

    /** 
     * ��ȡ�ļ���������
     * @param pBuf �����ַ
     * @param uSize �����С
     * @return �ɹ�����true�����򷵻�false
     */
    dbool getAllDatas(dbyte *&pBuf, dint64 &uSize);

    /** 
     * �ж��ļ��Ƿ����
     * @param url �ļ�·��
     * @return ���ڷ���true�����򷵻�false
     */
    static dbool exists(const UrlString& url);

    /** 
     * ɾ���ļ�
     * @param url �ļ�·��
     * @return �ɹ�����true�����򷵻�false
     */
    static dbool remove(const UrlString& url);

    /** 
     * �������ļ�
     * @param url �ļ�·��
     * @param newUrl ���ļ���
     * @return �ɹ�����true�����򷵻�false
     */
    static dbool rename(const UrlString& url, const UrlString& newUrl);

    /** 
     * �����ļ���������
     * @param url �ļ�·��
     * @param hidden trueΪ���أ�falseΪ������
     * @return �ɹ�����true�����򷵻�false
     */
    static dbool setHidden(const UrlString& url, dbool hidden);
};
DM_END_NAMESPACE

#endif // DMFILE_H
