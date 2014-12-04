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
#ifndef _DMMATH_H_
#define _DMMATH_H_
#include <math.h>
#include "dmnamespace.h"

DM_BEGIN_NAMESPACE
#ifndef DM_PI
  //#define DM_PI 3.1415926535
    #define DM_PI 3.141593f
#endif

#define DM_PIUnder180 57.295779f // 180 / DM_PI
#define DM_PIOver180 0.017453f // DM_PI / 180;

#define DM_SINE_TABLE_SIZE 256
extern const dreal dm_sine_table[DM_SINE_TABLE_SIZE];

template <typename T>
inline T Abs(const T &t) { return t >= 0 ? t : -t; }

inline int Round(dreal d)
{ return d >= 0.0 ? int(d + 0.5) : int(d - int(d-1) + 0.5) + int(d-1); }

#if 0
#if defined(DM_NO_FPU) || defined(DM_ARCH_ARM) || defined(DM_ARCH_WINDOWSCE) || defined(DM_ARCH_SYMBIAN)
DM_DECL_CONSTEXPR inline dint64 Round64(double d)
{ return d >= 0.0 ? dint64(d + 0.5) : dint64(d - dreal(dint64(d-1)) + 0.5) + dint64(d-1); }
#else
DM_DECL_CONSTEXPR inline dint64 Round64(dreal d)
{ return d >= dreal(0.0) ? dint64(d + dreal(0.5)) : dint64(d - dreal(dint64(d-1)) + dreal(0.5)) + dint64(d-1); }
#endif
#else
DM_DECL_CONSTEXPR inline dint64 Round64(double d)
{ return d >= 0.0 ? dint64(d + 0.5) : dint64(d - double(dint64(d-1)) + 0.5) + dint64(d-1); }
DM_DECL_CONSTEXPR inline dint64 Round64(float f)
{ return Round64(static_cast<double>(f)); }
#endif

template<class T>
inline const T& Max(const T &a, const T &b)
{
    return a > b ? a : b;
}

template<class T>
inline const T& Min(const T &a, const T &b)
{
    return a < b ? a : b;
}

template<class T>
inline const T& Bound(const T &min, const T &val, const T &max)
{
    return Max(min, Min(val, max));
}

template <typename T>
inline void Swap(T &value1, T &value2)
{
    const T t = value1;
    value1 = value2;
    value2 = t;
}

inline dbool FuzzyIsNull(double d)
{
    return Abs(d) <= 0.000000000001;
}

inline dbool FuzzyIsNull(float f)
{
    return Abs(f) <= 0.000001f;
}

inline dbool FuzzyCompare(double p1, double p2)
{
    return (Abs(p1 - p2) <= 0.000000000001 * Min(Abs(p1), Abs(p2)));
}

inline dbool FuzzyCompare(float p1, float p2)
{
    return (Abs(p1 - p2) <= 0.00001f * Min(Abs(p1), Abs(p2)));
}

static inline dbool IsNull(double d)
{
    union U {
        double d;
        duint64 u;
    };
    U val;
    val.d = d;
    return val.u == duint64(0);
}

static inline dbool IsNull(float f)
{
    union U {
        float f;
        duint32 u;
    };
    U val;
    val.f = f;
    return val.u == 0u;
}

dbool inline IsEqual(dreal lhs, dreal rhs) {
    return (lhs + DM_FLT_EPSILON > rhs && lhs - DM_FLT_EPSILON < rhs);
}

inline dreal Sin(dreal v)
{
#ifdef DM_USE_FLOATS
    if (sizeof(dreal) == sizeof(float))
        return sinf(float(v));
    else
#endif
        return sin(v);
}

inline dreal Cos(dreal v)
{
#ifdef DM_USE_FLOATS
    if (sizeof(dreal) == sizeof(float))
        return cosf(float(v));
    else
#endif
        return cos(v);
}

inline dreal SinFast(dreal x)
{
    /*
    dint si = dint(x * (0.5 * DM_SINE_TABLE_SIZE / DM_PI)); // Would be more accurate with Round, but slower.
    dreal d = x - si * (2.0 * DM_PI / DM_SINE_TABLE_SIZE);
    dint ci = si + DM_SINE_TABLE_SIZE / 4;
    si &= DM_SINE_TABLE_SIZE - 1;
    ci &= DM_SINE_TABLE_SIZE - 1;
    return dm_sine_table[si] + (dm_sine_table[ci] - 0.5 * dm_sine_table[si] * d) * d;
    */
    dint si = dint(x * 40.743665f);//40.74366543268974f);
    dreal d = x - si * 0.024544f;//0.0245436926054688f;
    dint ci = si + 64;
    si &= 255;
    ci &= 255;
    return dm_sine_table[si] + (dm_sine_table[ci] - 0.5f * dm_sine_table[si] * d) * d;
}

inline dreal CosFast(dreal x)
{
    /*
    dint ci = dint(x * (0.5 * DM_SINE_TABLE_SIZE / DM_PI)); // Would be more accurate with Round, but slower.
    dreal d = x - ci * (2.0 * DM_PI / DM_SINE_TABLE_SIZE);
    dint si = ci + DM_SINE_TABLE_SIZE / 4;
    si &= DM_SINE_TABLE_SIZE - 1;
    ci &= DM_SINE_TABLE_SIZE - 1;
    return dm_sine_table[si] - (dm_sine_table[ci] + 0.5 * dm_sine_table[si] * d) * d;
    */
    dint ci = dint(x * 40.743665f);//40.74366543268974f);
    dreal d = x - ci * 0.024544f;//0.0245436926054688f;
    dint si = ci + 64;
    si &= 255;
    ci &= 255;
    return dm_sine_table[si] - (dm_sine_table[ci] + 0.5f * dm_sine_table[si] * d) * d;
}

inline dreal Tan(dreal v)
{
#ifdef DM_USE_FLOATS
    if (sizeof(dreal) == sizeof(float))
        return tanf(float(v));
    else
#endif
        return tan(v);
}

inline dreal Acos(dreal v)
{
#ifdef DM_USE_FLOATS
    if (sizeof(dreal) == sizeof(float))
        return acosf(float(v));
    else
#endif
        return acos(v);
}

inline dreal Asin(dreal v)
{
#ifdef DM_USE_FLOATS
    if (sizeof(dreal) == sizeof(float))
        return asinf(float(v));
    else
#endif
        return asin(v);
}

inline dreal Atan(dreal v)
{
#ifdef DM_USE_FLOATS
    if(sizeof(dreal) == sizeof(float))
        return atanf(float(v));
    else
#endif
        return atan(v);
}

inline dreal Atan2(dreal x, dreal y)
{
#ifdef DM_USE_FLOATS
    if(sizeof(dreal) == sizeof(float))
        return atan2f(float(x), float(y));
    else
#endif
        return static_cast<dreal>(atan2(x, y));
}

//Converts a radian number to a degree
inline dreal radian2degree(dreal radian)
{
    return radian * DM_PIUnder180; //57.29577951f; // 180 / DM_PI;
}

//Converts a degree number to a radian
inline dreal degree2radian(dreal degree)
{
    return degree * DM_PIOver180; //0.01745329252f; // DM_PI / 180;
}

inline dreal Sqrt(dreal v)
{
#ifdef DM_USE_FLOATS
    if (sizeof(dreal) == sizeof(float))
        return sqrtf(float(v));
    else
#endif
        return sqrt(v);
}

inline dreal rSqrt(dreal v)
{
    return (1/Sqrt(v));
}

inline dreal SqrtFast(dreal x)
{
    float xhalf = 0.5f*x; 
    int i = *(int*)&x;      // get bits for floating VALUE 
    i = 0x5f375a86 - (i>>1); // gives initial guess y0
    x = *(float*)&i;        // convert bits BACK to float 
    x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy 
    //x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy 
    //x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy 
    return 1/x; 
}

inline dreal rSqrtFast(dreal x)
{
    float xhalf = 0.5f*x; 
    int i = *(int*)&x;      // get bits for floating VALUE 
    i = 0x5f375a86 - (i>>1); // gives initial guess y0
    x = *(float*)&i;        // convert bits BACK to float 
    x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy 
    //x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy 
    //x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy 
    return x;
}

/*
 * I test this functions.. SqrtFast is NOT always faster then sqrtf... shit!
 * BUT SinFast, CosFast and rSqrtFast is really faster then sin, cos and rSqrt
 */
#ifdef DM_USE_FAST_MATH_FUNCTION
//#define dmSqrt(x) DM::SqrtFast(x)
#define dmSqrt(x) DM::Sqrt(x)
#define dmRSqrt(x) DM::rSqrtFast(x)
#define dmSin(x) DM::SinFast(x)
#define dmCos(x) DM::CosFast(x)
#else
#define dmSqrt(x) DM::Sqrt(x)
#define dmRSqrt(x) DM::rSqrt(x)
#define dmSin(x) DM::Sin(x)
#define dmCos(x) DM::Cos(x)
#endif


DM_END_NAMESPACE
#endif //_DMMATH_H_

