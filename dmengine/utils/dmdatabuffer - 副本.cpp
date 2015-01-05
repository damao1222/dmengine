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

#include "dmbytebuffer.h"
#if 0
DM_BEGIN_NAMESPACE
class ByteBufferPrivate
{
public:
    ByteBufferPrivate(): data(NULL), size(0) {}
    unsigned char *data;
    dint size;
};

ByteBuffer::ByteBuffer():
    C_D(ByteBuffer)
{
}

ByteBuffer::ByteBuffer(unsigned char *data, dint size):
    C_D(ByteBuffer)
{
    pdm->data = data;
    pdm->size = size;
}

ByteBuffer::~ByteBuffer()
{
    clear();
    D_D(ByteBuffer);
}

const char& ByteBuffer::at(dint index) const
{
    return pdm->data[index];
}

char ByteBuffer::at(dint index)
{
    return pdm->data[index];
}

duint32 ByteBuffer::length() const
{
    return pdm->size;
}
    
duint32 ByteBuffer::size() const
{
    return pdm->size;
}

unsigned char* ByteBuffer::data() const
{
    return pdm->data;
}

dbool ByteBuffer::isEmpty() const
{
    return pdm->size == 0;
}

void ByteBuffer::clear()
{
    DM_SAFE_DELETE_ARRAY(pdm->data);
    pdm->size = 0;
}

ByteBuffer& ByteBuffer::operator=(const ByteBuffer &other)
{
    //Not Support!
    DM_ASSERT_X(false, "ByteBuffer::operator= Not Support");
    return *this;
}

bool ByteBuffer::operator==(unsigned char *data) const
{
    return pdm->data == data;
}

bool ByteBuffer::operator==(const ByteBuffer &other) const
{
    return pdm->data == other.pdm->data;
}

bool ByteBuffer::operator!=(unsigned char *data) const
{
    return pdm->data != data;
}

bool ByteBuffer::operator!=(const ByteBuffer &other) const
{
    return pdm->data != other.pdm->data;
}
DM_END_NAMESPACE
#endif