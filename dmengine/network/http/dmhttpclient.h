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

#ifndef  _DMHTTPCLIENT_H_
#define  _DMHTTPCLIENT_H_

#include "dmnamespace.h"

DM_BEGIN_NAMESPACE
class HttpRequest;
DM_PRIVATE_CLASS(HttpClient);
class DM_DECL_EXPORT HttpClient
{
	DM_DECLARE_PRIVATE(HttpClient);
public:
    enum{

    };

    /**
     * 执行http的get操作。此函数线程安全，可多线程执行
     * @param request 指向get请求类HttpRequest的指针
     * @return 成功返回CURLE_OK，否则是其他值
     */
	dint get(HttpRequest *request);

    /**
     * 执行http的post操作。此函数线程安全，可多线程执行
     * @param request 指向post请求类HttpRequest的指针
     * @return 成功返回CURLE_OK，否则是其他值
     */
	dint post(HttpRequest *request);

    /** 
     * 获得HttpClient的单例，如果单例不存在则new一个HttpClient对象（线程安全）.
     * @return 返回HttpClient单例.
     */
    static HttpClient* getInstance();

    /** 
     * 获得HttpClient的单例，如果单例存在返回单例指针，否则返回NULL.
     * @return 返回HttpClient单例.
     */
    static HttpClient* instance();

    /** 
     * 释放HttpClient的单例（线程安全）.
     */
    static void releaseInstance();

private:
    HttpClient();
    ~HttpClient();
};
DM_END_NAMESPACE
#define dmHttpClient DM_GLOBAL_OBJ(DM_NS::HttpClient)
#endif // _DMHTTPCLIENT_H_