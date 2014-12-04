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

#include "dmjsonwriter.h"
#include "dmlogger.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include "dmvariant.h"
#include "dmutilstring.h"
#include <sstream>
#include <vector>
#include "dmfile.h"

DM_BEGIN_NAMESPACE

class JsonWriterPrivate
{
public:
    JsonWriterPrivate(const std::string &indentation="\t" );
    ~JsonWriterPrivate(){}

public:
    /** \brief Serialize a Value in <a HREF="http://www.json.org">JSON</a> format.
    * \param out Stream to write to. (Can be ostringstream, e.g.)
    * \param root Value to serialize.
    * \note There is no point in deriving from Writer, since write() should not return a value.
    */
    void write(std::ostream &out, const Variant &root);
    void write(const char *filePath, const Variant &root);
    void write(File &file, const Variant &root);

private:
    void writeValue( const Variant &value );
    void writeArrayValue( const Variant &value );
    bool isMultineArray( const Variant &value );
    void pushValue( const std::string &value );
    void writeIndent();
    void writeWithIndent( const std::string &value );
    void indent();
    void unindent();
    void writeCommentBeforeValue( const Variant &root );
    void writeCommentAfterValueOnSameLine( const Variant &root );
    bool hasCommentForValue( const Variant &value );
    static std::string normalizeEOL( const std::string &text );

    typedef std::vector<std::string> ChildValues;

private:
    ChildValues childValues_;
    std::ostream* document_;
    std::string indentString_;
    int rightMargin_;
    std::string indentation_;
    bool addChildValues_;
};


/// Returns true if ch is a control character (in range [0,32[).
static inline bool 
isControlCharacter(char ch)
{
   return ch > 0 && ch <= 0x1F;
}

static bool containsControlCharacter( const char* str )
{
   while ( *str ) 
   {
      if ( isControlCharacter( *(str++) ) )
         return true;
   }
   return false;
}

std::string valueToQuotedString( const char *value )
{
   // Not sure how to handle unicode...
   if (strpbrk(value, "\"\\\b\f\n\r\t") == NULL && !containsControlCharacter( value ))
      return std::string("\"") + value + "\"";
   // We have to walk value and escape any special characters.
   // Appending to std::string is not efficient, but this should be rare.
   // (Note: forward slashes are *not* rare, but I am not escaping them.)
   std::string::size_type maxsize = strlen(value)*2 + 3; // allescaped+quotes+NULL
   std::string result;
   result.reserve(maxsize); // to avoid lots of mallocs
   result += "\"";
   for (const char* c=value; *c != 0; ++c)
   {
      switch(*c)
      {
         case '\"':
            result += "\\\"";
            break;
         case '\\':
            result += "\\\\";
            break;
         case '\b':
            result += "\\b";
            break;
         case '\f':
            result += "\\f";
            break;
         case '\n':
            result += "\\n";
            break;
         case '\r':
            result += "\\r";
            break;
         case '\t':
            result += "\\t";
            break;
         //case '/':
            // Even though \/ is considered a legal escape in JSON, a bare
            // slash is also legal, so I see no reason to escape it.
            // (I hope I am not misunderstanding something.
            // blep notes: actually escaping \/ may be useful in javascript to avoid </ 
            // sequence.
            // Should add a flag to allow this compatibility mode and prevent this 
            // sequence from occurring.
         default:
            if ( isControlCharacter( *c ) )
            {
               std::ostringstream oss;
               oss << "\\u" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << static_cast<int>(*c);
               result += oss.str();
            }
            else
            {
               result += *c;
            }
            break;
      }
   }
   result += "\"";
   return result;
}

JsonWriterPrivate::JsonWriterPrivate(const std::string &indentation /*= "\t"*/)
   : document_(NULL)
   , rightMargin_( 74 )
   , indentation_( indentation )
{
}


void JsonWriterPrivate::write( std::ostream &out, const Variant &root )
{
   document_ = &out;
   addChildValues_ = false;
   indentString_ = "";
   writeCommentBeforeValue( root );
   writeValue( root );
   writeCommentAfterValueOnSameLine( root );
   *document_ << "\n";
   document_ = NULL; // Forget the stream, for safety.
}

void JsonWriterPrivate::write( const char *filePath, const Variant &root )
{
    std::ofstream file;
    file.open(filePath);		
    write(file, root);
    file.close();
}

void JsonWriterPrivate::write(File &file, const Variant &root)
{
    std::ostringstream out;
    document_ = &out;
    addChildValues_ = false;
    indentString_ = "";
    writeCommentBeforeValue( root );
    writeValue( root );
    writeCommentAfterValueOnSameLine( root );
    *document_ << "\n";
    document_ = NULL; // Forget the stream, for safety.
    
    const std::string &str = out.str();
    file.write(str.c_str(), str.length());
}

void JsonWriterPrivate::writeValue( const Variant &value )
{
   switch ( value.type() )
   {
   case Variant::eInvalid:
      pushValue( "null" );
      break;
   case Variant::eChar:
   case Variant::eLongLong:
   case Variant::eInt:
      pushValue( value.toString().asStdString() );
      break;
   case Variant::eULongLong:
   case Variant::eUInt:
      pushValue( value.toString().asStdString() );
      break;
   case Variant::eFloat:
   case Variant::eDouble:
      pushValue( value.toString().asStdString() );
      break;
   case Variant::eString:
      pushValue( valueToQuotedString(value.toString().toCharStr()));
      break;
   case Variant::eBool:
      pushValue( value.toString().asStdString() );
      break;
   case Variant::eRect:
   case Variant::eRectF:
   case Variant::eCircle:
   case Variant::eCircleF:
   case Variant::eSize:
   case Variant::eSizeF:
   case Variant::eLine:
   case Variant::eLineF:
   case Variant::ePoint:
   case Variant::ePointF:
    {
        pushValue( valueToQuotedString(value.toString().toCharStr()) );
        break;
    }
   case Variant::eVarList:
      writeArrayValue( value);
      break;
   case Variant::eVarHash:
      {
        VarHash hash = value.toHash();
        if (hash.isEmpty())
            pushValue( "{}" );
        else
        {
            writeWithIndent( "{" );
            indent();
            VarHash::Iterator it = hash.begin();
            for (;;)
            {
                //const std::string &name = it.key().stdString();
                const UtilString &name = it.key();
                const Variant &childValue = it.value();

                writeCommentBeforeValue( childValue );
                writeWithIndent( valueToQuotedString( name.toCharStr() ) );
                *document_ << " : ";
                writeValue( childValue );
                if ( ++it == hash.constEnd() )
                {
                    writeCommentAfterValueOnSameLine( childValue );
                    break;
                }
                *document_ << ",";
                writeCommentAfterValueOnSameLine( childValue );
            }
            unindent();
            writeWithIndent( "}" );
        }
      }
      break;
   default:
       DM_FATAL("Unhandle Type");
   }
}


void JsonWriterPrivate::writeArrayValue( const Variant &value )
{
   unsigned size = value.size();
   if ( size == 0 )
      pushValue( "[]" );
   else
   {
      bool isArrayMultiLine = isMultineArray( value );
      if ( isArrayMultiLine )
      {
         writeWithIndent( "[" );
         indent();
         bool hasChildValue = !childValues_.empty();
         unsigned index =0;
         for (;;)
         {
            const Variant &childValue = value[index];
            writeCommentBeforeValue( childValue );
            if ( hasChildValue )
               writeWithIndent( childValues_[index] );
            else
            {
               writeIndent();
               writeValue( childValue );
            }
            if ( ++index == size )
            {
               writeCommentAfterValueOnSameLine( childValue );
               break;
            }
            *document_ << ",";
            writeCommentAfterValueOnSameLine( childValue );
         }
         unindent();
         writeWithIndent( "]" );
      }
      else // output on a single line
      {
         assert( childValues_.size() == size );
         *document_ << "[ ";
         for ( unsigned index =0; index < size; ++index )
         {
            if ( index > 0 )
               *document_ << ", ";
            *document_ << childValues_[index];
         }
         *document_ << " ]";
      }
   }
}


bool JsonWriterPrivate::isMultineArray( const Variant &value )
{
   int size = value.size();
   bool isMultiLine = size*3 >= rightMargin_ ;
   childValues_.clear();
   for ( int index =0; index < size  &&  !isMultiLine; ++index )
   {
      const Variant &childValue = value[index];
      isMultiLine = isMultiLine  ||
                     ( (childValue.isList()  ||  childValue.isHash())  &&  
                        childValue.size() > 0 );
   }
   if ( !isMultiLine ) // check if line length > max line length
   {
      childValues_.reserve( size );
      addChildValues_ = true;
      int lineLength = 4 + (size-1)*2; // '[ ' + ', '*n + ' ]'
      for ( int index =0; index < size  &&  !isMultiLine; ++index )
      {
         writeValue( value[index] );
         lineLength += int( childValues_[index].length() );
         isMultiLine = isMultiLine  &&  hasCommentForValue( value[index] );
      }
      addChildValues_ = false;
      isMultiLine = isMultiLine  ||  lineLength >= rightMargin_;
   }
   return isMultiLine;
}


void JsonWriterPrivate::pushValue( const std::string &value )
{
   if ( addChildValues_ )
      childValues_.push_back( value );
   else
      *document_ << value;
}


void JsonWriterPrivate::writeIndent()
{
  /*
    Some comments in this method would have been nice. ;-)

   if ( !document_.empty() )
   {
      char last = document_[document_.length()-1];
      if ( last == ' ' )     // already indented
         return;
      if ( last != '\n' )    // Comments may add new-line
         *document_ << '\n';
   }
  */
   *document_ << '\n' << indentString_;
}


void JsonWriterPrivate::writeWithIndent( const std::string &value )
{
   writeIndent();
   *document_ << value;
}


void JsonWriterPrivate::indent()
{
   indentString_ += indentation_;
}


void JsonWriterPrivate::unindent()
{
   assert( indentString_.size() >= indentation_.size() );
   indentString_.resize( indentString_.size() - indentation_.size() );
}


void JsonWriterPrivate::writeCommentBeforeValue( const Variant &root )
{
    /*
   if ( !root.hasComment( commentBefore ) )
      return;
   *document_ << normalizeEOL( root.getComment( commentBefore ) );
   *document_ << "\n";
   */
}


void JsonWriterPrivate::writeCommentAfterValueOnSameLine( const Variant &root )
{
    /*
   if ( root.hasComment( commentAfterOnSameLine ) )
      *document_ << " " + normalizeEOL( root.getComment( commentAfterOnSameLine ) );

   if ( root.hasComment( commentAfter ) )
   {
      *document_ << "\n";
      *document_ << normalizeEOL( root.getComment( commentAfter ) );
      *document_ << "\n";
   }
   */
}


bool JsonWriterPrivate::hasCommentForValue( const Variant &value )
{
    /*
   return value.hasComment( commentBefore )
          ||  value.hasComment( commentAfterOnSameLine )
          ||  value.hasComment( commentAfter );
          */
    return false;
}


std::string JsonWriterPrivate::normalizeEOL( const std::string &text )
{
   std::string normalized;
   normalized.reserve( text.length() );
   const char *begin = text.c_str();
   const char *end = begin + text.length();
   const char *current = begin;
   while ( current != end )
   {
      char c = *current++;
      if ( c == '\r' ) // mac or dos EOL
      {
         if ( *current == '\n' ) // convert dos EOL
            ++current;
         normalized += '\n';
      }
      else // handle unix EOL & other char
         normalized += c;
   }
   return normalized;
}


JsonWriter::JsonWriter(const UtilString &indentation /*= "\t"*/):
    C_D1(JsonWriter, indentation.stdString())
{

}

JsonWriter::~JsonWriter()
{
    D_D(JsonWriter);
}

void JsonWriter::write(const char *filePath, const Variant &root)
{
    File file(filePath);
    file.open(eO_ForceWrite);
    pdm->write(file, root);
    file.close();
    //pdm->write(filePath, root);
}

std::string JsonWriter::toString(const Variant &root)
{
    JsonWriter write;
    std::ostringstream out;
    write.d_func()->write(out, root);
    return out.str();
}
DM_END_NAMESPACE

