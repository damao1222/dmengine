/*
   Copyright (C) 2012-2014 Xiongfa Li
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

#include "dmhttp_test.h"
#include "http/dmhttpclient.h"
#include "http/dmhttprequest.h"

DM_BEGIN_NAMESPACE

dbool HttpTest::init()
{
   return true;
}

void HttpTest::cleanup()
{

}

void HttpTest::run()
{
	const char* path = "http://img.smart-tv.cn/images/group1/M00/06/87/OtwFbVR0NCOAQ9LjAAACz_9TNeY55.json";
    HttpRequest *request = new HttpRequest;
    request->setUrl(path);
    dmHttpClient.get(request);
    const char *data = request->getResponseData()->data();
    delete request;
}
DM_END_NAMESPACE

