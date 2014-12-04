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
#ifndef DMGRAPHICSMATH_H
#define DMGRAPHICSMATH_H
#include "dmnamespace.h"

DM_BEGIN_NAMESPACE
class Point;
class PointF;
class Line;
class LineF;
class Rect;
class RectF;
class DM_DLL_EXPORT GraphicsMath
{
public:
    //Deprecated! Recommand don't use this function, these functions is slower than lineIntersect functions
    static int pointAtLineLeftRight(const Point &ptStart, const Point &ptEnd, const Point &ptTest);

    static int pointAtLineLeftRight(const Line &line, const Point &ptTest);

    static dbool isTwoLineIntersect(const Point &ptLine1Start, const Point &ptLine1End, const Point &ptLine2Start, const Point &ptLine2End);

    static dbool isTwoLineIntersect(const Line &l1, const Line &l2);

    static dbool isLineIntersectRect(const Point &ptStart, const Point &ptEnd, const Rect &rect);

    static dbool isLineIntersectRect(const Line &line, const Rect &rect);
    //Deprecated end

    /** Rotates a point counter clockwise by the angle around a pivot
     @param v is the point to rotate
     @param pivot is the pivot, naturally
     @param angle is the angle of rotation cw in radians
     @returns the rotated point
     */
    static PointF rotateByAngle(const PointF &v, const PointF &pivot, dreal angle, dbool inDegree = true);

    static dint guessQuadrant(const PointF &p0, const PointF &p1);

    static PointF pointAtRect(const Point &p0, const Point &p1, const Rect &rect, dbool &success);
    static PointF pointAtRect(const PointF &p0, const PointF &p1, const RectF &rect, dbool &success);

    //----------------------line------------------------------

    static dbool lineIntersect(const Point& A, const Point& B,
                               const Point& C, const Point& D);

    static Point intersectPoint(const Point& A, const Point& B,
                                 const Point& C, const Point& D);

    static dbool mathlineIntersect(const Point& A, const Point& B,
                                   const Point& C, const Point& D,
                                   dreal *S, dreal *T);

    static dbool lineIntersectRect(const Point& A, const Point& B, const Rect &R);
    static dbool lineIntersectRect(const Line &L, const Rect &R);

    //float version

    static dbool lineIntersectF(const PointF& A, const PointF& B,
                               const PointF& C, const PointF& D);

    static PointF intersectPointF(const PointF& A, const PointF& B,
                                 const PointF& C, const PointF& D);

    static dbool mathlineIntersectF(const PointF& A, const PointF& B,
                                   const PointF& C, const PointF& D,
                                   dreal *S, dreal *T);

    static dbool lineIntersectRectF(const PointF& A, const PointF& B, const RectF &R);
    static dbool lineIntersectRectF(const LineF &L, const RectF &R);
};
DM_END_NAMESPACE
#endif // DMGRAPHICSMATH_H
