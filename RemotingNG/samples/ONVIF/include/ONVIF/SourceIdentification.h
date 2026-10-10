// 
// SourceIdentification.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SourceIdentification_INCLUDED
#define ONVIF_SourceIdentification_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class SourceIdentificationExtension;
} 


namespace ONVIF {


//@ name=SourceIdentification
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SourceIdentification
{
public:
	SourceIdentification();

	SourceIdentification(
		const std::string& name, 
		const std::vector<std::string>& token, 
		const Poco::SharedPtr<SourceIdentificationExtension>& extension);

	virtual ~SourceIdentification();

	const Poco::SharedPtr<SourceIdentificationExtension>& getExtension() const;

	const std::string& getName() const;

	const std::vector<std::string>& getToken() const;

	std::vector<std::string>& getToken();

	void setExtension(const Poco::SharedPtr<SourceIdentificationExtension>& val);

	void setExtension(Poco::SharedPtr<SourceIdentificationExtension>&& val);

	void setName(const std::string& val);

	void setName(std::string&& val);

	void setToken(const std::vector<std::string>& val);

	void setToken(std::vector<std::string>&& val);

private:
	//@ name=Name
	//@ order=0
	std::string _name;

	//@ name=Token
	//@ order=1
	std::vector<std::string> _token;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<SourceIdentificationExtension> _extension;

};


inline const Poco::SharedPtr<SourceIdentificationExtension>& SourceIdentification::getExtension() const
{
	return _extension;
}


inline const std::string& SourceIdentification::getName() const
{
	return _name;
}


inline const std::vector<std::string>& SourceIdentification::getToken() const
{
	return _token;
}


inline std::vector<std::string>& SourceIdentification::getToken()
{
	return _token;
}


inline void SourceIdentification::setExtension(const Poco::SharedPtr<SourceIdentificationExtension>& val)
{
	_extension = val;
}


inline void SourceIdentification::setExtension(Poco::SharedPtr<SourceIdentificationExtension>&& val)
{
	_extension = std::move(val);
}


inline void SourceIdentification::setName(const std::string& val)
{
	_name = val;
}


inline void SourceIdentification::setName(std::string&& val)
{
	_name = std::move(val);
}


inline void SourceIdentification::setToken(const std::vector<std::string>& val)
{
	_token = val;
}


inline void SourceIdentification::setToken(std::vector<std::string>&& val)
{
	_token = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/SourceIdentificationExtension.h"


#endif // ONVIF_SourceIdentification_INCLUDED
