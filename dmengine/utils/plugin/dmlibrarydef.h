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
#ifndef DMLIBRARYDEF_H
#define DMLIBRARYDEF_H
#include "dmpublic.h"

#define DM_LIBRARY_CLAZZ_CREATE     "create_clazz"
#define DM_LIBRARY_CLAZZ_DESTROY    "destroy_clazz"
#define DM_LIBRARY_CLAZZ_VERSION    "version_clazz"
#define DM_LIBRARY_CLAZZ_NAME       "name_clazz"
#define DM_LIBRARY_CLAZZ_INFO       "classinfo_clazz"

#define DM_DECLARE_CREATE_CLAZZ     extern "C" DM_DECL_EXPORT  void* create_clazz()
#define DM_DECLARE_DESTROY_CLAZZ(PARAM)    extern "C" DM_DECL_EXPORT  void destroy_clazz(void* PARAM)
#define DM_DECLARE_VERSION_CLAZZ    extern "C" DM_DECL_EXPORT  int version_clazz()
#define DM_DECLARE_NAME_CLAZZ    extern "C" DM_DECL_EXPORT  const char* name_clazz()

typedef void* dm_create_clazz_t();
typedef void dm_destroy_clazz_t(void*);
typedef int dm_version_clazz_t();
typedef const char* dm_name_clazz_t();

struct DmLibraryClass
{
    dm_create_clazz_t*  createFunc;
    dm_destroy_clazz_t* destroyFunc;
    dm_version_clazz_t* versionFunc;
    dm_name_clazz_t*    nameFunc;
};

typedef DmLibraryClass *dm_classinfo_clazz_t();

#define DM_DECLARE_LIBRARY_CLASS_FUNCS(CLASSNAME, LIBVERSION) \
            extern "C" DM_DECL_EXPORT void* create_clazz() { return new CLASSNAME; } \
            extern "C" DM_DECL_EXPORT void destroy_clazz(void* p) { delete static_cast<CLASSNAME*>(p); } \
            extern "C" DM_DECL_EXPORT int version_clazz() { return LIBVERSION; } \
            extern "C" DM_DECL_EXPORT const char* name_clazz() { return #CLASSNAME; }

#define DM_DECLARE_LIBRARY_CLASS_INFO(CLASSNAME) \
            static struct DmLibraryClass CLASSNAME##_ClassInfo = { create_clazz, destroy_clazz, version_clazz, name_clazz }; \
            extern "C" DM_DECL_EXPORT DmLibraryClass *classinfo_clazz() { return &CLASSNAME##_ClassInfo; }

#define DM_DECLARE_LIBRARY_CLASS(CLASSNAME, LIBVERSION) \
            DM_DECLARE_LIBRARY_CLASS_FUNCS(CLASSNAME, LIBVERSION) \
            DM_DECLARE_LIBRARY_CLASS_INFO(CLASSNAME)
        

#define DM_DEFINE_METHOD_LINKAGE_BASE(result, linkage, name, args, args2) \
    protected: \
        typedef result (linkage * name##_METHOD) args; \
        union { \
            name##_METHOD m_##name; \
            void*         m_##name##_ptr; \
        }; \
    public: \
        virtual result name args \
        { \
            return m_##name args2; \
        }

#define DM_DEFINE_METHOD_LINKAGE0(result, linkage, name) \
        DM_DEFINE_METHOD_LINKAGE_BASE(result, linkage, name, ()  , ())

#define DM_DEFINE_METHOD_LINKAGE1(result, linkage, name, args) \
        DM_DEFINE_METHOD_LINKAGE_BASE(result, linkage, name, args, (p1))

#define DM_DEFINE_METHOD_LINKAGE2(result, linkage, name, args) \
        DM_DEFINE_METHOD_LINKAGE_BASE(result, linkage, name, args, (p1, p2))

#define DM_DEFINE_METHOD_LINKAGE3(result, linkage, name, args) \
        DM_DEFINE_METHOD_LINKAGE_BASE(result, linkage, name, args, (p1, p2, p3))

#define DM_DEFINE_METHOD_LINKAGE4(result, linkage, name, args) \
        DM_DEFINE_METHOD_LINKAGE_BASE(result, linkage, name, args, (p1, p2, p3, p4))

#define DM_DEFINE_METHOD_LINKAGE5(result, linkage, name, args) \
        DM_DEFINE_METHOD_LINKAGE_BASE(result, linkage, name, args, (p1, p2, p3, p4, p5))

#define DM_DEFINE_METHOD_LINKAGE6(result, linkage, name, args) \
        DM_DEFINE_METHOD_LINKAGE_BASE(result, linkage, name, args, (p1, p2, p3, p4, p5, p6))

#define DM_DEFINE_METHOD_LINKAGE7(result, linkage, name, args) \
        DM_DEFINE_METHOD_LINKAGE_BASE(result, linkage, name, args, (p1, p2, p3, p4, p5, p6, p7))

#define DM_DEFINE_METHOD_LINKAGE8(result, linkage, name, args) \
        DM_DEFINE_METHOD_LINKAGE_BASE(result, linkage, name, args, (p1, p2, p3, p4, p5, p6, p7, p8))

#define DM_DEFINE_METHOD_LINKAGE9(result, linkage, name, args) \
        DM_DEFINE_METHOD_LINKAGE_BASE(result, linkage, name, args, (p1, p2, p3, p4, p5, p6, p7, p8, p9))

#define DM_DEFINE_METHOD_LINKAGE10(result, linkage, name, args) \
        DM_DEFINE_METHOD_LINKAGE_BASE(result, linkage, name, args, (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10))

#define DM_DEFINE_METHOD_LINKAGE11(result, linkage, name, args) \
        DM_DEFINE_METHOD_LINKAGE_BASE(result, linkage, name, args, (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11))

#define DM_LOAD_SYMBOLS() \
    protected: \
        virtual bool loadSymbols() { return true; }

#define DM_DEFINE_METHOD0(result, name)         DM_DEFINE_METHOD_LINKAGE0(result, __cdecl, name)
#define DM_DEFINE_METHOD1(result, name, args)   DM_DEFINE_METHOD_LINKAGE1(result, __cdecl, name, args)
#define DM_DEFINE_METHOD2(result, name, args)   DM_DEFINE_METHOD_LINKAGE2(result, __cdecl, name, args)
#define DM_DEFINE_METHOD3(result, name, args)   DM_DEFINE_METHOD_LINKAGE3(result, __cdecl, name, args)
#define DM_DEFINE_METHOD4(result, name, args)   DM_DEFINE_METHOD_LINKAGE4(result, __cdecl, name, args)
#define DM_DEFINE_METHOD5(result, name, args)   DM_DEFINE_METHOD_LINKAGE5(result, __cdecl, name, args)
#define DM_DEFINE_METHOD6(result, name, args)   DM_DEFINE_METHOD_LINKAGE6(result, __cdecl, name, args)
#define DM_DEFINE_METHOD7(result, name, args)   DM_DEFINE_METHOD_LINKAGE7(result, __cdecl, name, args)
#define DM_DEFINE_METHOD8(result, name, args)   DM_DEFINE_METHOD_LINKAGE8(result, __cdecl, name, args)
#define DM_DEFINE_METHOD9(result, name, args)   DM_DEFINE_METHOD_LINKAGE9(result, __cdecl, name, args)
#define DM_DEFINE_METHOD10(result, name, args)  DM_DEFINE_METHOD_LINKAGE10(result, __cdecl, name, args)
#define DM_DEFINE_METHOD11(result, name, args)  DM_DEFINE_METHOD_LINKAGE11(result, __cdecl, name, args)


#ifdef DM_CC_MSVC
///////////////////////////////////////////////////////////
//
//  DM_DEFINE_FUNC_ALIGNED 0-X
//
//  Defines a function for an export from a dll, wich
//  require a aligned stack on function call
//  Use DM_DEFINE_FUNC_ALIGNED for each function to be resolved.
//
//  result:  Result of the function
//  linkage: Calling convention of the function
//  name:    Name of the function
//  args:    Argument types of the function
//
//  Actual function call will expand to something like this
//  this will align the stack (esp) at the point of function
//  entry as required by gcc compiled dlls, it is abit abfuscated
//  to allow for different sized variables
//
//  int64_t test(int64_t p1, char p2, char p3)
//  {
//    int o,s = ((sizeof(p1)+3)&~3)+((sizeof(p2)+3)&~3)+((sizeof(p3)+3)&~3);
//    __asm mov [o],esp;
//    __asm sub esp, [s];
//    __asm and esp, ~15;
//    __asm add esp, [s]
//    m_test(p1, p2, p3);  //return value will still be correct aslong as we don't mess with it
//    __asm mov esp,[o];
//  };

#define ALS(a) ((sizeof(a)+3)&~3)
#define DM_DEFINE_FUNC_PART1(result, linkage, name, args) \
  private:                                             \
    typedef result (linkage * name##_type)##args;      \
    union { \
      name##_type m_##name;                            \
      void*       m_##name##_ptr;                      \
    }; \
  public:                                              \
    virtual result name##args

#define DM_DEFINE_FUNC_PART2(size) \
  {                             \
    int o,s = size;             \
    __asm {                     \
      __asm mov [o], esp        \
      __asm sub esp, [s]        \
      __asm and esp, ~15        \
      __asm add esp, [s]        \
    }

#define DM_DEFINE_FUNC_PART3(name,args) \
    m_##name##args;                  \
    __asm {                          \
      __asm mov esp,[o]              \
    }                                \
  }

#define DM_DEFINE_FUNC_ALIGNED0(result, linkage, name) \
    DM_DEFINE_FUNC_PART1(result, linkage, name, ()) \
    DM_DEFINE_FUNC_PART2(0) \
    DM_DEFINE_FUNC_PART3(name,())

#define DM_DEFINE_FUNC_ALIGNED1(result, linkage, name, t1) \
    DM_DEFINE_FUNC_PART1(result, linkage, name, (t1 p1)) \
    DM_DEFINE_FUNC_PART2(ALS(p1)) \
    DM_DEFINE_FUNC_PART3(name,(p1))

#define DM_DEFINE_FUNC_ALIGNED2(result, linkage, name, t1, t2) \
    DM_DEFINE_FUNC_PART1(result, linkage, name, (t1 p1, t2 p2)) \
    DM_DEFINE_FUNC_PART2(ALS(p1)+ALS(p2)) \
    DM_DEFINE_FUNC_PART3(name,(p1, p2))

#define DM_DEFINE_FUNC_ALIGNED3(result, linkage, name, t1, t2, t3) \
    DM_DEFINE_FUNC_PART1(result, linkage, name, (t1 p1, t2 p2, t3 p3)) \
    DM_DEFINE_FUNC_PART2(ALS(p1)+ALS(p2)+ALS(p3)) \
    DM_DEFINE_FUNC_PART3(name,(p1, p2, p3))

#define DM_DEFINE_FUNC_ALIGNED4(result, linkage, name, t1, t2, t3, t4) \
    DM_DEFINE_FUNC_PART1(result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4)) \
    DM_DEFINE_FUNC_PART2(ALS(p1)+ALS(p2)+ALS(p3)+ALS(p4)) \
    DM_DEFINE_FUNC_PART3(name,(p1, p2, p3, p4))

#define DM_DEFINE_FUNC_ALIGNED5(result, linkage, name, t1, t2, t3, t4, t5) \
    DM_DEFINE_FUNC_PART1(result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4, t5 p5)) \
    DM_DEFINE_FUNC_PART2(ALS(p1)+ALS(p2)+ALS(p3)+ALS(p4)+ALS(p5)) \
    DM_DEFINE_FUNC_PART3(name,(p1, p2, p3, p4, p5))

#define DM_DEFINE_FUNC_ALIGNED6(result, linkage, name, t1, t2, t3, t4, t5, t6) \
    DM_DEFINE_FUNC_PART1(result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6)) \
    DM_DEFINE_FUNC_PART2(ALS(p1)+ALS(p2)+ALS(p3)+ALS(p4)+ALS(p5)+ALS(p6)) \
    DM_DEFINE_FUNC_PART3(name,(p1, p2, p3, p4, p5, p6))

#define DM_DEFINE_FUNC_ALIGNED7(result, linkage, name, t1, t2, t3, t4, t5, t6, t7) \
    DM_DEFINE_FUNC_PART1(result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7)) \
    DM_DEFINE_FUNC_PART2(ALS(p1)+ALS(p2)+ALS(p3)+ALS(p4)+ALS(p5)+ALS(p6)+ALS(p7)) \
    DM_DEFINE_FUNC_PART3(name,(p1, p2, p3, p4, p5, p6, p7))

#define DM_DEFINE_FUNC_ALIGNED8(result, linkage, name, t1, t2, t3, t4, t5, t6, t7, t8) \
    DM_DEFINE_FUNC_PART1(result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7, t8 p8)) \
    DM_DEFINE_FUNC_PART2(ALS(p1)+ALS(p2)+ALS(p3)+ALS(p4)+ALS(p5)+ALS(p6)+ALS(p7)+ALS(p8)) \
    DM_DEFINE_FUNC_PART3(name,(p1, p2, p3, p4, p5, p6, p7, p8))

#define DM_DEFINE_FUNC_ALIGNED9(result, linkage, name, t1, t2, t3, t4, t5, t6, t7, t8, t9) \
    DM_DEFINE_FUNC_PART1(result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7, t8 p8, t9 p9)) \
    DM_DEFINE_FUNC_PART2(ALS(p1)+ALS(p2)+ALS(p3)+ALS(p4)+ALS(p5)+ALS(p6)+ALS(p7)+ALS(p8)+ALS(p9)) \
    DM_DEFINE_FUNC_PART3(name,(p1, p2, p3, p4, p5, p6, p7, p8, p9))

#else

#define DM_DEFINE_FUNC_ALIGNED0(result, linkage, name)                                            DM_DEFINE_METHOD_LINKAGE0 (result, linkage, name)
#define DM_DEFINE_FUNC_ALIGNED1(result, linkage, name, t1)                                        DM_DEFINE_METHOD_LINKAGE1 (result, linkage, name, (t1 p1) )
#define DM_DEFINE_FUNC_ALIGNED2(result, linkage, name, t1, t2)                                    DM_DEFINE_METHOD_LINKAGE2 (result, linkage, name, (t1 p1, t2 p2) )
#define DM_DEFINE_FUNC_ALIGNED3(result, linkage, name, t1, t2, t3)                                DM_DEFINE_METHOD_LINKAGE3 (result, linkage, name, (t1 p1, t2 p2, t3 p3) )
#define DM_DEFINE_FUNC_ALIGNED4(result, linkage, name, t1, t2, t3, t4)                            DM_DEFINE_METHOD_LINKAGE4 (result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4) )
#define DM_DEFINE_FUNC_ALIGNED5(result, linkage, name, t1, t2, t3, t4, t5)                        DM_DEFINE_METHOD_LINKAGE5 (result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4, t5 p5) )
#define DM_DEFINE_FUNC_ALIGNED6(result, linkage, name, t1, t2, t3, t4, t5, t6)                    DM_DEFINE_METHOD_LINKAGE6 (result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6) )
#define DM_DEFINE_FUNC_ALIGNED7(result, linkage, name, t1, t2, t3, t4, t5, t6, t7)                DM_DEFINE_METHOD_LINKAGE7 (result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7) )
#define DM_DEFINE_FUNC_ALIGNED8(result, linkage, name, t1, t2, t3, t4, t5, t6, t7, t8)            DM_DEFINE_METHOD_LINKAGE8 (result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7, t8 p8) )
#define DM_DEFINE_FUNC_ALIGNED9(result, linkage, name, t1, t2, t3, t4, t5, t6, t7, t8, t9)        DM_DEFINE_METHOD_LINKAGE9 (result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7, t8 p8, t9 p9) )
#define DM_DEFINE_FUNC_ALIGNED10(result, linkage, name, t1, t2, t3, t4, t5, t6, t7, t8, t10)      DM_DEFINE_METHOD_LINKAGE10(result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7, t8 p8, t9 p9, t10 p10) )
#define DM_DEFINE_FUNC_ALIGNED11(result, linkage, name, t1, t2, t3, t4, t5, t6, t7, t8, t10, t11) DM_DEFINE_METHOD_LINKAGE11(result, linkage, name, (t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7, t8 p8, t9 p9, t10 p10, t11 p11) )

#endif
#endif // DMLIBRARYDEF_H
