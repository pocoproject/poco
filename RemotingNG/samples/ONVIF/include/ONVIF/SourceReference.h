// 
// SourceReference.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SourceReference_INCLUDED
#define ONVIF_SourceReference_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=SourceReference
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SourceReference
{
public:
	SourceReference();

	SourceReference(
		const Poco::Optional<Poco::URI>& type, 
		const std::string& token);

	virtual ~SourceReference();

	const std::string& getToken() const;

	const Poco::Optional<Poco::URI>& getType() const;

	void setToken(const std::string& val);

	void setToken(std::string&& val);

	void setType(const Poco::Optional<Poco::URI>& val);

	void setType(Poco::Optional<Poco::URI>&& val);

private:
	//@ mandatory=false
	//@ name=Type
	//@ order=0
	//@ type=attr
	Poco::Optional<Poco::URI> _type;

	//@ name=Token
	//@ order=1
	std::string _token;

};


inline const std::string& SourceReference::getToken() const
{
	return _token;
}


inline const Poco::Optional<Poco::URI>& SourceReference::getType() const
{
	return _type;
}


inline void SourceReference::setToken(const std::string& val)
{
	_token = val;
}


inline void SourceReference::setToken(std::string&& val)
{
	_token = std::move(val);
}


inline void SourceReference::setType(const Poco::Optional<Poco::URI>& val)
{
	_type = val;
}


inline void SourceReference::setType(Poco::Optional<Poco::URI>&& val)
{
	_type = std::move(val);
}


} // ONVIF


#endif // ONVIF_SourceReference_INCLUDED
