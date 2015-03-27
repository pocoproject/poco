//
// Unicode.cpp
//
// $Id: //poco/1.4/Foundation/src/Unicode.cpp#2 $
//
// Library: Foundation
// Package: Text
// Module:  Unicode
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Unicode.h"


//
// PCRE Unicode character database (UCD)
// Taken from pcre_internal.h
//


typedef Poco::UInt8 pcre_uint8;
typedef Poco::UInt16 pcre_uint16;
typedef Poco::Int32 pcre_int32;
typedef Poco::UInt32 pcre_uint32;

typedef struct {
  pcre_uint8 script;     /* ucp_Arabic, etc. */
  pcre_uint8 chartype;   /* ucp_Cc, etc. (general categories) */
  pcre_uint8 gbprop;     /* ucp_gbControl, etc. (grapheme break property) */
  pcre_uint8 caseset;    /* offset to multichar other cases or zero */
  pcre_int32 other_case; /* offset to other case, or zero if none */
} ucd_record;

extern "C" const pcre_uint32 _pcre_ucd_caseless_sets[];
extern "C" const ucd_record  _pcre_ucd_records[];
extern "C" const pcre_uint8  _pcre_ucd_stage1[];
extern "C" const pcre_uint16 _pcre_ucd_stage2[];
extern "C" const pcre_uint32 _pcre_ucp_gentype[];
extern "C" const pcre_uint32 _pcre_ucp_gbtable[];

#define UCD_BLOCK_SIZE 128
#define GET_UCD(ch) (_pcre_ucd_records + \
        _pcre_ucd_stage2[_pcre_ucd_stage1[(int)(ch) / UCD_BLOCK_SIZE] * \
        UCD_BLOCK_SIZE + (int)(ch) % UCD_BLOCK_SIZE])

#define UCD_CHARTYPE(ch)    GET_UCD(ch)->chartype
#define UCD_SCRIPT(ch)      GET_UCD(ch)->script
#define UCD_CATEGORY(ch)    _pcre_ucp_gentype[UCD_CHARTYPE(ch)]
#define UCD_GRAPHBREAK(ch)  GET_UCD(ch)->gbprop
#define UCD_CASESET(ch)     GET_UCD(ch)->caseset
#define UCD_OTHERCASE(ch)   ((pcre_uint32)((int)ch + (int)(GET_UCD(ch)->other_case)))


namespace Poco {


void Unicode::properties(int ch, CharacterProperties& props)
{
	if (ch > UCP_MAX_CODEPOINT) ch = 0;
	const ucd_record* ucd = GET_UCD(ch);
	props.category = static_cast<CharacterCategory>(_pcre_ucp_gentype[ucd->chartype]);
	props.type     = static_cast<CharacterType>(ucd->chartype);
	props.script   = static_cast<Script>(ucd->script);
}


int Unicode::toLower(int ch)
{
	if (isUpper(ch))
		return static_cast<int>(UCD_OTHERCASE(static_cast<unsigned>(ch)));
	else
		return ch;
}


int Unicode::toUpper(int ch)
{
	if (isLower(ch))
		return static_cast<int>(UCD_OTHERCASE(static_cast<unsigned>(ch)));
	else
		return ch;
}


} // namespace Poco
