/*
Copyright (C) 2012 Xiongfa Li
All rights reserved.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of the author nor the names of contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef DMZONEHELPER_H
#define DMZONEHELPER_H

#include "dmnamespace.h"
#include "dmrect.h"
#include "dmsize.h"

DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(ZoneHelper);
class DM_DLL_EXPORT ZoneHelper
{
    DM_DECLARE_PRIVATE(ZoneHelper)
public:
    ZoneHelper();
    ~ZoneHelper();

    void setSize(dint nWidth, dint nHeigh);
    void setSize(const Size &size);
    void getSize(dint &nWidth, dint &nHeigh) const;
    Size getSize() const;
    void setOffset(dint nX, dint nY);
    void getOffset(dint &nX, dint &nY);

    void setZoneSize(dint nWidth, dint nHeigh);
    void setZoneSize(const Size &size);
    void getZoneSize(dint &nWidth, dint &nHeigh) const;
    Size getZoneSize() const;

    void setRow(dint nRow);
    void setColumn(dint nColumn);

    dint getCount() const;

    Rect hitTestRect(dint nX, dint nY) const;
    dint hitTestIndex(dint nX, dint nY) const;
    Rect fromIndex(dint index) const;

    dbool isValid() const;
    ZoneHelper &operator=(const ZoneHelper &other);
};
DM_END_NAMESPACE
#endif // DMZONEHELPER_H
