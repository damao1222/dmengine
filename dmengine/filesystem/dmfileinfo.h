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
     * ���캯��
     * @param path Ŀ¼·��
     */
	FileInfo(const UrlString &path, EFileType type);

    /** 
     * �������캯��
     * @param other ��������
     */
    FileInfo(const FileInfo &other);

	/** 
     * ��������
     */
	~FileInfo();

    /** 
     * �ж��Ƿ�ΪĿ¼
     * @return ��Ŀ¼����true�����򷵻�false
     */
    dbool isDir() const;

    /** 
     * ����ļ�·��
     * @return ·��
     */
    const UrlString& getPath() const;

    /** 
     * ��ֵ���������
     * @param other ��ֵ����
     * @return ��ֵ��ǰ���������
     */
    FileInfo& operator=(const FileInfo &other);

    /** 
     * �������������
     * @param other �Ƚ϶���
     * @return ��ȷ���true�����򷵻�false
     */
    dbool operator==(const FileInfo &other) const;

     /** 
     * �������������
     * @param other �Ƚ϶���
     * @return ����ȷ���true�����򷵻�false
     */
    dbool operator!=(const FileInfo &other) const;
};

typedef Vector<FileInfo> FileInfoList;

DM_END_NAMESPACE
#endif // DMFILEINFO_H