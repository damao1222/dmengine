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

#ifndef  _DMHTTPREQUEST_H_
#define  _DMHTTPREQUEST_H_

#include "dmsingleton.h"
#include <string>
#include <vector>

DM_BEGIN_NAMESPACE

/** http get进度回调函数指针
*	@param clientp		传入的用户指针
*	@param dltotal		需要下载的总量
*	@param dlnow		已完成下载的量
*	@param ultotal		需要上传的总量
*	@param ulnow		已完成上传的量
*/
typedef dint (*HttpProgressCallback)(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

DM_PRIVATE_CLASS(HttpRequest);
class DM_DECL_EXPORT HttpRequest
{
	DM_DECLARE_PRIVATE(HttpRequest);
public:
	HttpRequest();
	~HttpRequest();

     /** Required field for HttpRequest object before being sent.
     */
    void setUrl(const char* url);

    /** Get back the setted url */
    const std::string& getUrl() const;
    
    /** Option field. You can set your post data here
     */
    void setRequestData(const char* buffer, duint len);

    /** Get the request data pointer back */
    char* getRequestData() const;

    /** Get the size of request data back */
    dint getRequestDataSize() const;

	/** Set any custom headers **/
    void setHeaders(const std::vector<std::string> &pHeaders);
   
    /** Get custom headers **/
   	const std::vector<std::string>& getHeaders()  const;

	/** Set the http response raw buffer, is used by CCHttpClient */
    void setResponseData(std::vector<char>* data);

	/** Get the http response raw data */
    std::vector<char>* getResponseData();

    /**
     * Change the connect timeout
     * @param timeout 
     * @return NULL
     */
    void setTimeoutForConnect(dint value);
    
    /**
     * Get connect timeout
     * @return dint
     *
     */
    dint getTimeoutForConnect() const;
    
    
    /**
     * Change the download timeout
     * @param value
     * @return NULL
     */
    void setTimeoutForRead(dint value);
    

    /**
     * Get download timeout
     * @return dint
     */
    dint getTimeoutForRead() const;

    /**
     * Set the process callback function
     * @param callback function pointer
     * @return NULL
     */
    void setProgressCallback(HttpProgressCallback callback);

    /**
     * Get the process callback function pointer
     * @return callback function pointer
     */
    HttpProgressCallback getProgressCallback() const;

    /**
     * Set the get progress clientp
     * @param clientP get progress clientp
     * @return NULL
     */
    void setGetProgressClientp(void *clientP);
    

    /**
     * Get the get progress clientp
     * @return get progress clientp
     */
    void *getGetProgressClientp() const;
};

DM_END_NAMESPACE
#endif // _DMHTTPREQUEST_H_