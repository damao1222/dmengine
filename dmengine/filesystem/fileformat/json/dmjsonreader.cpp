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

#include "dmjsonreader.h"
#include "dmfile.h"

DM_BEGIN_NAMESPACE

class JsonReaderPrivate
{
    enum TokenType
    {
        tokenEndOfStream = 0,
        tokenObjectBegin,
        tokenObjectEnd,
        tokenArrayBegin,
        tokenArrayEnd,
        tokenString,
        tokenNumber,
        tokenTrue,
        tokenFalse,
        tokenNull,
        tokenArraySeparator,
        tokenMemberSeparator,
        tokenComment,
        tokenError
    };

    class Token
    {
    public:
        TokenType type;
        const char* start;
        const char* end;
    };
public:
    JsonReaderPrivate();
    ~JsonReaderPrivate();

    bool parser(ByteBuffer *buffer);
	bool parser(void *buffer, dint64 size);

    const Variant& root() const { return m_root; }
    Variant getData(const UtilString &key);
    Variant getData(const UtilString &key, const Variant &defaultValue);

    void cleanup();

private:
    void skipSpaces();
    char getNextChar();
    bool readToken(Token &token);
    bool readValue();
    bool readObject(Token &token);
    bool readArray(Token &token);
    bool readString(Token &token);
    void readNumber();
    Variant* currentValue();
    void setCurrentValue(Variant *v) { m_currentValue = v; }
    bool decodeNumber(Token &token);
    bool decodeString(Token &token);
    bool decodeString(Token &token, UtilString &decoded);
    bool readString();
    bool match( const char * pattern, int patternLength );
    bool decodeUnicodeCodePoint(Token &token, const char *&current, const char * end, unsigned int &unicode);
    bool decodeUnicodeEscapeSequence(Token &token, const char *&current, const char *end, unsigned int &unicode);
    bool addError( const UtilString &message, Token &token, const char* extra = 0);
    bool recoverFromError(TokenType skipUntilToken);
    bool addErrorAndRecover(const UtilString &message, Token &token, TokenType skipUntilToken );

public:
    Variant m_root;
private:
    ByteBuffer *m_buffer;
    Variant *m_currentValue;
    const char *m_current;
    const char *m_begin;
    const char *m_end;
};


static inline bool in( char c, char c1, char c2, char c3, char c4 )
{
   return c == c1  ||  c == c2  ||  c == c3  ||  c == c4;
}

static inline bool in( char c, char c1, char c2, char c3, char c4, char c5 )
{
   return c == c1  ||  c == c2  ||  c == c3  ||  c == c4  ||  c == c5;
}

static inline std::string codePointToUTF8(unsigned int cp)
{
   std::string result;
   
   // based on description from http://en.wikipedia.org/wiki/UTF-8

   if (cp <= 0x7f) 
   {
      result.resize(1);
      result[0] = static_cast<char>(cp);
   } 
   else if (cp <= 0x7FF) 
   {
      result.resize(2);
      result[1] = static_cast<char>(0x80 | (0x3f & cp));
      result[0] = static_cast<char>(0xC0 | (0x1f & (cp >> 6)));
   } 
   else if (cp <= 0xFFFF) 
   {
      result.resize(3);
      result[2] = static_cast<char>(0x80 | (0x3f & cp));
      result[1] = 0x80 | static_cast<char>((0x3f & (cp >> 6)));
      result[0] = 0xE0 | static_cast<char>((0xf & (cp >> 12)));
   }
   else if (cp <= 0x10FFFF) 
   {
      result.resize(4);
      result[3] = static_cast<char>(0x80 | (0x3f & cp));
      result[2] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
      result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 12)));
      result[0] = static_cast<char>(0xF0 | (0x7 & (cp >> 18)));
   }

   return result;
}

JsonReaderPrivate::JsonReaderPrivate():
    m_buffer(NULL),
    m_current(NULL),
    m_begin(NULL),
    m_end(NULL)
{

}

JsonReaderPrivate::~JsonReaderPrivate()
{
    DM_SAFE_RELEASE(m_buffer);
}

bool JsonReaderPrivate::parser(ByteBuffer *buffer)
{
    if (buffer)
    {
        if (m_buffer != buffer)
        {
            cleanup();
            m_buffer = buffer;
            m_buffer->retain();
            m_current = m_begin = (const char*)buffer->data();
            m_end = m_begin + buffer->size();
            setCurrentValue(&m_root);
            return readValue();
        }
    }
    return false;
}

bool JsonReaderPrivate::parser(void *buffer, dint64 size)
{
	cleanup();
	m_current = m_begin = (const char*)buffer;
	m_end = m_begin + size;
	setCurrentValue(&m_root);
	return readValue();
}

Variant JsonReaderPrivate::getData(const UtilString &key)
{
    return m_root.value(key);
}

Variant JsonReaderPrivate::getData(const UtilString &key, const Variant &defaultValue)
{
    return m_root.value(key, defaultValue);
}

void JsonReaderPrivate::cleanup()
{
    DM_SAFE_RELEASE(m_buffer);
    m_root.clear();
}

void JsonReaderPrivate::skipSpaces()
{
   while (m_current != m_end)
   {
      char c = *m_current;
      if ( c == ' '  ||  c == '\t'  ||  c == '\r'  ||  c == '\n' )
         ++m_current;
      else
         break;
   }
}

char JsonReaderPrivate::getNextChar()
{
   if ( m_current == m_end )
      return 0;
   return *m_current++;
}

bool JsonReaderPrivate::readToken( Token &token )
{
   skipSpaces();
   token.start = m_current;
   char c = getNextChar();
   bool ok = true;
   switch ( c )
   {
   case '{':
      token.type = tokenObjectBegin;
      break;
   case '}':
      token.type = tokenObjectEnd;
      break;
   case '[':
      token.type = tokenArrayBegin;
      break;
   case ']':
      token.type = tokenArrayEnd;
      break;
   case '"':
      token.type = tokenString;
      ok = readString();
      break;
#if 0
   case '/':
      token.type = tokenComment;
      ok = readComment();
      break;
#endif
   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
   case '-':
      token.type = tokenNumber;
      readNumber();
      break;
   case 't':
      token.type = tokenTrue;
      ok = match( "rue", 3 );
      break;
   case 'f':
      token.type = tokenFalse;
      ok = match( "alse", 4 );
      break;
   case 'n':
      token.type = tokenNull;
      ok = match( "ull", 3 );
      break;
   case ',':
      token.type = tokenArraySeparator;
      break;
   case ':':
      token.type = tokenMemberSeparator;
      break;
   case 0:
      token.type = tokenEndOfStream;
      break;

   default:
      ok = false;
      break;
   }
   if ( !ok )
      token.type = tokenError;
   token.end = m_current;
   return true;
}

bool JsonReaderPrivate::readString()
{
   char c = 0;
   while ( m_current != m_end )
   {
      c = getNextChar();
      if ( c == '\\' )
         getNextChar();
      else if ( c == '"' )
         break;
   }
   return c == '"';
}

void JsonReaderPrivate::readNumber()
{
   while ( m_current != m_end )
   {
      if ( !(*m_current >= '0'  &&  *m_current <= '9')  &&
           !in( *m_current, '.', 'e', 'E', '+', '-' ) )
         break;
      ++m_current;
   }
}

bool JsonReaderPrivate::match( const char * pattern, int patternLength )
{
   if ( m_end - m_current < patternLength )
      return false;
   int index = patternLength;
   while ( index-- )
      if ( m_current[index] != pattern[index] )
         return false;
   m_current += patternLength;
   return true;
}

bool JsonReaderPrivate::readValue()
{
   Token token;
   readToken(token);
   bool successful = true;

   switch ( token.type )
   {
   case tokenObjectBegin:
      successful = readObject( token );
      break;
   case tokenArrayBegin:
      successful = readArray( token );
      break;
   case tokenNumber:
      successful = decodeNumber( token );
      break;
   case tokenString:
      successful = decodeString( token );
      break;
   case tokenTrue:
      *currentValue() = true;
      break;
   case tokenFalse:
      *currentValue() = false;
      break;
   case tokenNull:
      *currentValue() = Variant();
      break;
   default:
       {
            return addError( "Syntax error: value, object or array expected.", token );
       }
   }
   return successful;
}

bool JsonReaderPrivate::readObject(Token &token)
{
    Token tokenName;
    UtilString name;
    Variant* root = currentValue();

    while ( readToken( tokenName ) )
   {
      bool initialTokenOk = true;
      if  ( !initialTokenOk )
         break;
      if ( tokenName.type == tokenObjectEnd  &&  name.isEmpty() )  // empty object
         return true;
      if ( tokenName.type != tokenString )
         break;
      
      name.clear();
      name.setSharable(false);
      if ( !decodeString( tokenName, name ) )
         return recoverFromError(tokenObjectEnd);

      Token colon;
      if ( !readToken( colon ) ||  colon.type != tokenMemberSeparator )
      {
          return addErrorAndRecover( "Missing ':' after object member name", 
                                    colon, 
                                    tokenObjectEnd );
      }
      Variant v;
      setCurrentValue(&v);
      bool ok = readValue();
      
        if ( !ok ) // error already set
            return recoverFromError( tokenObjectEnd );
        else
            root->insert(name, v);

      Token comma;
      if ( !readToken( comma )
            ||  ( comma.type != tokenObjectEnd  &&  
                  comma.type != tokenArraySeparator &&
                  comma.type != tokenComment ) )
      {
          return addErrorAndRecover( "Missing ',' or '}' in object declaration", 
                                    comma, 
                                    tokenObjectEnd );
      }
      bool finalizeTokenOk = true;
      while ( comma.type == tokenComment &&
              finalizeTokenOk )
         finalizeTokenOk = readToken( comma );
      if ( comma.type == tokenObjectEnd )
         return true;
   }
    return addErrorAndRecover( "Missing '}' or object member name", 
                              tokenName, 
                              tokenObjectEnd );
}

bool JsonReaderPrivate::readArray(Token &token)
{
    Variant v;
    skipSpaces();
    if ( *m_current == ']' ) // empty array
    {
        Token endArray;
        readToken( endArray );
        return true;
    }

    Variant* root = currentValue();
    for (;;)
    {
        Variant v;
        setCurrentValue(&v);
        bool ok = readValue();
        root->append(v);

        if ( !ok ) // error already set
            return recoverFromError(tokenArrayEnd);

        Token token;
        // Accept Comment after last item in the array.
        ok = readToken( token );

        bool badTokenType = ( token.type != tokenArraySeparator  &&
                            token.type != tokenArrayEnd );
        if ( !ok  ||  badTokenType )
        {
            return addErrorAndRecover( "Missing ',' or ']' in array declaration", 
                                    token, 
                                    tokenArrayEnd );
        }
        if ( token.type == tokenArrayEnd )
            break;
    }
    return true;
}

Variant* JsonReaderPrivate::currentValue()
{
    return m_currentValue;
}

bool JsonReaderPrivate::decodeNumber(Token &token)
{
   const char * current = token.start;
   const char * end = token.end;
   UtilString decoded;
   decoded.setSharable(false);
   while ( current != end )
   {
      char c = *current++;
      decoded += c;
   }
   *currentValue() = decoded;
   return true;
}

bool JsonReaderPrivate::decodeString( Token &token )
{
   UtilString decoded;
   decoded.setSharable(false);
   if ( !decodeString( token, decoded ) )
      return false;
   *currentValue() = decoded;
   return true;
}

bool JsonReaderPrivate::decodeString( Token &token, UtilString &decoded )
{
   const char * current = token.start + 1; // skip '"'
   const char * end = token.end - 1;      // do not include '"'
   while ( current != end )
   {
      char c = *current++;
      if ( c == '"' )
         break;
      else if ( c == '\\' )
      {
         if ( current == end )
         {
             return addError( "Empty escape sequence in string", token, current );
         }
         char escape = *current++;
         switch ( escape )
         {
         case '"': decoded += '"'; break;
         case '/': decoded += '/'; break;
         case '\\': decoded += '\\'; break;
         case 'b': decoded += '\b'; break;
         case 'f': decoded += '\f'; break;
         case 'n': decoded += '\n'; break;
         case 'r': decoded += '\r'; break;
         case 't': decoded += '\t'; break;
         case 'u':
            {
               unsigned int unicode;
               if ( !decodeUnicodeCodePoint( token, current, end, unicode ) )
                  return false;
               decoded += codePointToUTF8(unicode);
            }
            break;
         default:
             {
                 return addError( "Bad escape sequence in string", token, current );
             }
         }
      }
      else
      {
         decoded += c;
      }
   }
   return true;
}

bool JsonReaderPrivate::decodeUnicodeCodePoint( Token &token, 
                                     const char *&current, 
                                     const char * end, 
                                     unsigned int &unicode )
{

   if ( !decodeUnicodeEscapeSequence( token, current, end, unicode ) )
      return false;
   if (unicode >= 0xD800 && unicode <= 0xDBFF)
   {
      // surrogate pairs
      if (end - current < 6)
         return addError( "additional six characters expected to parse unicode surrogate pair.", token, current );
      unsigned int surrogatePair;
      if (*(current++) == '\\' && *(current++)== 'u')
      {
         if (decodeUnicodeEscapeSequence( token, current, end, surrogatePair ))
         {
            unicode = 0x10000 + ((unicode & 0x3FF) << 10) + (surrogatePair & 0x3FF);
         } 
         else
            return false;
      } 
      else
         return addError( "expecting another \\u token to begin the second half of a unicode surrogate pair", token, current );
   }
   return true;
}

bool JsonReaderPrivate::decodeUnicodeEscapeSequence( Token &token, 
                                     const char *&current, 
                                     const char * end, 
                                     unsigned int &unicode )
{
   if ( end - current < 4 )
      return addError( "Bad unicode escape sequence in string: four digits expected.", token, current );
   unicode = 0;
   for ( int index =0; index < 4; ++index )
   {
      char c = *current++;
      unicode *= 16;
      if ( c >= '0'  &&  c <= '9' )
         unicode += c - '0';
      else if ( c >= 'a'  &&  c <= 'f' )
         unicode += c - 'a' + 10;
      else if ( c >= 'A'  &&  c <= 'F' )
         unicode += c - 'A' + 10;
      else
         return addError( "Bad unicode escape sequence in string: hexadecimal digit expected.", token, current );
   }
   return true;
}

bool JsonReaderPrivate::addError( const UtilString &message, 
                  Token &token,
                  const char* extra )
{
    DM_LOGW("%s  %d", message.toCharStr(), token.type);
    return false;
}

bool JsonReaderPrivate::recoverFromError( TokenType skipUntilToken )
{
   Token skip;
   for (;;)
   {
      if ( !readToken(skip) )
      {} // discard errors caused by recovery
      if ( skip.type == skipUntilToken  ||  skip.type == tokenEndOfStream )
         break;
   }

   return false;
}


bool JsonReaderPrivate::addErrorAndRecover(const UtilString &message, 
                            Token &token,
                            TokenType skipUntilToken )
{
   addError( message, token);
   return recoverFromError( skipUntilToken );
}

JsonReader::JsonReader():
    C_D(JsonReader)
{

}

JsonReader::~JsonReader()
{
    D_D(JsonReader);
}

dbool JsonReader::parse(const char *fileName)
{
    return parse(UrlString(fileName));
}

dbool JsonReader::parse(const UrlString &fileName)
{
    dbool ret = false;

    File file(fileName);
    if (!file.open())
        return false;

    dbyte *buf;
    dint64 size;

    if (!file.getAllDatas(buf, size))
    {
        file.close();
        return false;
    }

    if (buf)
    {
        ByteBuffer *buffer = new ByteBuffer(buf, size);
        ret = pdm->parser(buffer);
        buffer->release();
    }

    file.close();
    return ret;
}

dbool JsonReader::parse(void *pData, dint64 size)
{
	if (pData)
	{
		return pdm->parser(pData, size);
	}
	return false;
}

const Variant& JsonReader::root() const
{
    return pdm->root();
}
/*
Variant JsonReader::getData(const UtilString &key)
{
    return pdm->m_root.value(key);
}

Variant JsonReader::getData(const UtilString &key, const Variant &defaultValue)
{
    return pdm->m_root.value(key, defaultValue);
}
*/
void JsonReader::cleanup()
{
    pdm->cleanup();
}

DM_END_NAMESPACE

