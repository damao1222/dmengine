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

#include "dmjson_test.h"
#include "fileformat/json/dmjsonreader.h"
#include "fileformat/json/dmjsonwriter.h"

DM_BEGIN_NAMESPACE

dbool JsonTest::init()
{
   return true;
}

void JsonTest::cleanup()
{

}

void JsonTest::run()
{
    JsonReader reader;
    if (reader.parse("./test_json.txt"))
    {
        //xxx
        JsonWriter writer;
        //JsonWriter::toString(reader.root());
        writer.write("./test_json_new.txt", reader.root());
    }
}
DM_END_NAMESPACE

