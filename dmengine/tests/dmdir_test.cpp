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

#include "dmdir_test.h"
#include "dmdir.h"

DM_BEGIN_NAMESPACE

dbool DirTest::init()
{
   return true;
}

void DirTest::cleanup()
{

}

void DirTest::run()
{
	UrlString path = "d:/test/hahah/1234/5676/test";
    bool ret = Dir::mkpath(path);
	ret = Dir::exists(path);
	UrlString newPath = path + UrlString("/test");
	ret = Dir::mkdir(newPath);
	ret = Dir::rmdir(path);
    ret = Dir::removeAll(path);
}
DM_END_NAMESPACE

