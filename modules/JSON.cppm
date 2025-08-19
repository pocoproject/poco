module;

#define ENABLE_JSON

#ifdef ENABLE_JSON
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Handler.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParserImpl.h"
#include "Poco/JSON/PrintHandler.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/JSON/TemplateCache.h"
#include "Poco/JSON/Template.h"
#endif

export module Poco.JSON;

export namespace Poco::JSON {
	#ifdef ENABLE_JSON
	using Poco::JSON::Array;
	using Poco::JSON::Handler;
	using Poco::JSON::JSONException;
	using Poco::JSON::JSONTemplateException;
	using Poco::JSON::Object;
	using Poco::JSON::ParseHandler;
	using Poco::JSON::Parser;
	using Poco::JSON::ParserImpl;
	using Poco::JSON::PrintHandler;
	using Poco::JSON::Query;
	using Poco::JSON::Stringifier;
	using Poco::JSON::Template;
	using Poco::JSON::TemplateCache;
	#endif
}
