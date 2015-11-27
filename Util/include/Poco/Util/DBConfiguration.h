
#ifndef POCO_UTIL_NO_JSONCONFIGURATION

#ifndef Util_DBConfiguration_INCLUDED
#define Util_DBConfiguration_INCLUDED


#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Data/Session.h"
#include <istream>
#include <string>
namespace Poco {
namespace Util {

class Util_API DBConfiguration :
	public AbstractConfiguration
{
public:
	DBConfiguration(void);
	virtual ~DBConfiguration(void);
	DBConfiguration(const std::string& path);
	void load(const std::string& path);
	void load(std::istream& istr);
	virtual void removeRaw(const std::string& key);	

	/// Loads the configuration from the given stream
protected:

	bool getRaw(const std::string & key, std::string & value) const;


	void setRaw(const std::string& key, const std::string& value);


	void enumerate(const std::string& key, Keys& range) const;

private:
	bool existKey(Poco::Data::Session& session,const std::string& table,const std::string& key);
	std::string _path;

};
}}
#endif

#endif