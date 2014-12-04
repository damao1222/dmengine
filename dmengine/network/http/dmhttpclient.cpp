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
#include "dmhttpclient.h"
#include "curl/dmcurl.h"
#include "dmhttprequest.h"

DM_BEGIN_NAMESPACE

// Callback function used by libcurl for collect response data
static size_t WriteData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;

    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);

    return sizes;
}

class HttpClientPrivate
{
public:
    dint config(HttpRequest *request, CURL* curl);
};

dint HttpClientPrivate::config(HttpRequest *request, CURL* curl)
{
    int code = 0;

    /// Keeps custom header data
    curl_slist *m_headers = NULL;

    code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, request->getTimeoutForRead());
    if (code != CURLE_OK) {
        return code;
    }
    code = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, request->getTimeoutForConnect());
    if (code != CURLE_OK) {
        return code;
    }
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    /* get custom header data (if set) */
    const std::vector<std::string>& headers=request->getHeaders();
    if(!headers.empty())
    {
        /* append custom headers one by one */
        for (std::vector<std::string>::const_iterator it = headers.cbegin(); it != headers.cend(); ++it)
            m_headers = curl_slist_append(m_headers,it->c_str());
        /* set custom headers for curl */
        code = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, m_headers);

        /* free the linked list for header data */
        if (m_headers)
            curl_slist_free_all(m_headers);
        if (code != CURLE_OK)
            return code;
    }

    code = curl_easy_setopt(curl, CURLOPT_URL, request->getUrl().c_str());
    if (code != CURLE_OK)
        return code;

    code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
    if (code != CURLE_OK)
        return code;

    code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, request->getResponseData());
    if (code != CURLE_OK)
        return code;

    HttpProgressCallback progressFunc = request->getProgressCallback();
    if(progressFunc)
    {
        code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        if (code != CURLE_OK)
            return code;

        code = curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progressFunc);
        if (code != CURLE_OK)
            return code;
    }

    void *clientP = request->getGetProgressClientp();
    if(clientP)
    {
        code = curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, clientP);
        if (code != CURLE_OK)
            return code;
    }

    return CURLE_OK;
}

HttpClient::HttpClient()
	:C_D(HttpClient)
{
    curl_global_init(CURL_GLOBAL_ALL);
}

HttpClient::~HttpClient()
{
    curl_global_cleanup();
	D_D(HttpClient);
}

dint HttpClient::get(HttpRequest *request)
{
    int returnCode = 0;
    int responseCode = 0;
    CURL* curl = curl_easy_init();
    if(!curl)
    {
        return CURLE_FAILED_INIT;
    }

    returnCode = pdm->config(request, curl);
    if(CURLE_OK != returnCode)
    {
        if (curl)
            curl_easy_cleanup(curl);
        return returnCode;
    }

    returnCode = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
    if (returnCode != CURLE_OK)
    {
        if (curl)
            curl_easy_cleanup(curl);
        return returnCode;
    }

    returnCode = curl_easy_perform(curl);
    if (returnCode != CURLE_OK)
    {
        if (curl)
            curl_easy_cleanup(curl);
        return returnCode;
    }

    returnCode = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
    if (returnCode != CURLE_OK || responseCode != 200)
    {
        if (curl)
            curl_easy_cleanup(curl);
        return -1;
    }

    if (curl)
        curl_easy_cleanup(curl);
    return CURLE_OK;
}

dint HttpClient::post(HttpRequest *request)
{
    int returnCode = 0;
    int responseCode = 0;
    CURL* curl = curl_easy_init();
    if(!curl)
    {
        return CURLE_FAILED_INIT;
    }

    returnCode = pdm->config(request, curl);
    if(CURLE_OK != returnCode)
    {
        if (curl)
            curl_easy_cleanup(curl);
        return returnCode;
    }

    returnCode = curl_easy_setopt(curl, CURLOPT_POST, 1);
    if (returnCode != CURLE_OK)
    {
        if (curl)
            curl_easy_cleanup(curl);
        return returnCode;
    }

    returnCode = curl_easy_setopt(curl, CURLOPT_POST, request->getRequestData());
    if (returnCode != CURLE_OK)
    {
        if (curl)
            curl_easy_cleanup(curl);
        return returnCode;
    }

    returnCode = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request->getRequestDataSize());
    if (returnCode != CURLE_OK)
    {
        if (curl)
            curl_easy_cleanup(curl);
        return returnCode;
    }

    returnCode = curl_easy_perform(curl);
    if (returnCode != CURLE_OK)
    {
        if (curl)
            curl_easy_cleanup(curl);
        return returnCode;
    }

    returnCode = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
    if (returnCode != CURLE_OK || responseCode != 200)
    {
        if (curl)
            curl_easy_cleanup(curl);
        return -1;
    }

    if (curl)
        curl_easy_cleanup(curl);
    return CURLE_OK;
}

DM_END_NAMESPACE