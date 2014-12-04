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
#ifndef DMFLAG_H
#define DMFLAG_H

#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT Flag
{
    int i;
public:
    inline Flag(int i);
    inline operator int() const { return i; }
};

inline Flag::Flag(int ai) : i(ai) {}

template<typename Enum>
class DM_DLL_EXPORT Flags
{
    typedef void **Zero;
    int i;
public:
    typedef Enum enum_type;
    inline Flags(const Flags &f) : i(f.i) {}
    inline Flags(Enum f) : i(f) {}
    inline Flags(Zero = 0) : i(0) {}
    inline Flags(Flag f) : i(f) {}

    inline Flags &operator=(const Flags &f) { i = f.i; return *this; }
    inline Flags &operator&=(int mask) { i &= mask; return *this; }
    inline Flags &operator&=(uint32 mask) { i &= mask; return *this; }
    inline Flags &operator|=(Flags f) { i |= f.i; return *this; }
    inline Flags &operator|=(Enum f) { i |= f; return *this; }
    inline Flags &operator^=(Flags f) { i ^= f.i; return *this; }
    inline Flags &operator^=(Enum f) { i ^= f; return *this; }

    inline operator int() const { return i; }

    inline Flags operator|(Flags f) const { Flags g; g.i = i | f.i; return g; }
    inline Flags operator|(Enum f) const { Flags g; g.i = i | f; return g; }
    inline Flags operator^(Flags f) const { Flags g; g.i = i ^ f.i; return g; }
    inline Flags operator^(Enum f) const { Flags g; g.i = i ^ f; return g; }
    inline Flags operator&(int mask) const { Flags g; g.i = i & mask; return g; }
    inline Flags operator&(uint32 mask) const { Flags g; g.i = i & mask; return g; }
    inline Flags operator&(Enum f) const { Flags g; g.i = i & f; return g; }
    inline Flags operator~() const { Flags g; g.i = ~i; return g; }

    inline boolean operator!() const { return !i; }

    inline boolean testFlag(Enum f) const { return (i & f) == f && (f != 0 || i == int(f) ); }
};
DM_END_NAMESPACE
#define DM_DECLARE_FLAGS(flags, Enum)\
typedef Flags<Enum> flags;
#define DM_DECLARE_OPERATORS_FOR_FLAGS(flags) \
inline Flags<flags::enum_type> operator|(flags::enum_type f1, flags::enum_type f2) \
{ return Flags<flags::enum_type>(f1) | f2; } \
inline Flags<flags::enum_type> operator|(flags::enum_type f1, Flags<flags::enum_type> f2) \
{ return f2 | f1; }


#endif // DMFLAG_H

