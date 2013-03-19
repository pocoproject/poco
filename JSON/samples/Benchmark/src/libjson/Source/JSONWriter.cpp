#include "JSONNode.h"
#ifdef JSON_WRITE_PRIORITY
#include "JSONWorker.h"
#include "JSONGlobals.h"

extern bool used_ascii_one;

#ifdef JSON_INDENT
    inline json_string makeIndent(unsigned int amount) json_nothrow json_write_priority;
    inline json_string makeIndent(unsigned int amount) json_nothrow {
	   if (amount == 0xFFFFFFFF) return json_global(EMPTY_JSON_STRING);
	   json_string result;
	   result.reserve(amount * json_global(INDENT).length());
	   for(unsigned int i = 0; i < amount; ++i){
		  result += json_global(INDENT);
	   }
	   JSON_ASSERT(result.capacity == amount * json_global(INDENT).length(), JSON_TEXT("makeIndent made a string too big"));
	   return result;
    }
#else
    inline json_string makeIndent(unsigned int amount) json_nothrow {
	   if (amount == 0xFFFFFFFF) return json_global(EMPTY_JSON_STRING);
		if (json_likely(amount < 8)){
			static json_string cache[] = {
				json_string(),
				json_string(JSON_TEXT("\t")),
				json_string(JSON_TEXT("\t\t")),
				json_string(JSON_TEXT("\t\t\t")),
				json_string(JSON_TEXT("\t\t\t\t")),
				json_string(JSON_TEXT("\t\t\t\t\t")),
				json_string(JSON_TEXT("\t\t\t\t\t\t")),
				json_string(JSON_TEXT("\t\t\t\t\t\t\t"))
			};
			return cache[amount];
		}
	   return json_string(amount, JSON_TEXT('\t'));
    }
#endif

json_string internalJSONNode::WriteName(bool formatted, bool arrayChild) const json_nothrow {
    if (arrayChild){
	   return json_global(EMPTY_JSON_STRING) ;
    } else {
	   return json_string(JSON_TEXT("\"")) + JSONWorker::UnfixString(_name, _name_encoded) + ((formatted) ? JSON_TEXT("\" : ") : JSON_TEXT("\":"));
    }
}

json_string internalJSONNode::WriteChildren(unsigned int indent) const json_nothrow {
    //Iterate through the children and write them
    if (json_likely(CHILDREN -> empty())) return json_global(EMPTY_JSON_STRING);

    json_string res;

    json_string indent_plus_one;
    //handle whether or not it's formatted JSON
    if (indent != 0xFFFFFFFF){  //it's formatted, make the indentation strings
	   indent_plus_one = json_global(NEW_LINE) + makeIndent(++indent);
    }

    //else it's not formatted, leave the indentation strings empty
    const size_t size_minus_one = CHILDREN -> size() - 1;
    size_t i = 0;
    JSONNode ** it = CHILDREN -> begin();
    for(JSONNode ** it_end = CHILDREN -> end(); it != it_end; ++it, ++i){

	   res += indent_plus_one + (*it) -> internal -> Write(indent, type() == JSON_ARRAY);
	   if (json_likely(i < size_minus_one)) res += JSON_TEXT(",");  //the last one does not get a comma, but all of the others do
    }
    if (indent != 0xFFFFFFFF){
	   return res + json_global(NEW_LINE) + makeIndent(indent - 1);
    }
    return res;
}

#ifdef JSON_ARRAY_SIZE_ON_ONE_LINE
    json_string internalJSONNode::WriteChildrenOneLine(unsigned int indent) const json_nothrow {
	   //Iterate through the children and write them
	   if (json_likely(CHILDREN -> empty())) return json_global(EMPTY_JSON_STRING);
	   if ((*CHILDREN -> begin()) -> internal -> isContainer()) return WriteChildren(indent);

	   json_string res;
	   json_string comma(JSON_TEXT(","));
	   if (indent != 0xFFFFFFFF){
		  comma += JSON_TEXT(' ');
	   }

	   //else it's not formatted, leave the indentation strings empty
	   const size_t size_minus_one = CHILDREN -> size() - 1;
	   size_t i = 0;
	   JSONNode ** it = CHILDREN -> begin();
	   for(JSONNode ** it_end = CHILDREN -> end(); it != it_end; ++it, ++i){
		  res += (*it) -> internal -> Write(indent, type() == JSON_ARRAY);
		  if (json_likely(i < size_minus_one)) res += comma;  //the last one does not get a comma, but all of the others do
	   }
	   return res;
    }
#endif

#ifdef JSON_COMMENTS
    json_string internalJSONNode::WriteComment(unsigned int indent) const json_nothrow {
	   if (indent == 0xFFFFFFFF) return json_global(EMPTY_JSON_STRING);
	   if (json_likely(_comment.empty())) return json_global(EMPTY_JSON_STRING);
	   size_t pos = _comment.find(JSON_TEXT('\n'));
		
		const json_string current_indent(json_global(NEW_LINE) + makeIndent(indent));
		
	   if (json_likely(pos == json_string::npos)){  //Single line comment
		  return current_indent + json_global(SINGLELINE_COMMENT) + _comment + current_indent;
	   }

	   /*
	    Multiline comments
	    */
	   #if defined(JSON_WRITE_BASH_COMMENTS) || defined(JSON_WRITE_SINGLE_LINE_COMMENTS)
		  json_string result(current_indent);
	   #else
		  const json_string current_indent_plus_one(json_global(NEW_LINE) + makeIndent(indent + 1));
		  json_string result(current_indent + JSON_TEXT("/*") + current_indent_plus_one);
	   #endif
	   size_t old = 0;
	   while(pos != json_string::npos){
		  if (json_unlikely(pos && _comment[pos - 1] == JSON_TEXT('\r'))) --pos;
		  #if defined(JSON_WRITE_BASH_COMMENTS) || defined(JSON_WRITE_SINGLE_LINE_COMMENTS)
			 result += json_global(SINGLELINE_COMMENT);
		  #endif
		  result.append(_comment.begin() + old, _comment.begin() + pos);
		  
		  #if defined(JSON_WRITE_BASH_COMMENTS) || defined(JSON_WRITE_SINGLE_LINE_COMMENTS)
			 result += current_indent;
		  #else
			 result += current_indent_plus_one;
		  #endif
		  old = (_comment[pos] == JSON_TEXT('\r')) ? pos + 2 : pos + 1;
		  pos = _comment.find(JSON_TEXT('\n'), old);
	   }
	   #if defined(JSON_WRITE_BASH_COMMENTS) || defined(JSON_WRITE_SINGLE_LINE_COMMENTS)
		  result += json_global(SINGLELINE_COMMENT);
	   #endif
	   result.append(_comment.begin() + old, _comment.end());
		result += current_indent;
	   #if defined(JSON_WRITE_BASH_COMMENTS) || defined(JSON_WRITE_SINGLE_LINE_COMMENTS)
		  return result;
	   #else
		  return result + JSON_TEXT("*/") + current_indent;
	   #endif
    }
#else
    inline json_string internalJSONNode::WriteComment(unsigned int) const json_nothrow {
	   return json_global(EMPTY_JSON_STRING);
    }
#endif

#ifdef JSON_LESS_MEMORY
    #define STRING_RETURN(x) return (_string = (x))
#else
    #define STRING_RETURN(x) return x
#endif
#include "NumberToString.h"  //So that I can convert numbers into strings
json_string internalJSONNode::getValueAsString(void) const json_nothrow{
    if (!_string.empty()) return _string;
    switch(_type){
	   case JSON_NUMBER:
		  STRING_RETURN(NumberToString::_ftoa(_value._number));
	   case JSON_NULL:
		  STRING_RETURN(json_global(CONST_NULL));
	   case JSON_BOOL:
		  STRING_RETURN(_value._bool ? json_global(CONST_TRUE) : json_global(CONST_FALSE));
    }
    
    JSON_FAIL_SAFE(JSON_TEXT("Calling getValueAsString on the wrong type"), return json_global(CONST_NULL););
}

json_string internalJSONNode::Write(unsigned int indent, bool arrayChild) const json_nothrow {
    const bool formatted = indent != 0xFFFFFFFF;

    #if !defined(JSON_PREPARSE) && defined(JSON_READ_PRIORITY)
	   if (!(formatted || fetched)){  //It's not formatted or fetched, just do a raw dump
          //first remove the \1 characters
		   if (used_ascii_one){  //if it hasn't been used yet, don't bother checking
			  json_string result(_string);
			  for(json_string::iterator beg = result.begin(), en = result.end(); beg != en; ++beg){
				  if (*beg == JSON_TEXT('\1')) *beg = JSON_TEXT('\"');
			  }
			  return WriteComment(indent) + WriteName(false, arrayChild) + result;
		   }
		  return WriteComment(indent) + WriteName(false, arrayChild) + _string;
	   }
    #endif

    //It's either formatted or fetched
    switch (_type){
	   case JSON_NODE:   //got members, write the members
		  Fetch();
            return WriteComment(indent) + WriteName(formatted, arrayChild) + JSON_TEXT("{") + WriteChildren(indent) + JSON_TEXT("}");
	   case JSON_ARRAY:	   //write out the child nodes int he array
		  Fetch();
		  #ifdef JSON_ARRAY_SIZE_ON_ONE_LINE
			 if (size() <= JSON_ARRAY_SIZE_ON_ONE_LINE){
				return WriteComment(indent) + WriteName(formatted, arrayChild) + JSON_TEXT("[") + WriteChildrenOneLine(indent) + JSON_TEXT("]");
			 }
		  #endif
            return WriteComment(indent) + WriteName(formatted, arrayChild) + JSON_TEXT("[") + WriteChildren(indent) + JSON_TEXT("]");
	   case JSON_NUMBER:   //write out a literal, without quotes
	   case JSON_NULL:
	   case JSON_BOOL:
            return WriteComment(indent) + WriteName(formatted, arrayChild) + _string;
    }

    JSON_ASSERT_SAFE(type() == JSON_STRING, JSON_TEXT("Writing an unknown JSON node type"), return json_global(EMPTY_JSON_STRING););
    //If it go here, then it's a json_string
    #if !defined(JSON_PREPARSE) && defined(JSON_READ_PRIORITY)
	   if (json_likely(fetched)) return WriteComment(indent) + WriteName(formatted, arrayChild) + JSON_TEXT("\"") + JSONWorker::UnfixString(_string, _string_encoded) + JSON_TEXT("\"");  //It's already been fetched, meaning that it's unescaped
	   return WriteComment(indent) + WriteName(formatted, arrayChild) + _string;  //it hasn't yet been fetched, so it's already unescaped, just do a dump
    #else
	   return WriteComment(indent) + WriteName(formatted, arrayChild) + JSON_TEXT("\"") + JSONWorker::UnfixString(_string, _string_encoded) + JSON_TEXT("\"");
    #endif
}
#endif
