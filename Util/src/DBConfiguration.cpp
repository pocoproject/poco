#include "Poco/Util/DBConfiguration.h"
#include "Poco/StringTokenizer.h"
#include "Poco/Data/Extraction.h"
#include "Poco/Data/Statement.h"
#include "Poco/Data/SQLite/SQLiteException.h"
#include "Poco/Data/SQLite/Connector.h"
#include <iostream>
#include "Poco/String.h"
#include "Poco/Format.h"


using Poco::format;
using Poco::Data::Session;
using namespace Poco::Data::Keywords;

namespace Poco {
namespace Util {

DBConfiguration::DBConfiguration(void)
{
}


DBConfiguration::~DBConfiguration(void)
{
}
DBConfiguration::DBConfiguration(const std::string& path)
{
	_path = path;
}

void DBConfiguration::removeRaw(const std::string& key)
{
	Poco::StringTokenizer token(key,".");
	if(token.count() != 2) return;

	std::string table_name  = token[0];
	std::string table_key   = token[1];

	Session session (Poco::Data::SQLite::Connector::KEY, _path);
	try
	{
		session << "DELETE  FROM " << table_name << " WHERE key=?", use(table_key), now;
	}
	catch(Poco::Data::SQLite::SQLiteException& e)
	{
		
	}

}

bool DBConfiguration::getRaw(const std::string & key, std::string & value) const
{
	Poco::StringTokenizer token(key,".");
	if(token.count() != 2) return false;
	std::string table_name = token[0];
	std::string table_key   = token[1];

	Session session (Poco::Data::SQLite::Connector::KEY, _path);
	try
	{
		session << "SELECT value FROM " + table_name + " WHERE key=?", into(value),use(table_key), now;
	}
	catch(Poco::Data::SQLite::SQLiteException& e)
	{
		
		return false;
	}
	
	return true;
}

bool DBConfiguration::existKey(Session& session,const std::string& table,const std::string& key)
{
	int count = 0;

	try
	{
		session << "SELECT COUNT(*) FROM " << table << " WHERE key=?", into(count),useRef(key), now;
	}
	catch(Poco::Data::SQLite::SQLiteException& e)
	{
		
		return false;
	}
	

	return (count>=1)?true:false;
}

void DBConfiguration::setRaw(const std::string& key, const std::string& value)
{
	Poco::StringTokenizer token(key,".");
	if(token.count() != 2) return;
	std::string table_name  = token[0];
	std::string table_key   = token[1];

	
	try
	{
		Session session (Poco::Data::SQLite::Connector::KEY, _path);

		std::string sql;

		Poco::format(sql,"CREATE TABLE IF NOT EXISTS %s (key VARCHAR,value VARCHAR)",table_name);

		session << sql, now;
	
	
		if(existKey(session,table_name,table_key))
		{
			session << "UPDATE "<<table_name<<" SET value=? WHERE key = ?", useRef(value), useRef(table_key), now;
		}
		else
		{
			session << "INSERT INTO " + table_name +" VALUES(?,?)",use(table_key),useRef(value), now;
		}
	}
	catch(Poco::Data::SQLite::SQLiteException& e)
	{
		
	}
	return;
}


void DBConfiguration::enumerate(const std::string& key, Keys& range) const
{
	Session session (Poco::Data::SQLite::Connector::KEY, _path);
	std::string table_name = key;
	try
	{
		session << "SELECT key FROM " << table_name, into(range), now;
	}
	catch(Poco::Data::SQLite::SQLiteException& e)
	{
		
	}
	
}

}}