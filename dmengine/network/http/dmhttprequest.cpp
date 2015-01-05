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
#include "dmhttprequest.h"
DM_BEGIN_NAMESPACE
class HttpRequestPrivate
{
public:
    std::string url; 
    std::vector<char> requestData; 
    std::vector<std::string> headers;
    dint timeoutForConnect;
    dint timeoutForRead;	
    HttpProgressCallback progressCallback;
    void *progressClientp;
    std::vector<char> responseData;
};

HttpRequest::HttpRequest()
	:C_D(HttpRequest)
{
    pdm->url.clear();
    pdm->requestData.clear();
    pdm->responseData.clear();
    pdm->timeoutForConnect = 30;
    pdm->timeoutForRead = 60;
    pdm->progressCallback = NULL;
    pdm->progressClientp = NULL;
}

HttpRequest::~HttpRequest()
{
	D_D(HttpRequest);
}

void HttpRequest::setUrl(const char* url)
{
    pdm->url = url;
}

 const std::string& HttpRequest::getUrl() const
 {
     return pdm->url;
 }

void HttpRequest::setRequestData(const char* buffer, duint len)
{
    pdm->requestData.assign(buffer, buffer + len);
}

char* HttpRequest::getRequestData() const
{
    return &(pdm->requestData.front());
}

dint HttpRequest::getRequestDataSize() const
{
    return pdm->requestData.size();
}

void HttpRequest::setHeaders(const std::vector<std::string> &pHeaders)
{
    pdm->headers = pHeaders;
}

const std::vector<std::string>& HttpRequest::getHeaders() const
{
    return pdm->headers;
}

void HttpRequest::setResponseData(std::vector<char>* data)
{
    pdm->responseData = *data;
}

std::vector<char>* HttpRequest::getResponseData()
{
    return &pdm->responseData;
}

void HttpRequest::setTimeoutForConnect(dint value)
{
    pdm->timeoutForConnect = value;
}

dint HttpRequest::getTimeoutForConnect() const
{
    return pdm->timeoutForConnect;
}

void HttpRequest::setTimeoutForRead(dint value)
{
    pdm->timeoutForRead = value;
}

dint HttpRequest::getTimeoutForRead() const
{
    return pdm->timeoutForRead;
}

void HttpRequest::setProgressCallback(HttpProgressCallback callback)
{
    pdm->progressCallback = callback;
}

HttpProgressCallback HttpRequest::getProgressCallback() const
{
    return pdm->progressCallback;
}

void HttpRequest::setGetProgressClientp(void *clientP)
{
    pdm->progressClientp = clientP;
}   

void *HttpRequest::getGetProgressClientp() const
{
    return pdm->progressClientp;
}
DM_END_NAMESPACE