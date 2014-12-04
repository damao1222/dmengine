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

#ifndef PUBLIC_H
#define PUBLIC_H
#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  define DM_OS_DARWIN
#  define DM_OS_BSD4
#  ifdef __LP64__
#    define DM_OS_DARWIN64
#  else
#    define DM_OS_DARWIN32
#  endif
#elif defined(__SYMBIAN32__) || defined(SYMBIAN)
#  define DM_OS_SYMBIAN
#  define DM_NO_POSIX_SIGNALS
#elif defined(__CYGWIN__)
#  define DM_OS_CYGWIN
#elif defined(MSDOS) || defined(_MSDOS)
#  define DM_OS_MSDOS
#elif defined(__OS2__)
#  if defined(__EMX__)
#    define DM_OS_OS2EMX
#  else
#    define DM_OS_OS2
#  endif
#elif !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define DM_OS_WIN32
#  define DM_OS_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  if defined(WINCE) || defined(_WIN32_WCE)
#    define DM_OS_WINCE
#  else
#    define DM_OS_WIN32
#  endif
#elif defined(__MWERKS__) && defined(__INTEL__)
#  define DM_OS_WIN32
#elif defined(__sun) || defined(sun)
#  define DM_OS_SOLARIS
#elif defined(hpux) || defined(__hpux)
#  define DM_OS_HPUX
#elif defined(__ultrix) || defined(ultrix)
#  define DM_OS_ULTRIX
#elif defined(sinix)
#  define DM_OS_RELIANT
#elif defined(__native_client__)
#  define DM_OS_NACL
#elif defined(__linux__) || defined(__linux)
#  define DM_OS_LINUX
#  if defined(__ANDROID__)
#    define DM_OS_ANDROID
#  endif
#elif defined(__FreeBSD__) || defined(__DragonFly__)
#  define DM_OS_FREEBSD
#  define DM_OS_BSD4
#elif defined(__NetBSD__)
#  define DM_OS_NETBSD
#  define DM_OS_BSD4
#elif defined(__OpenBSD__)
#  define DM_OS_OPENBSD
#  define DM_OS_BSD4
#elif defined(__bsdi__)
#  define DM_OS_BSDI
#  define DM_OS_BSD4
#elif defined(__sgi)
#  define DM_OS_IRIX
#elif defined(__osf__)
#  define DM_OS_OSF
#elif defined(_AIX)
#  define DM_OS_AIX
#elif defined(__Lynx__)
#  define DM_OS_LYNX
#elif defined(__GNU__)
#  define DM_OS_HURD
#elif defined(__DGUX__)
#  define DM_OS_DGUX
#elif defined(__QNXNTO__)
#  define DM_OS_QNX
#elif defined(_SEQUENT_)
#  define DM_OS_DYNIX
#elif defined(_SCO_DS) /* SCO OpenServer 5 + GCC */
#  define DM_OS_SCO
#elif defined(__USLC__) /* all SCO platforms + UDK or OUDK */
#  define DM_OS_UNIXWARE
#elif defined(__svr4__) && defined(i386) /* Open UNIX 8 + GCC */
#  define DM_OS_UNIXWARE
#elif defined(__INTEGRITY)
#  define DM_OS_INTEGRITY
#elif defined(VXWORKS) /* there is no "dreal" VxWorks define - this has to be set in the mkspec! */
#  define DM_OS_VXWORKS
#elif defined(__MAKEDEPEND__)
#else
#  error "Dm has not been ported to this OS"
#endif

#if defined(DM_OS_WIN32) || defined(DM_OS_WIN64) || defined(DM_OS_WINCE)
#  define DM_OS_WIN
#endif

#if defined(DM_OS_MSDOS) || defined(DM_OS_OS2) || defined(DM_OS_WIN)
#  undef DM_OS_UNIX
#elif !defined(DM_OS_UNIX)
#  define DM_OS_UNIX
#endif

#if defined(__arm__)
#  define DM_ARCH_ARM
#elif defined(__mips__)
#  define DM_ARCH_MIPS
#endif

#ifdef DM_ARCH_ARM
#if defined(__ARM_ARCH_7__) \
    || defined(__ARM_ARCH_7A__) \
    || defined(__ARM_ARCH_7R__) \
    || defined(__ARM_ARCH_7M__)
# define DM_ARCH_ARMV7
#elif defined(__ARM_ARCH_6__) \
    || defined(__ARM_ARCH_6J__) \
    || defined(__ARM_ARCH_6T2__) \
    || defined(__ARM_ARCH_6Z__) \
    || defined(__ARM_ARCH_6K__) \
    || defined(__ARM_ARCH_6ZK__) \
    || defined(__ARM_ARCH_6M__) \
    || (defined(__TARGET_ARCH_ARM) && (__TARGET_ARCH_ARM-0 >= 6))
# define DM_ARCH_ARMV6
#else
# define DM_ARCH_ARMV5
#endif
#endif

//compiler
/* Symantec C++ is now Digital Mars */
#if defined(__DMC__) || defined(__SC__)
#  define DM_CC_SYM
/* "explicit" semantics implemented in 8.1e but keyword recognized since 7.5 */
#  if defined(__SC__) && __SC__ < 0x750
#    define DM_NO_EXPLICIT_KEYWORD
#  endif
#  define DM_NO_USING_KEYWORD

#elif defined(__MWERKS__)
#  define DM_CC_MWERKS
#  if defined(__EMU_SYMBIAN_OS__)
#    define DM_CC_NOKIAX86
#  endif
/* "explicit" recognized since 4.0d1 */

#elif defined(_MSC_VER)
#  define DM_CC_MSVC
#  define DM_CC_MSVC_NET
#  define DM_CANNOT_DELETE_CONSTANT
#  define DM_OUTOFLINE_TEMPLATE inline
#  define DM_NO_TEMPLATE_FRIENDS
#  define DM_ALIGNOF(type) __alignof(type)
#  define DM_DECL_ALIGN(n) __declspec(align(n))
/* Intel C++ disguising as Visual C++: the `using' keyword avoids warnings */
#  if defined(__INTEL_COMPILER)
#    define DM_CC_INTEL
#  endif
/* MSVC does not support SSE/MMX on x64 */
#  if (defined(DM_CC_MSVC) && defined(_M_X64))
#    undef DM_HAVE_SSE
#    undef DM_HAVE_MMX
#    undef DM_HAVE_3DNOW
#  endif

#if defined(DM_CC_MSVC) && _MSC_VER >= 1600
#      define DM_COMPILER_RVALUE_REFS
#      define DM_COMPILER_AUTO_TYPE
#      define DM_COMPILER_LAMBDA
#      define DM_COMPILER_DECLTYPE
//  MSCV has std::initilizer_list, but do not support the braces initialization
//#      define DM_COMPILER_INITIALIZER_LISTS
#  endif


#elif defined(__BORLANDC__) || defined(__TURBOC__)
#  define DM_CC_BOR
#  define DM_INLINE_TEMPLATE
#  if __BORLANDC__ < 0x502
#    define DM_NO_BOOL_TYPE
#    define DM_NO_EXPLICIT_KEYWORD
#  endif
#  define DM_NO_USING_KEYWORD

#elif defined(__WATCOMC__)
#  define DM_CC_WAT

/* Symbian GCCE */
#elif defined(__GCCE__)
#  define DM_CC_GCCE
#  define DMT_VISIBILITY_AVAILABLE
#  if defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__)
#    define DM_HAVE_ARMV6
#  endif

/* ARM Realview Compiler Suite
   RVCT compiler also defines __EDG__ and __GNUC__ (if --gnu flag is given),
   so check for it before that */
#elif defined(__ARMCC__) || defined(__CC_ARM)
#  define DM_CC_RVCT
#  if __TARGET_ARCH_ARM >= 6
#    define DM_HAVE_ARMV6
#  endif
/* work-around for missing compiler intrinsics */
#  define __is_empty(X) false
#  define __is_pod(X) false
#elif defined(__GNUC__)
#  define DM_CC_GNU
#  define DM_C_CALLBACKS
#  if defined(__MINGW32__)
#    define DM_CC_MINGW
#  endif
#  if defined(__INTEL_COMPILER)
/* Intel C++ also masquerades as GCC 3.2.0 */
#    define DM_CC_INTEL
#  endif
#  if defined(__clang__)
/* Clang also masquerades as GCC 4.2.1 */
#    define DM_CC_CLANG
#  endif
#  ifdef __APPLE__
#    define DM_NO_DEPRECATED_CONSTRUCTORS
#  endif
#  if __GNUC__ == 2 && __GNUC_MINOR__ <= 7
#    define DM_FULL_TEMPLATE_INSTANTIATION
#  endif
/* GCC 2.95 knows "using" but does not support it correctly */
#  if __GNUC__ == 2 && __GNUC_MINOR__ <= 95
#    define DM_NO_USING_KEYWORD
#    define DM_NO_STL_WCHAR
#  endif
#  if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
#    define DM_ALIGNOF(type)   __alignof__(type)
#    define DM_TYPEOF(expr)    __typeof__(expr)
#    define DM_DECL_ALIGN(n)   __attribute__((__aligned__(n)))
#  endif
#  if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 96)
#    define DM_LIKELY(expr)    __builtin_expect(!!(expr), true)
#    define DM_UNLIKELY(expr)  __builtin_expect(!!(expr), false)
#  endif
/* GCC 3.1 and GCC 3.2 wrongly define _SB_CTYPE_MACROS on HP-UX */
#  if defined(DM_OS_HPUX) && __GNUC__ == 3 && __GNUC_MINOR__ >= 1
#    define DM_WRONG_SB_CTYPE_MACROS
#  endif
/* GCC <= 3.3 cannot handle template friends */
#  if __GNUC__ < 3 || (__GNUC__ == 3 && __GNUC_MINOR__ <= 3)
#    define DM_NO_TEMPLATE_FRIENDS
#  endif
/* Apple's GCC 3.1 chokes on our streaming qDebug() */
#  if defined(DM_OS_DARWIN) && __GNUC__ == 3 && (__GNUC_MINOR__ >= 1 && __GNUC_MINOR__ < 3)
#    define DM_BROKEN_DEBUG_STREAM
#  endif
#  if (defined(DM_CC_GNU) || defined(DM_CC_INTEL)) && !defined(DM_MOC_CPP)
#    define DM_PACKED __attribute__ ((__packed__))
#    define DM_NO_PACKED_REFERENCE
#    ifndef __ARM_EABI__
#      define DM_NO_ARM_EABI
#    endif
#  endif
#  if defined(__GXX_EXPERIMENTAL_CXX0X__)
#    if (__GNUC__ * 100 + __GNUC_MINOR__) >= 403
       /* C++0x features supported in GCC 4.3: */
#      define DM_COMPILER_RVALUE_REFS
#      define DM_COMPILER_DECLTYPE
#    endif
#    if (__GNUC__ * 100 + __GNUC_MINOR__) >= 404
       /* C++0x features supported in GCC 4.4: */
#      define DM_COMPILER_VARIADIC_TEMPLATES
#      define DM_COMPILER_AUTO_TYPE
#      define DM_COMPILER_EXTERN_TEMPLATES
#      define DM_COMPILER_DEFAULT_DELETE_MEMBERS
#      define DM_COMPILER_CLASS_ENUM
#      define DM_COMPILER_INITIALIZER_LISTS
#    endif
#    if (__GNUC__ * 100 + __GNUC_MINOR__) >= 405
       /* C++0x features supported in GCC 4.5: */
#      define DM_COMPILER_LAMBDA
#      define DM_COMPILER_UNICODE_STRINGS
#    endif
#    if (__GNUC__ * 100 + __GNUC_MINOR__) >= 406
       /* C++0x features supported in GCC 4.6: */
#      define DM_COMPILER_CONSTEXPR
#    endif

#  endif

/* IBM compiler versions are a bit messy. There are actually two products:
   the C product, and the C++ product. The C++ compiler is always packaged
   with the latest version of the C compiler. Version numbers do not always
   match. This little table (I'm not sure it's accurate) should be helpful:

   C++ product                C product

   C Set 3.1                  C Compiler 3.0
   ...                        ...
   C++ Compiler 3.6.6         C Compiler 4.3
   ...                        ...
   Visual Age C++ 4.0         ...
   ...                        ...
   Visual Age C++ 5.0         C Compiler 5.0
   ...                        ...
   Visual Age C++ 6.0         C Compiler 6.0

   Now:
   __xlC__    is the version of the C compiler in hexadecimal notation
              is only an approximation of the C++ compiler version
   __IBMCPP__ is the version of the C++ compiler in decimal notation
              but it is not defined on older compilers like C Set 3.1 */
#elif defined(__xlC__)
#  define DM_CC_XLC
#  define DM_FULL_TEMPLATE_INSTANTIATION
#  if __xlC__ < 0x400
#    define DM_NO_BOOL_TYPE
#    define DM_NO_EXPLICIT_KEYWORD
#    define DM_NO_USING_KEYWORD
#    define DM_TYPENAME
#    define DM_OUTOFLINE_TEMPLATE inline
#    define DM_BROKEN_TEMPLATE_SPECIALIZATION
#    define DM_CANNOT_DELETE_CONSTANT
#  elif __xlC__ >= 0x0600
#    define DM_ALIGNOF(type)     __alignof__(type)
#    define DM_TYPEOF(expr)      __typeof__(expr)
#    define DM_DECL_ALIGN(n)     __attribute__((__aligned__(n)))
#    define DM_PACKED            __attribute__((__packed__))
#  endif

/* Older versions of DEC C++ do not define __EDG__ or __EDG - observed
   on DEC C++ V5.5-004. New versions do define  __EDG__ - observed on
   Compaq C++ V6.3-002.
   This compiler is different enough from other EDG compilers to handle
   it separately anyway. */
#elif defined(__DECCXX) || defined(__DECC)
#  define DM_CC_DEC
/* Compaq C++ V6 compilers are EDG-based but I'm not sure about older
   DEC C++ V5 compilers. */
#  if defined(__EDG__)
#    define DM_CC_EDG
#  endif
/* Compaq have disabled EDG's _BOOL macro and use _BOOL_EXISTS instead
   - observed on Compaq C++ V6.3-002.
   In any case versions prior to Compaq C++ V6.0-005 do not have bool. */
#  if !defined(_BOOL_EXISTS)
#    define DM_NO_BOOL_TYPE
#  endif
/* Spurious (?) error messages observed on Compaq C++ V6.5-014. */
#  define DM_NO_USING_KEYWORD
/* Apply to all versions prior to Compaq C++ V6.0-000 - observed on
   DEC C++ V5.5-004. */
#  if __DECCXX_VER < 60060000
#    define DM_TYPENAME
#    define DM_BROKEN_TEMPLATE_SPECIALIZATION
#    define DM_CANNOT_DELETE_CONSTANT
#  endif
/* avoid undefined symbol problems with out-of-line template members */
#  define DM_OUTOFLINE_TEMPLATE inline

/* The Portland Group C++ compiler is based on EDG and does define __EDG__
   but the C compiler does not */
#elif defined(__PGI)
#  define DM_CC_PGI
#  if defined(__EDG__)
#    define DM_CC_EDG
#  endif

/* Compilers with EDG front end are similar. To detect them we test:
   __EDG documented by SGI, observed on MIPSpro 7.3.1.1 and KAI C++ 4.0b
   __EDG__ documented in EDG online docs, observed on Compaq C++ V6.3-002
   and PGI C++ 5.2-4 */
#elif !defined(DM_OS_HPUX) && (defined(__EDG) || defined(__EDG__))
#  define DM_CC_EDG
/* From the EDG documentation (does not seem to apply to Compaq C++):
   _BOOL
        Defined in C++ mode when bool is a keyword. The name of this
        predefined macro is specified by a configuration flag. _BOOL
        is the default.
   __BOOL_DEFINED
        Defined in Microsoft C++ mode when bool is a keyword. */
#  if !defined(_BOOL) && !defined(__BOOL_DEFINED)
#    define DM_NO_BOOL_TYPE
#  endif

/* The Comeau compiler is based on EDG and does define __EDG__ */
#  if defined(__COMO__)
#    define DM_CC_COMEAU
#    define DM_C_CALLBACKS

/* The `using' keyword was introduced to avoid KAI C++ warnings
   but it's now causing KAI C++ errors instead. The standard is
   unclear about the use of this keyword, and in practice every
   compiler is using its own set of rules. Forget it. */
#  elif defined(__KCC)
#    define DM_CC_KAI
#    define DM_NO_USING_KEYWORD

/* Using the `using' keyword avoids Intel C++ for Linux warnings */
#  elif defined(__INTEL_COMPILER)
#    define DM_CC_INTEL

/* Uses CFront, make sure to read the manual how to tweak templates. */
#  elif defined(__ghs)
#    define DM_CC_GHS

#  elif defined(__DCC__)
#    define DM_CC_DIAB
#    undef DM_NO_BOOL_TYPE
#    if !defined(__bool)
#      define DM_NO_BOOL_TYPE
#    endif

/* The UnixWare 7 UDK compiler is based on EDG and does define __EDG__ */
#  elif defined(__USLC__) && defined(__SCO_VERSION__)
#    define DM_CC_USLC
/* The latest UDK 7.1.1b does not need this, but previous versions do */
#    if !defined(__SCO_VERSION__) || (__SCO_VERSION__ < 302200010)
#      define DM_OUTOFLINE_TEMPLATE inline
#    endif
#    define DM_NO_USING_KEYWORD /* ### check "using" status */

/* Never tested! */
#  elif defined(CENTERLINE_CLPP) || defined(OBJECTCENTER)
#    define DM_CC_OC
#    define DM_NO_USING_KEYWORD

/* CDS++ defines __EDG__ although this is not documented in the Reliant
   documentation. It also follows conventions like _BOOL and this documented */
#  elif defined(sinix)
#    define DM_CC_CDS
#    define DM_NO_USING_KEYWORD

/* The MIPSpro compiler defines __EDG */
#  elif defined(__sgi)
#    define DM_CC_MIPS
#    define DM_NO_USING_KEYWORD /* ### check "using" status */
#    define DM_NO_TEMPLATE_FRIENDS
#    if defined(_COMPILER_VERSION) && (_COMPILER_VERSION >= 740)
#      define DM_OUTOFLINE_TEMPLATE inline
#      pragma set woff 3624,3625,3649 /* turn off some harmless warnings */
#    endif
#  endif

/* VxWorks' DIAB toolchain has an additional EDG type C++ compiler
   (see __DCC__ above). This one is for C mode files (__EDG is not defined) */
#elif defined(_DIAB_TOOL)
#  define DM_CC_DIAB

/* Never tested! */
#elif defined(__HIGHC__)
#  define DM_CC_HIGHC

#elif defined(__SUNPRO_CC) || defined(__SUNPRO_C)
#  define DM_CC_SUN
/* 5.0 compiler or better
    'bool' is enabled by default but can be disabled using -features=nobool
    in which case _BOOL is not defined
        this is the default in 4.2 compatibility mode triggered by -compat=4 */
#  if __SUNPRO_CC >= 0x500
#    define DM_NO_TEMPLATE_TEMPLATE_PARAMETERS
   /* see http://developers.sun.com/sunstudio/support/Ccompare.html */
#    if __SUNPRO_CC >= 0x590
#      define DM_ALIGNOF(type)   __alignof__(type)
#      define DM_TYPEOF(expr)    __typeof__(expr)
#      define DM_DECL_ALIGN(n)   __attribute__((__aligned__(n)))
#    endif
#    if __SUNPRO_CC >= 0x550
#      define DM_DECL_EXPORT     __global
#    endif
#    if __SUNPRO_CC < 0x5a0
#      define DM_NO_TEMPLATE_FRIENDS
#    endif
#    if !defined(_BOOL)
#      define DM_NO_BOOL_TYPE
#    endif
#    if defined(__SUNPRO_CC_COMPAT) && (__SUNPRO_CC_COMPAT <= 4)
#      define DM_NO_USING_KEYWORD
#    endif
#    define DM_C_CALLBACKS
/* 4.2 compiler or older */
#  else
#    define DM_NO_BOOL_TYPE
#    define DM_NO_EXPLICIT_KEYWORD
#    define DM_NO_USING_KEYWORD
#  endif

/* CDS++ does not seem to define __EDG__ or __EDG according to Reliant
   documentation but nevertheless uses EDG conventions like _BOOL */
#elif defined(sinix)
#  define DM_CC_EDG
#  define DM_CC_CDS
#  if !defined(_BOOL)
#    define DM_NO_BOOL_TYPE
#  endif
#  define DM_BROKEN_TEMPLATE_SPECIALIZATION

#elif defined(DM_OS_HPUX)
/* __HP_aCC was not defined in first aCC releases */
#  if defined(__HP_aCC) || __cplusplus >= 199707L
#    define DM_NO_TEMPLATE_FRIENDS
#    define DM_CC_HPACC
#    if __HP_aCC-0 < 060000
#      define DM_NO_TEMPLATE_TEMPLATE_PARAMETERS
#      define DM_DECL_EXPORT     __declspec(dllexport)
#      define DM_DECL_IMPORT     __declspec(dllimport)
#    endif
#    if __HP_aCC-0 >= 061200
#      define DM_DECL_ALIGN(n) __attribute__((aligned(n)))
#    endif
#    if __HP_aCC-0 >= 062000
#      define DM_DECL_EXPORT     __attribute__((visibility("default")))
#      define DM_DECL_HIDDEN     __attribute__((visibility("hidden")))
#      define DM_DECL_IMPORT     DM_DECL_EXPORT
#    endif
#  else
#    define DM_CC_HP
#    define DM_NO_BOOL_TYPE
#    define DM_FULL_TEMPLATE_INSTANTIATION
#    define DM_BROKEN_TEMPLATE_SPECIALIZATION
#    define DM_NO_EXPLICIT_KEYWORD
#  endif
#  define DM_NO_USING_KEYWORD /* ### check "using" status */

#elif defined(__WINSCW__) && !defined(DM_CC_NOKIAX86)
#  define DM_CC_NOKIAX86

#else
#  error "DM has not been tested with this compiler - talk to qt-bugs@trolltech.com"
#endif


#ifdef DM_CC_INTEL
#  if __INTEL_COMPILER < 1200
#    define DM_NO_TEMPLATE_FRIENDS
#  endif
#  if defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(__GXX_EXPERIMENTAL_CPP0X__)
#    if __INTEL_COMPILER >= 1100
#      define DM_COMPILER_RVALUE_REFS
#      define DM_COMPILER_EXTERN_TEMPLATES
#      define DM_COMPILER_DECLTYPE
#    elif __INTEL_COMPILER >= 1200
#      define DM_COMPILER_VARIADIC_TEMPLATES
#      define DM_COMPILER_AUTO_TYPE
#      define DM_COMPILER_DEFAULT_DELETE_MEMBERS
#      define DM_COMPILER_CLASS_ENUM
#      define DM_COMPILER_LAMBDA
#    endif
#  endif
#endif

#ifdef DM_CC_GNU
#    define DM_GCC_VERSION_AT_LEAST(x,y) (__GNUC__ > x || __GNUC__ == x && __GNUC_MINOR__ >= y)
#else
#    define DM_GCC_VERSION_AT_LEAST(x,y) 0
#endif

#ifndef DM_DECL_EXPORT
#  if defined(DM_OS_WIN)
#    define DM_DECL_EXPORT __declspec(dllexport)
#  elif defined(DM_VISIBILITY_AVAILABLE)
#    define DM_DECL_EXPORT __attribute__((visibility("default")))
#  endif
#  ifndef DM_DECL_EXPORT
#    define DM_DECL_EXPORT
#  endif
#endif
#ifndef DM_DECL_IMPORT
#  if defined(DM_OS_WIN)
#    define DM_DECL_IMPORT __declspec(dllimport)
#  else
#    define DM_DECL_IMPORT
#  endif
#endif

//defined(_USRDLL) || 
//build config add this
#if defined(DM_USRDLL)
#  define DM_DLL_EXPORT DM_DECL_EXPORT
#else
#  define DM_DLL_EXPORT DM_DECL_IMPORT
#endif

#if defined(DM_OS_WIN) && (defined(_WP8) || defined(WP8))
# define DM_OS_WP8
#endif

#define DM_EXPORT_TEMPLATE template class DM_DLL_EXPORT
#endif // PUBLIC_H
