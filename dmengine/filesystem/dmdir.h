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
     * ���캯��
     * @param path Ŀ¼·��
     */
	Dir(const UrlString &path);

	/** 
     * ��������
     */
	~Dir();

	/** 
     * �ж�Ŀ¼�Ƿ����
     * @return ���ڷ���true�����򷵻�false
     */
	dbool exists() const;

	/** 
     * ɾ��Ŀ¼
     * @return �ɹ�����true�����򷵻�false
     */
	dbool remove();

	/** 
     * ������Ŀ¼
	 * @param newUrl ��Ŀ¼��
     * @return �ɹ�����true�����򷵻�false
     */
	dbool rename(const UrlString &newUrl);

	/** 
     * ����Ŀ¼
     * @return �ɹ�����true�����򷵻�false
     */
	dbool create();

	/** 
     * ���Ŀ¼·��
     * @return ·��
     */
	const UrlString& path() const;

    /** 
     * ���Ŀ¼�������ļ��б�
     * @param fileFilter �����ļ����ͣ�Ĭ�ϲ�����
     * @return �ļ��б�
     */
    FileInfoList fileInfoList(dint fileFilter = FileInfo::eUnknown) const;

    /** 
     * ɾ����·���µ������ļ�
     * @return �ɹ�����true�����򷵻�false
     */
    dbool removeAll();

	/** 
     * ���Ŀ¼�ָ���
     * @return �ָ���
     */
	static Char separator();

    /** 
     * ����ĩβ�ָ���
     * @return ӵ��ĩβ�ָ������ִ�
     */
	static UrlString& addSlashAtEnd(UrlString &url);

	/** 
     * �ж�һ��Ŀ¼�Ƿ����
     * @param path Ŀ¼·��
     * @return ���ڷ���true�����򷵻�false
     */
	static dbool exists(const UrlString &path);

	/** 
     * ����һ��Ŀ¼
     * @param dir Ŀ¼·��
     * @return �ɹ�����true�����򷵻�false
     */
	static dbool mkdir(const UrlString &dir);

	/** 
     * ɾ��һ��Ŀ¼
     * @param dir Ŀ¼·��
     * @return �ɹ�����true�����򷵻�false
     */
	static dbool rmdir(const UrlString &dir);

	/** 
     * �𼶴���Ŀ¼
     * @param path Ŀ¼·��
     * @return �ɹ�����true�����򷵻�false
     */
	static dbool mkpath(const UrlString &path);

	/** 
     * ��ɾ��Ŀ¼����Ŀ¼ͬʱɾ����
     * @param path Ŀ¼·��
     * @return �ɹ�����true�����򷵻�false
     */
	static dbool rmpath(const UrlString &path);

    /** 
     * ɾ����·���µ������ļ�
     * @param path Ŀ¼·��
     * @return �ɹ�����true�����򷵻�false
     */
    static dbool removeAll(const UrlString &path);
};

DM_END_NAMESPACE
#endif // DMDIR_H