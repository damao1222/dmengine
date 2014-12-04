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

#ifndef  DMCHAR_H
#define  DMCHAR_H

#include "dmnamespace.h"
#include "private/dmutilstring_p.h"

DM_BEGIN_NAMESPACE
#ifdef UNICODE
	typedef StdChar strchar;
#else
	typedef char strchar;
#endif

class DM_DECL_EXPORT Char
{
public:
	Char();
	Char(char c);
	Char(duchar c);
	Char(duint16 c);
	
	char toAscii() const;
	duint16 unicode() const;
	strchar toStrChar() const; 

	duint16 ucs;
};

inline duint16 Char::unicode() const 
{ return ucs; }

DM_END_NAMESPACE
#endif // DMCHAR_H