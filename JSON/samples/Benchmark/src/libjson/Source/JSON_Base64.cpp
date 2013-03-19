#include "JSON_Base64.h"
#include "JSONGlobals.h"

#if defined(JSON_BINARY) || defined(JSON_EXPOSE_BASE64)  //if this is not needed, don't waste space and time compiling it

static const json_char * chars64(JSON_TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"));

json_string JSONBase64::json_encode64(const unsigned char * binary, size_t bytes) json_nothrow {
    size_t misaligned = bytes % 3;
    json_string result;
    result.reserve((size_t)(((float)bytes) * 1.37f) + 4);

    //do all of the ones that are 3 byte aligned
    for (size_t i = 0, aligned((bytes - misaligned) / 3); i < aligned; ++i){
	   result += chars64[(binary[0] & 0xFC) >> 2];
	   result += chars64[((binary[0] & 0x03) << 4) + ((binary[1] & 0xF0) >> 4)];
	   result += chars64[((binary[1] & 0x0F) << 2) + ((binary[2] & 0xC0) >> 6)];
	   result += chars64[binary[2] & 0x3F];
	   binary += 3;
    }

    if (json_likely(misaligned != 0)){
	   //copy the rest into a temporary buffer
	   unsigned char temp[3];
	   for (unsigned int i = 0; i < misaligned; ++i){
		  temp[i] = *binary++;
	   }
	   for (unsigned int i = (unsigned int)misaligned; i < 3; ++i){
		  temp[i] = '\0';
	   }

	   //now do the final three bytes
	   result += chars64[(temp[0] & 0xFC) >> 2];
	   result += chars64[((temp[0] & 0x03) << 4) + ((temp[1] & 0xF0) >> 4)];
	   if (misaligned == 2){
		  result += chars64[((temp[1] & 0x0F) << 2) + ((temp[2] & 0xC0) >> 6)];
		  result += JSON_TEXT('=');
	   } else {
		  result += JSON_TEXT("==");
	   }
    }
    JSON_ASSERT((size_t)(((float)bytes) * 1.37f) + 4 >= result.length(), JSON_TEXT("Didn't reserve enough space for a one-time go"));
    return result;
}


//define it first for the pure qualifier
inline json_uchar toBinary(json_uchar c) json_pure;
inline json_uchar toBinary(json_uchar c) json_nothrow {
    JSON_ASSERT_SAFE(c >= 43, json_global(ERROR_LOWER_RANGE) + JSON_TEXT("base64"), return JSON_TEXT('0'););
    JSON_ASSERT_SAFE(c <= 122, json_global(ERROR_UPPER_RANGE) + JSON_TEXT("base64"), return JSON_TEXT('0'););
	static json_uchar binaryConvert[80] = {62,48,49,50,63,52,53,54,55,56,57,58,59,60,61,249,250,251,252,253,254,255,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
    return binaryConvert[c - 43];
}

std::string JSONBase64::json_decode64(const json_string & encoded) json_nothrow {
    const size_t length = encoded.length();
    #if defined JSON_DEBUG || defined JSON_SAFE
	   size_t pos = encoded.find_first_not_of(chars64);
	   if (json_unlikely(pos != json_string::npos)){
		  JSON_ASSERT_SAFE(encoded[pos] == JSON_TEXT('='), json_global(ERROR_NOT_BASE64), return json_global(EMPTY_STD_STRING););
		  if (pos != length - 1){
			 JSON_ASSERT_SAFE(pos == length - 2, json_global(ERROR_NOT_BASE64), return json_global(EMPTY_STD_STRING););
			 JSON_ASSERT_SAFE(encoded[pos + 1] == JSON_TEXT('='), json_global(ERROR_NOT_BASE64), return json_global(EMPTY_STD_STRING););
		  }
	   }
    #endif
    const json_char * runner = encoded.c_str();
    size_t aligned = length / 4; //don't do the last ones as they might be = padding
    std::string result;
    if (json_likely(aligned != 0)){
	   --aligned;
	   result.reserve((size_t)((float)length / 1.37) + 1);

	   //first do the ones that can not have any padding
	   for (unsigned int i = 0; i < aligned; ++i){
		  const json_char second = toBinary(runner[1]);
		  const json_char third = toBinary(runner[2]);
		  result += (toBinary(runner[0]) << 2) + ((second & 0x30) >> 4);
		  result += ((second & 0xf) << 4) + ((third & 0x3c) >> 2);
		  result += ((third & 0x3) << 6) + toBinary(runner[3]);
		  runner += 4;
	   }

	   //now do the ones that might have padding, the first two characters can not be padding, so do them quickly
	   const json_char second = toBinary(runner[1]);
	   result += (toBinary(runner[0]) << 2) + ((second & 0x30) >> 4);
	   if (runner[2] != '='){  //not two = pads
		  const json_char third = toBinary(runner[2]);
		  result += ((second & 0xf) << 4) + ((third & 0x3c) >> 2);
		  if (runner[3] != '='){  //no padding
			 result += ((third & 0x3) << 6) + toBinary(runner[3]);
		  }
	   }
    }
    return result;
}

#endif
