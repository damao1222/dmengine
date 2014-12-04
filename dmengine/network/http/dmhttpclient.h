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

#include "dmsingleton.h"

DM_BEGIN_NAMESPACE
class HttpRequest;
DM_PRIVATE_CLASS(HttpClient);
class DM_DECL_EXPORT HttpClient : public Singleton<HttpClient>
{
	DM_DECLARE_PRIVATE(HttpClient);
public:
    enum{

    };

	HttpClient();
	~HttpClient();

    /**
     * ִ��http��get�������˺����̰߳�ȫ���ɶ��߳�ִ��
     * @param request ָ��get������HttpRequest��ָ��
     * @return �ɹ�����CURLE_OK������������ֵ
     */
	dint get(HttpRequest *request);

    /**
     * ִ��http��post�������˺����̰߳�ȫ���ɶ��߳�ִ��
     * @param request ָ��post������HttpRequest��ָ��
     * @return �ɹ�����CURLE_OK������������ֵ
     */
	dint post(HttpRequest *request);
};
DM_END_NAMESPACE
#define dmHttpClient DM_GLOBAL_OBJ(DM::HttpClient)
#endif // _DMHTTPCLIENT_H_