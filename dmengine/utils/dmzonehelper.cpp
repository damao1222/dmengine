/*
Copyright (C) 2012 Xiongfa Li
All rights reserved.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of the author nor the names of contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "dmzonehelper.h"
#include "dmdebug.h"

DM_BEGIN_NAMESPACE
class ZoneHelperPrivate
{
public:
    ZoneHelperPrivate();

    dint mWidth;
    dint mHeight;

    dint mOffsetX;
    dint mOffsetY;

    dint mRow;
    dint mColumn;

    dint mZoneWidth;
    dint mZoneHeight;

    dint mZoneSize;
};

ZoneHelperPrivate::ZoneHelperPrivate():
    mWidth(0),
    mHeight(0),
    mOffsetX(0),
    mOffsetY(0),
    mRow(0),
    mColumn(0),
    mZoneWidth(0),
    mZoneHeight(0),
    mZoneSize(0)
{

}

ZoneHelper::ZoneHelper():
    C_D(ZoneHelper)
{
}

ZoneHelper::~ZoneHelper()
{
    D_D(ZoneHelper);
}

void ZoneHelper::setSize(dint nWidth, dint nHeigh)
{
    if (nWidth <= 0 || nHeigh <= 0)
        return ;

    if (pdm->mWidth != nWidth)
    {
        pdm->mWidth = nWidth;
    }

    if (pdm->mHeight != nHeigh)
    {
        pdm->mHeight = nHeigh;
    }
}

void ZoneHelper::setSize(const Size &size)
{
    pdm->mWidth = size.width();
    pdm->mHeight = size.height();
}

void ZoneHelper::getSize(dint &nWidth, dint &nHeigh) const
{
    nWidth = pdm->mWidth;
    nHeigh = pdm->mHeight;
}

Size ZoneHelper::getSize() const
{
    return Size(pdm->mWidth, pdm->mHeight);
}

void ZoneHelper::setOffset(dint nX, dint nY)
{
    if (pdm->mOffsetX != nX)
    {
        pdm->mOffsetX = nX;
    }

    if (pdm->mOffsetY != nY)
    {
        pdm->mOffsetY = nY;
    }
}

void ZoneHelper::getOffset(dint &nX, dint &nY)
{
    nX = pdm->mOffsetX;
    nY = pdm->mOffsetY;
}

void ZoneHelper::setZoneSize(dint nWidth, dint nHeigh)
{
    if (nWidth <= 0 || nHeigh <= 0)
        return ;

    if (pdm->mZoneWidth != nWidth)
    {
        pdm->mZoneWidth = nWidth;

        if (pdm->mWidth > 0)
        {
            pdm->mColumn = pdm->mWidth / pdm->mZoneWidth;
        }
    }

    if (pdm->mZoneHeight != nHeigh)
    {
        pdm->mZoneHeight = nHeigh;
        if (pdm->mHeight > 0)
        {
            pdm->mRow = pdm->mHeight / pdm->mZoneHeight;
        }
    }
}

void ZoneHelper::setZoneSize(const Size &size)
{
    setZoneSize(size.width(), size.height());
}

void ZoneHelper::getZoneSize(dint &nWidth, dint &nHeigh) const
{
    nWidth = pdm->mZoneWidth;
    nHeigh = pdm->mZoneHeight;
}

Size ZoneHelper::getZoneSize() const
{
    return Size(pdm->mZoneWidth, pdm->mZoneHeight);
}

void ZoneHelper::setRow(dint nRow)
{
    if (nRow <= 0)
        return ;

    if (pdm->mRow != nRow)
    {
        pdm->mRow = nRow;
    }

    if (pdm->mHeight == 0)
    {
        DMDEBUG("height need be set at first!");
        return ;
    }

    pdm->mZoneHeight = pdm->mHeight / pdm->mRow;

    if (pdm->mColumn != 0)
    {
        pdm->mZoneSize = pdm->mRow * pdm->mColumn;
    }
}

void ZoneHelper::setColumn(dint nColumn)
{
    if (nColumn <= 0)
        return ;

    if (pdm->mColumn != nColumn)
    {
        pdm->mColumn = nColumn;
    }

    if (pdm->mWidth == 0)
    {
        DMDEBUG("width need be set at first!");
        return ;
    }

    pdm->mZoneWidth = pdm->mWidth / pdm->mColumn;

    if (pdm->mRow != 0)
    {
        pdm->mZoneSize = pdm->mRow * pdm->mColumn;
    }
}

dint ZoneHelper::getCount() const
{
    return pdm->mZoneSize;
}

Rect ZoneHelper::hitTestRect(dint nX, dint nY) const
{
    if (pdm->mZoneWidth <= 0 || pdm->mZoneHeight <= 0)
    {
        DMDEBUG("Zone size is 0, call setZoneSize first");
        return Rect();
    }

    dint x = ((nX-pdm->mOffsetX) / pdm->mZoneWidth)*pdm->mZoneWidth;
    dint y = ((nY-pdm->mOffsetY) / pdm->mZoneHeight)*pdm->mZoneHeight;

    return Rect(x+pdm->mOffsetX, y+pdm->mOffsetY, pdm->mZoneWidth, pdm->mZoneHeight);
}

dint ZoneHelper::hitTestIndex(dint nX, dint nY) const
{
    if (pdm->mZoneWidth <= 0 || pdm->mZoneHeight <= 0)
    {
        DMDEBUG("Zone size is 0, call setZoneSize first");
        return -1;
    }

    dint column = (nX-pdm->mOffsetX) / pdm->mZoneWidth;
    dint row = (nY-pdm->mOffsetY) / pdm->mZoneHeight;

    return (row*pdm->mColumn+column);
}

Rect ZoneHelper::fromIndex(dint index) const
{
    if (pdm->mZoneWidth <= 0 || pdm->mZoneHeight <= 0)
    {
        DMDEBUG("Zone size is 0, call setZoneSize first");
        return Rect();
    }

    dint row = index / pdm->mColumn;
    dint column = index % pdm->mColumn;

    return Rect(column*pdm->mZoneWidth+pdm->mOffsetX, \
                row*pdm->mZoneHeight+pdm->mOffsetY, \
                pdm->mZoneWidth, pdm->mZoneHeight);
}

dbool ZoneHelper::isValid() const
{
    return (pdm->mWidth>0&&pdm->mHeight>0&&pdm->mZoneWidth>0&&pdm->mZoneHeight>0);
    //return pdm->mZoneSize > 0;
}

ZoneHelper &ZoneHelper::operator=(const ZoneHelper &other)
{
    this->pdm->mWidth = other.pdm->mWidth;
    this->pdm->mHeight = other.pdm->mHeight;
    this->pdm->mOffsetX = other.pdm->mOffsetX;
    this->pdm->mOffsetY = other.pdm->mOffsetY;
    this->pdm->mRow = other.pdm->mRow;
    this->pdm->mColumn = other.pdm->mColumn;
    this->pdm->mZoneWidth = other.pdm->mZoneWidth;
    this->pdm->mZoneHeight = other.pdm->mZoneHeight;
    this->pdm->mZoneSize = other.pdm->mZoneSize;

    return (*this);
}

DM_END_NAMESPACE