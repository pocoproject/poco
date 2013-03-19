#include "JSONStream.h"

#ifdef JSON_STREAM
#include "JSONWorker.h"
#include "JSONValidator.h"


JSONStream::JSONStream(json_stream_callback_t call_p, json_stream_e_callback_t call_e, void * callbackIdentifier) json_nothrow : state(true), call(call_p), err_call(call_e), buffer(), callback_identifier(callbackIdentifier) {}

JSONStream::JSONStream(const JSONStream & orig) json_nothrow : state(orig.state), call(orig.call), err_call(orig.err_call), buffer(orig.buffer), callback_identifier(orig.callback_identifier){}

JSONStream & JSONStream::operator =(const JSONStream & orig) json_nothrow {
	err_call = orig.err_call;
    call = orig.call;
	state = orig.state;
    buffer = orig.buffer;
	callback_identifier = orig.callback_identifier;
    return *this;
}

#ifdef JSON_LIBRARY
    JSONStream & JSONStream::operator << (const json_char * str) json_nothrow {
#else
    JSONStream & JSONStream::operator << (const json_string & str) json_nothrow {
#endif
		if (state){
			buffer += str;
			parse();
		}
		return *this;
}


#define QUOTECASE_STREAM()\
    case JSON_TEXT('\"'):\
	   while (*(++p) != JSON_TEXT('\"')){\
		  if (json_unlikely(*p == JSON_TEXT('\0'))) return json_string::npos;\
	   }\
	   break;


#define NULLCASE_STREAM()\
    case JSON_TEXT('\0'):\
	   return json_string::npos;\


#define BRACKET_STREAM(left, right)\
    case left: {\
	   size_t brac = 1;\
	   while (brac){\
		  switch (*(++p)){\
			 case right:\
				--brac;\
				break;\
			 case left:\
				++brac;\
				break;\
			 QUOTECASE_STREAM()\
			 NULLCASE_STREAM()\
		  }\
	   }\
	   break;}\
    case right:\
	   return json_string::npos;

size_t JSONStream::FindNextRelevant(json_char ch, const json_string & value_t, const size_t pos) json_nothrow {
    const json_char * start = value_t.c_str();
    for (const json_char * p = start + pos; *p; ++p){
	   if (json_unlikely(*p == ch)) return p - start;
	   switch (*p){
			 BRACKET_STREAM(JSON_TEXT('['), JSON_TEXT(']'))
			 BRACKET_STREAM(JSON_TEXT('{'), JSON_TEXT('}'))
			 QUOTECASE_STREAM()
	   }
    };
    return json_string::npos;
}

#include <iostream>
void JSONStream::parse(void) json_nothrow {
    size_t pos = buffer.find_first_of(JSON_TEXT("{["));
    if (json_likely(pos != json_string::npos)){
	   size_t end = (buffer[pos] == JSON_TEXT('[')) ? FindNextRelevant(JSON_TEXT(']'), buffer, pos + 1) : FindNextRelevant(JSON_TEXT('}'), buffer, pos + 1);
	   if (end != json_string::npos){
		  START_MEM_SCOPE
			 JSONNode temp(JSONWorker::parse(buffer.substr(pos, end - pos + 1)));
			 #ifndef JSON_LIBRARY
				call(temp, getIdentifier());
			 #else
				call(&temp, getIdentifier());
			 #endif
		  END_MEM_SCOPE
		  json_string::iterator beginning = buffer.begin();
		  buffer.erase(beginning, beginning + end);
		  parse();
	   }
	   #ifdef JSON_SAFE
			else {
				//verify that what's in there is at least valid so far
				#ifndef JSON_VALIDATE
					#error In order to use safe mode and streams, JSON_VALIDATE needs to be defined			
				#endif
				
				json_auto<json_char> s;
				#if defined JSON_DEBUG || defined JSON_SAFE
					json_char c;
					s.set(JSONWorker::RemoveWhiteSpace(json_string(buffer.c_str() + pos), c, false));
				#else
					s.set(JSONWorker::RemoveWhiteSpace(json_string(buffer.c_str() + pos), false));
				#endif
				
				
				if (!JSONValidator::isValidPartialRoot(s.ptr)){
					if (err_call) err_call(getIdentifier());
					state = false;
				}
			}
	   #endif
    }
}

#endif
