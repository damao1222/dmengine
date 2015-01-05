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

#include "dmifile.h"

DM_BEGIN_NAMESPACE

IFile::IFile()
{
}

IFile::~IFile()
{
}

int IFile::stat(struct __stat64* buffer)
{
    memset(buffer, 0, sizeof(struct __stat64));
    errno = ENOENT;
    return -1;
}

dbool IFile::readString(char *szLine, dint iLineLength)
{
    if(seek(0, SEEK_CUR) < 0) return false;

    dint64 iFilePos = getPosition();
    dint iBytesRead = read( (unsigned char*)szLine, iLineLength - 1);
    if (iBytesRead <= 0)
    return false;

    szLine[iBytesRead] = 0;

    for (int i = 0; i < iBytesRead; i++)
    {
        if ('\n' == szLine[i])
        {
            if ('\r' == szLine[i + 1])
            {
                szLine[i + 1] = 0;
                seek(iFilePos + i + 2, SEEK_SET);
            }
            else
            {
                // end of line
                szLine[i + 1] = 0;
                seek(iFilePos + i + 1, SEEK_SET);
            }
            break;
        }
        else if ('\r' == szLine[i])
        {
            if ('\n' == szLine[i + 1])
            {
                szLine[i + 1] = 0;
                seek(iFilePos + i + 2, SEEK_SET);
            }
            else
            {
                // end of line
                szLine[i + 1] = 0;
                seek(iFilePos + i + 1, SEEK_SET);
            }
            break;
        }
    }
    return true;
}
DM_END_NAMESPACE

