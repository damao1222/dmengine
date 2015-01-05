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
#include "dmgraphicsmath.h"
#include "dmpoint.h"
#include "dmline.h"
#include "dmrect.h"
#include "dmmath.h"

DM_USING_NAMESPACE;


// 判断点在有向直线的左侧还是右侧.
// 返回值:-1: 点在线段左侧; 0: 点在线段上; 1: 点在线段右侧
int GraphicsMath::pointAtLineLeftRight(const Point &ptStart, const Point &ptEnd, const Point &ptTest)
{
    int sx = ptStart.x();
    int sy = ptStart.y();
    int ex = ptEnd.x();
    int ey = ptEnd.y();
    sx -= ptTest.x();
    sy -= ptTest.y();
    ex -= ptTest.x();
    ey -= ptTest.y();

    int nRet = sx * ey - sy * ex;
    if (nRet == 0)
        return 0;
    else if (nRet > 0)
        return 1;
    else if (nRet < 0)
        return -1;

    return 0;
}

// 判断点在有向直线的左侧还是右侧.
// 返回值:-1: 点在线段左侧; 0: 点在线段上; 1: 点在线段右侧
int GraphicsMath::pointAtLineLeftRight(const Line &line, const Point &ptTest)
{
    return pointAtLineLeftRight(line.start(), line.end(), ptTest);
}

// 判断两条线段是否相交
dbool GraphicsMath::isTwoLineIntersect(const Point &ptLine1Start, const Point &ptLine1End, const Point &ptLine2Start, const Point &ptLine2End)
{
    int nLine1Start = pointAtLineLeftRight(ptLine2Start, ptLine2End, ptLine1Start);
    int nLine1End = pointAtLineLeftRight(ptLine2Start, ptLine2End, ptLine1End);
    if (nLine1Start * nLine1End > 0)
        return false;

    int nLine2Start = pointAtLineLeftRight(ptLine1Start, ptLine1End, ptLine2Start);
    int nLine2End = pointAtLineLeftRight(ptLine1Start, ptLine1End, ptLine2End);

    if (nLine2Start * nLine2End > 0)
        return false;

    return true;
}

// 判断两条线段是否相交
dbool GraphicsMath::isTwoLineIntersect(const Line &l1, const Line &l2)
{
    return isTwoLineIntersect(l1.start(), l1.end(), l2.start(), l2.end());
}

// 判断线段是否与矩形相交
dbool GraphicsMath::isLineIntersectRect(const Point &ptStart, const Point &ptEnd, const Rect &rect)
{
    if (rect.contains(ptStart) || rect.contains(ptEnd))
    {
        return true;
    }

    // Two point both aren't in rect
    if (isTwoLineIntersect(ptStart, ptEnd, rect.topLeft(), rect.bottomLeft()))
        return true;
    if (isTwoLineIntersect(ptStart, ptEnd, rect.bottomLeft(), rect.bottomRight()))
        return true;
    if (isTwoLineIntersect(ptStart, ptEnd, rect.bottomRight(), rect.topRight()))
        return true;
    if (isTwoLineIntersect(ptStart, ptEnd, rect.topLeft(), rect.topRight()))
        return true;

    return false;
}

dbool GraphicsMath::isLineIntersectRect(const Line &line, const Rect &rect)
{
    return isLineIntersectRect(line.start(), line.end(), rect);
}

PointF GraphicsMath::rotateByAngle(const PointF &v, const PointF &pivot, dreal angle, dbool inDegree /*= true*/)
{
    if (inDegree)
        angle = degree2radian(angle);
    PointF r = v - pivot;
    dreal cosa = dmCos(angle), sina = dmSin(angle);
    dreal x = r.x()*cosa - r.y()*sina + pivot.x();
    dreal y = r.x()*sina + r.y()*cosa + pivot.y();
    return PointF(x, y);
}

dint GraphicsMath::guessQuadrant(const PointF &p0, const PointF &p1)
{
#if 1
        if (p0.x() < p1.x() && p0.y() < p1.y())
            return 0;
        else if (p0.x() < p1.x() && p0.y() > p1.y())
            return 3;
        else if (p0.x() > p1.x() && p0.y() < p1.y())
            return 1;
        else if (p0.x() > p1.x() && p0.y() > p1.y())
            return 2;
        else
            return 4;
#else
    {
        dreal radian = Line::radian(p0, p1);
        if (radian < DM_PI/2 && radian > 0)
        {
            return 0;
        }
        else if (radian < DM_PI && radian > DM_PI/2)
        {
            return 1;
        }
        else if (radian < -DM_PI/2 && radian > -DM_PI)
        {
            return 2;
        }
        else if (radian < 0 && radian > -DM_PI/2)
        {
            return 3;
        }
        else
        {
            return 4;
        }
    }
#endif
}

PointF GraphicsMath::pointAtRect(const Point &p0, const Point &p1, const Rect &rect, dbool &success)
{
    PointF p;
    if (p0 == p1)
    {
        success = false;
        return p;
    }

    success = true;
    if (p0.x() == p1.x())
    {
        if (p0.y() < p1.y())
            return PointF(p0.x(), rect.bottom());
        else
            return PointF(p0.x(), rect.top());
    }
    else if (p0.y() == p1.y())
    {
        if (p0.x() < p1.x())
            return PointF(rect.right(), p0.y());
        else
            return PointF(rect.left(), p0.y());
    }
    else
    {
        dreal a, b, c;
        Line(p0, p1).getMathLine(a, b, c);
        dint quadrant = guessQuadrant(p0, p1);
        switch (quadrant)
        {
            case 0:
            {
                dreal x = rect.right();
                dreal y = -(a*x+c) / b;
                if (y <= rect.bottom())
                {
                    return PointF(x, y);
                }
                else
                {
                    y = rect.bottom();
                    x = -(b*y+c)/a;
                    return PointF(x, y);
                }
                break;
            }
            case 1:
            {
                dreal x = rect.left();
                dreal y = -(a*x+c) / b;
                if (y <= rect.bottom())
                {
                    return PointF(x, y);
                }
                else
                {
                    y = rect.bottom();
                    x = -(b*y+c)/a;
                    return PointF(x, y);
                }
                break;
            }
            case 2:
            {
                dreal x = rect.left();
                dreal y = -(a*x+c) / b;
                if (y >= rect.top())
                {
                    return PointF(x, y);
                }
                else
                {
                    y = rect.top();
                    x = -(b*y+c)/a;
                    return PointF(x, y);
                }
                break;
            }
            case 3:
            {
                dreal x = rect.right();
                dreal y = -(a*x+c) / b;
                if (y >= rect.top())
                {
                    return PointF(x, y);
                }
                else
                {
                    y = rect.top();
                    x = -(b*y+c)/a;
                    return PointF(x, y);
                }
                break;
            }
            default:
            {
                success = false;
                return p;
            }
        }
    }
}

PointF GraphicsMath::pointAtRect(const PointF &p0, const PointF &p1, const RectF &rect, dbool &success)
{
    PointF p;
    if (p0 == p1)
    {
        success = false;
        return p;
    }

    success = true;
    if (IsEqual(p0.x(), p1.x()))
    {
        if (p0.y() < p1.y())
            return PointF(p0.x(), rect.bottom());
        else
            return PointF(p0.x(), rect.top());
    }
    else if (IsEqual(p0.y(), p1.y()))
    {
        if (p0.x() < p1.x())
            return PointF(rect.right(), p0.y());
        else
            return PointF(rect.left(), p0.y());
    }
    else
    {
        dreal a, b, c;
        LineF(p0, p1).getMathLine(a, b, c);
        dint quadrant = guessQuadrant(p0, p1);
        switch (quadrant)
        {
            case 0:
            {
                dreal x = rect.right();
                dreal y = -(a*x+c) / b;
                if (y <= rect.bottom())
                {
                    return PointF(x, y);
                }
                else
                {
                    y = rect.bottom();
                    x = -(b*y+c)/a;
                    return PointF(x, y);
                }
                break;
            }
            case 1:
            {
                dreal x = rect.left();
                dreal y = -(a*x+c) / b;
                if (y <= rect.bottom())
                {
                    return PointF(x, y);
                }
                else
                {
                    y = rect.bottom();
                    x = -(b*y+c)/a;
                    return PointF(x, y);
                }
                break;
            }
            case 2:
            {
                dreal x = rect.left();
                dreal y = -(a*x+c) / b;
                if (y >= rect.top())
                {
                    return PointF(x, y);
                }
                else
                {
                    y = rect.top();
                    x = -(b*y+c)/a;
                    return PointF(x, y);
                }
                break;
            }
            case 3:
            {
                dreal x = rect.right();
                dreal y = -(a*x+c) / b;
                if (y >= rect.top())
                {
                    return PointF(x, y);
                }
                else
                {
                    y = rect.top();
                    x = -(b*y+c)/a;
                    return PointF(x, y);
                }
                break;
            }
            default:
            {
                success = false;
                return p;
            }
        }
    }
}

dbool GraphicsMath::lineIntersect(const Point& A, const Point& B, const Point& C, const Point& D)
{
    dreal S, T;

    if( mathlineIntersect(A, B, C, D, &S, &T )
        && (S >= 0.0f && S <= 1.0f && T >= 0.0f && T <= 1.0f) )
        return true;

    return false;
}

Point GraphicsMath::intersectPoint(const Point& A, const Point& B, const Point& C, const Point& D)
{
    dreal S, T;

    if( mathlineIntersect(A, B, C, D, &S, &T) )
    {
        dreal x = A.x() + S * (B.x() - A.x());
        dreal y = A.y() + S * (B.y() - A.y());
        // Point of intersection
        Point p(x, y);
        return p;
    }

    return Point();
}

dbool GraphicsMath::mathlineIntersect(const Point& A, const Point& B,
                      const Point& C, const Point& D,
                      dreal *S, dreal *T)
{
    // FAIL: Line undefined
    if ( (A.x() == B.x() && A.y() == B.y()) || (C.x() == D.x() && C.y() == D.y()) )
    {
        return false;
    }
    const dint BAx = B.x() - A.x();
    const dint BAy = B.y() - A.y();
    const dint DCx = D.x() - C.x();
    const dint DCy = D.y() - C.y();
    const dint ACx = A.x() - C.x();
    const dint ACy = A.y() - C.y();

    const dint denom = DCy*BAx - DCx*BAy;

    dint s = DCx*ACy - DCy*ACx;
    dint t = BAx*ACy - BAy*ACx;

    if (denom == 0)
    {
        if (*S == 0 || *T == 0)
        { 
            // Lines incident
            return true;   
        }
        // Lines parallel and not incident
        return false;
    }

    *S = static_cast<dreal>(s) / denom;
    *T = static_cast<dreal>(t) / denom;

    return true;
}

dbool GraphicsMath::lineIntersectRect(const Point& A, const Point& B, const Rect &R)
{
    if (R.contains(A) || R.contains(B))
        return true;

    if (lineIntersect(A, B, R.topLeft(), R.topRight()))
        return true;
    else if (lineIntersect(A, B, R.topLeft(), R.bottomLeft()))
        return true;
    else if (lineIntersect(A, B, R.bottomLeft(), R.bottomRight()))
        return true;
    else if (lineIntersect(A, B, R.topRight(), R.bottomRight()))
        return true;

    return false;
}

dbool GraphicsMath::lineIntersectRect(const Line &L, const Rect &R)
{
    return lineIntersectRect(L.start(), L.end(), R);
}

dbool GraphicsMath::lineIntersectF(const PointF& A, const PointF& B, const PointF& C, const PointF& D)
{
    dreal S, T;

    if( mathlineIntersectF(A, B, C, D, &S, &T )
        && (S >= 0.0f && S <= 1.0f && T >= 0.0f && T <= 1.0f) )
        return true;

    return false;
}

PointF GraphicsMath::intersectPointF(const PointF& A, const PointF& B, const PointF& C, const PointF& D)
{
    dreal S, T;

    if( mathlineIntersectF(A, B, C, D, &S, &T) )
    {
        dreal x = A.x() + S * (B.x() - A.x());
        dreal y = A.y() + S * (B.y() - A.y());
        // Point of intersection
        PointF p(x, y);
        return p;
    }

    return PointF();
}

dbool GraphicsMath::mathlineIntersectF(const PointF& A, const PointF& B,
                      const PointF& C, const PointF& D,
                      dreal *S, dreal *T)
{
    // FAIL: Line undefined
    if ( (A.x() == B.x() && A.y() == B.y()) || (C.x() == D.x() && C.y() == D.y()) )
    {
        return false;
    }
    const dreal BAx = B.x() - A.x();
    const dreal BAy = B.y() - A.y();
    const dreal DCx = D.x() - C.x();
    const dreal DCy = D.y() - C.y();
    const dreal ACx = A.x() - C.x();
    const dreal ACy = A.y() - C.y();

    const dreal denom = DCy*BAx - DCx*BAy;

    *S = DCx*ACy - DCy*ACx;
    *T = BAx*ACy - BAy*ACx;

    if (denom == 0)
    {
        if (*S == 0 || *T == 0)
        {
            // Lines incident
            return true;
        }
        // Lines parallel and not incident
        return false;
    }

    *S = *S / denom;
    *T = *T / denom;

    return true;
}

dbool GraphicsMath::lineIntersectRectF(const PointF& A, const PointF& B, const RectF &R)
{
    if (R.contains(A) || R.contains(B))
        return true;

    if (lineIntersectF(A, B, R.topLeft(), R.topRight()))
        return true;
    else if (lineIntersectF(A, B, R.topLeft(), R.bottomLeft()))
        return true;
    else if (lineIntersectF(A, B, R.bottomLeft(), R.bottomRight()))
        return true;
    else if (lineIntersectF(A, B, R.topRight(), R.bottomRight()))
        return true;

    return false;
}

dbool GraphicsMath::lineIntersectRectF(const LineF &L, const RectF &R)
{
    return lineIntersectRectF(L.start(), L.end(), R);
}



