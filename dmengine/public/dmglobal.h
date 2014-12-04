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
#ifndef DMGLOBAL_H
#define DMGLOBAL_H
#include "dmpublic.h"
#if defined(DM_OS_DARWIN) || !defined(DM_OS_OPENBSD) && defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && (_POSIX_THREAD_PRIORITY_SCHEDULING-0 >= 0)
#define DM_HAS_THREAD_PRIORITY_SCHEDULING
#endif

#ifndef DM_FmIu64
#ifdef DM_CC_MSVC
#define DM_FmIu64 "I64u"
#else
#if __WORDSIZE == 64
#define DM_FmIu64 "lu"
#else
#define DM_FmIu64 "llu"
#endif
#endif
#endif

#ifndef DM_FmI64
#ifdef DM_CC_MSVC
#define DM_FmI64 "I64d"
#else
#if __WORDSIZE == 64
#define DM_FmI64 "ld"
#else
#define DM_FmI64 "lld"
#endif
#endif
#endif

#ifndef DM_FLT_EPSILON
#define DM_FLT_EPSILON     1.192092896e-07F
#endif // FLT_EPSILON

#ifndef DM_SAFE_BUF
#define DM_SAFE_BUF static
#endif

#define DM_MEMORYNUM 1024

#define DM_BEGIN_C_HEADER extern "C" {
#define DM_END_C_HEADER }
#define DM_BEGIN_CPP_HEADER extern "C++" {
#define DM_END_CPP_HEADER }

#define DM_PRIVATE_CLASS(CLASS) class CLASS##Private
#define DM_DECLARE_PRIVATE(CLASS)                   \
            friend class CLASS##Private;            \
            public: const char* className() const { return #CLASS; } \
            private:                                \
                CLASS##Private* pdm;                \
                const CLASS##Private* d_func() const { return pdm; } \
                CLASS##Private* d_func() { return pdm; }

#define DM_DECLARE_PRIVATE_N(CLASS)                   \
            friend class CLASS##Private;            \
            public: const char* className() const { return #CLASS; } \
            private:                                \
                const CLASS##Private* d_func() const { return reinterpret_cast<CLASS##Private*>(pdm); } \
                CLASS##Private* d_func() { return reinterpret_cast<CLASS##Private*>(pdm); }

/*
   Some classes do not permit copies to be made of an object. These
   classes contains a private copy constructor and assignment
   operator to disable copying (the compiler gives an error message).
*/
#define DM_DISABLE_COPY(Class) \
    Class(const Class &); \
    Class &operator=(const Class &);

#define DM_PROPERTY(VarType, VarName, FunName) \
        protected: VarType VarName;\
            public: VarType get##FunName(void) const { return VarName; }\
            public: void set##FunName(VarType var) { VarName = var; }

#define DM_PROPERTY_REF(VarType, VarName, FunName) \
        protected: VarType VarName;\
            public: const VarType& get##FunName(void) const { return VarName; }\
            public: void set##FunName(const VarType &var) { VarName = var; }

#define DM_PROPERTY_DETACH(VarType, VarName, FunName, DetachFunName) \
        public: VarType get##FunName(void) const { return VarName; }\
        public: void set##FunName(VarType var) { if (VarName != var) { DetachFunName(); VarName = var; } }

#define DM_PROPERTY_DETACH_REF(VarType, VarName, FunName, DetachFunName) \
        public: const VarType& get##FunName(void) const { return VarName; }\
        public: void set##FunName(const VarType &var) { if (VarName != var) { DetachFunName(); VarName = var; } }

#define DM_DECLARE_PROPERTY(VarType, VarName, FunName) \
        protected: VarType VarName;\
            public: VarType get##FunName(void) const;\
            public: void set##FunName(VarType var);

#define DM_DECLARE_PROPERTY_REF(VarType, VarName, FunName) \
        protected: VarType VarName;\
            public: const VarType& get##FunName(void) const;\
            public: void set##FunName(const VarType &var);

#define DM_V_PROPERTY(VarType, VarName, FunName) \
        protected: VarType VarName;\
            public: virtual VarType get##FunName(void) const { return VarName; }\
            public: virtual void set##FunName(VarType var) { VarName = var; }

#define DM_V_PROPERTY_REF(VarType, VarName, FunName) \
        protected: VarType VarName;\
            public: virtual const VarType& get##FunName(void) const { return VarName; }\
            public: virtual void set##FunName(const VarType &var) { VarName = var; }

#define DM_V_PROPERTY_DETACH(VarType, VarName, FunName, DetachFunName) \
        public: virtual VarType get##FunName(void) const { return VarName; }\
        public: virtual void set##FunName(VarType var) { if (VarName != var) { DetachFunName(); VarName = var; } }

#define DM_V_PROPERTY_DETACH_REF(VarType, VarName, FunName, DetachFunName) \
        public: virtual const VarType& get##FunName(void) const { return VarName; }\
        public: virtual void set##FunName(const VarType &var) { if (VarName != var) { DetachFunName(); VarName = var; } }

#define DM_DECLARE_V_PROPERTY(VarType, VarName, FunName) \
        protected: VarType VarName;\
            public: virtual VarType get##FunName(void) const;\
            public: virtual void set##FunName(VarType var);

#define DM_DECLARE_V_PROPERTY_REF(VarType, VarName, FunName) \
        protected: VarType VarName;\
            public: virtual const VarType& get##FunName(void) const;\
            public: virtual void set##FunName(const VarType &var);

#define DM_BEGIN_NAMESPACE namespace DM {
#define DM_END_NAMESPACE }

#define DM_BEGIN_NS_THREAD namespace Threads {
#define DM_END_NS_THREAD }

#define DM_USING_NAMESPACE using namespace DM

#define DM_GLOBEL_INSTANCE(CLASS, NAME)                     \
                static CLASS* NAME##_globel_instance = 0;   \
                CLASS* NAME##_instance()                     \
                {                                           \
                    return NAME##_globel_instance;          \
                }

#define DM_GLOBEL_INSTANCE_QUICK(CLASS, NAME)           \
                static CLASS NAME##_globel_instance;    \
                CLASS* NAME##_instance()                       \
                {                                       \
                    return  &NAME##_globel_instance;    \
                }

//This is simple and effective
#define DM_FOREACH(ITEM, VEC) \
        for (int _d_index=0, _tmp=0; _d_index<VEC.size(); _tmp=_d_index)\
            for (ITEM=VEC.at(_d_index); _tmp==_d_index; ++_d_index)

#define DM_FOREACH_HASH(ITEM, MAP, KEY, TYPE) \
        for (DM::Hash<KEY, TYPE>::iterator _i_it=MAP.begin(), _t_it=MAP.begin(); _i_it!=MAP.constEnd(); _t_it=_i_it)\
            for (ITEM=_i_it.value(); _t_it==_i_it; ++_i_it)

inline void dm_noop() {}
#ifdef DM_NO_EXCEPTIONS
#  define DM_TRY if (true)
#  define DM_CATCH(A) else
#  define DM_THROW(A) dm_noop()
#  define DM_RETHROW dm_noop()
#else
#  define DM_TRY try
#  define DM_CATCH(A) catch (A)
#  define DM_THROW(A) throw A
#  define DM_RETHROW throw
#endif
#ifndef NULL
#define NULL 0
#endif

#define DM_INTERNAL_FUNC(Return, FuncName, Param) Return __dm_##FuncName Param
#define DM_INTERNAL_FUNCNAME(FuncName) __dm_##FuncName

#ifdef DM_COMPILER_CONSTEXPR
# define DM_DECL_CONSTEXPR constexpr
#else
# define DM_DECL_CONSTEXPR
#endif

#ifndef DM_CONSTRUCTOR_FUNCTION
# define DM_CONSTRUCTOR_FUNCTION(FUNC) \
    static const int FUNC ## __init_variable__ = FUNC();
#endif //DM_CONSTRUCTOR_FUNCTION

#ifndef DM_DESTRUCTOR_FUNCTION
#define DM_DESTRUCTOR_FUNCTION(FUNC) \
    class FUNC ## __rel_class__ { \
    public: \
       inline FUNC ## __rel_class__() { } \
       inline ~ FUNC ## __rel_class__() { FUNC(); } \
    } FUNC ## __rel_instance__;
#endif //DM_DESTRUCTOR_FUNCTION

#ifndef DM_UNUSED
#define DM_UNUSED(x) (void)(x)
#endif

#ifndef DM_ASSERT
#include <assert.h>
#define DM_ASSERT(expr) assert(expr)
#endif

#ifndef DM_SAFE_DELETE
#define DM_SAFE_DELETE(p)           \
                do {                \
                    if (p != NULL)  \
                    { delete p;     \
                      p = NULL;   } \
                } while (0)
#endif

#ifndef DM_SAFE_DELETE_ARRAY
#define DM_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = 0; } } while(0)
#endif

#ifndef DM_SAFE_RETAIN
#define DM_SAFE_RETAIN(p)       \
                do {            \
                    if (p != NULL)  \
                    { p->retain(); }\
                } while (0)
#endif //DM_SAFE_RETAIN

#ifndef DM_SAFE_RELEASE
#define DM_SAFE_RELEASE(p)       \
                do {            \
                    if (p != NULL)  \
                    { p->release();   \
                      p = NULL;   }\
                } while (0)
#endif //DM_SAFE_RETAIN

#define C_D(CLASS)                                      pdm(new CLASS##Private)
#define C_D1(CLASS, P0)                                 pdm(new CLASS##Private(P0))
#define C_D2(CLASS, P0, P1)                             pdm(new CLASS##Private(P0, P1))
#define C_D3(CLASS, P0, P1, P2)                         pdm(new CLASS##Private(P0, P1, P2))
#define C_D4(CLASS, P0, P1, P2, P3)                     pdm(new CLASS##Private(P0, P1, P2, P3))
#define C_D5(CLASS, P0, P1, P2, P3, P4)                 pdm(new CLASS##Private(P0, P1, P2, P3, P4))
#define C_D6(CLASS, P0, P1, P2, P3, P4, P5)             pdm(new CLASS##Private(P0, P1, P2, P3, P4, P5))
#define C_D7(CLASS, P0, P1, P2, P3, P4, P5, P6)         pdm(new CLASS##Private(P0, P1, P2, P3, P4, P5, P6))
#define C_D8(CLASS, P0, P1, P2, P3, P4, P5, P6, P7)     pdm(new CLASS##Private(P0, P1, P2, P3, P4, P5, P6, P7))
#define S_D(x)                                          pdm(x)
#define D_D(CLASS)                                      DM_SAFE_DELETE(pdm)

#ifndef DM_APP_LOCK
#define DM_APP_LOCK
#endif
                    
#ifndef DM_APP_UNLOCK
#define DM_APP_UNLOCK
#endif

#ifdef DM_OS_WIN32
#define DM_LINE_ENDING "\r\n"
#else
#define LINE_ENDING "\n"
#endif

template<class T, bool(T::*fn)()>
bool remove_compare(void* obj)
{
    return (static_cast<T*>(obj)->*fn)();
}

#define DM_REMOVE_INVALID_IT(LIST, TYPENAME, COMPARE_FUNC) \
    do { \
        Vector<TYPENAME*>::iterator it = LIST.begin(); \
        while (it != LIST.constEnd()) { \
            TYPENAME* t = *it; \
            if (t->COMPARE_FUNC()) { \
                it = LIST.erase(it); \
                t->release(); \
                continue; \
            } \
            ++it; \
        }; \
    } while (0)

#define DM_CHECK_VALID_IT(LIST, TYPENAME, COMPARE_FUNC, IT_FUNC, PARAM) \
    do { \
        Vector<TYPENAME*>::iterator it = LIST.begin(); \
        while (it != LIST.constEnd()) { \
            TYPENAME* t = *it; \
            if (t->COMPARE_FUNC()) { \
                it = LIST.erase(it); \
                DM_LOGI("DM_CHECK_VALID_IT release %p", t); \
                t->release(); \
                continue; \
            } \
            else { \
                t->IT_FUNC(PARAM); ++it; \
            }\
        }; \
    } while (0)

/* Machine byte-order */
#define DM_BIG_ENDIAN 4321
#define DM_LITTLE_ENDIAN 1234

#define DM_GLOBAL_REF(CLASS, NAME) \
    static DM::Ref<CLASS> NAME##Ref(CLASS::getInstance())

#define DM_GLOBAL_OBJ(CLASS) (*(CLASS::instance()))

#define DM_GLOBAL_USE(CLASS) (*(DM::Singleton<CLASS>::getInstance()))

#define DM_GLOBAL_MEMBER(CLASS, GetFunc) (*(CLASS::instance()->GetFunc()))

#define DM_GLOBAL(CLASS, NAME) \
    DM_GLOBAL_REF(CLASS, NAME); \
    static CLASS & NAME = (*(NAME##Ref.get()))


DM_BEGIN_NAMESPACE
template <typename T>    
inline void* FuncPtrValue(T fun)    
{    
    return *(void **)&fun;    
}
DM_END_NAMESPACE
//=================================CONFIG====================================
#define DM_BYTE_ORDER DM_LITTLE_ENDIAN
#if 0
#if defined(DM_CC_MSVC)
    #ifdef _DEBUG
        #define DM_DEBUG
    #endif
#else //NO DM_CC_MSVC
    #define DM_DEBUG
#endif
#endif

#if defined(_DEBUG) || defined(DEBUG) || defined(__DEBUG__)
    #define DM_DEBUG
#endif

#define DM_QUICK_TIME
#define DM_ARCH_I386

#define DM_MATH_COORDINATE

#define DM_GRAPHICS_USE_REAL

/*
 * I test this functions.. SqrtFast is NOT always faster then sqrtf... shit!
 * BUT SinFast, CosFast and rSqrtFast is really faster then sin, cos and rSqrt
 */
#define DM_USE_FAST_MATH_FUNCTION

#ifndef DM_USE_FLOATS
    #define DM_USE_FLOATS
#endif

#define DM_SUPPORT_OBJECT_PRO
//================================CONFIG END=================================

#endif // DMGLOBAL_H
