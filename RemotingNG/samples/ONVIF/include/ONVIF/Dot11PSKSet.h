// 
// Dot11PSKSet.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Dot11PSKSet_INCLUDED
#define ONVIF_Dot11PSKSet_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class Dot11PSKSetExtension;
} 


namespace ONVIF {


//@ name=Dot11PSKSet
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Dot11PSKSet
{
public:
	Dot11PSKSet();

	Dot11PSKSet(
		const Poco::Optional<std::string>& key, 
		const Poco::Optional<std::string>& passphrase, 
		const Poco::SharedPtr<Dot11PSKSetExtension>& extension);

	virtual ~Dot11PSKSet();

	const Poco::SharedPtr<Dot11PSKSetExtension>& getExtension() const;

	const Poco::Optional<std::string>& getKey() const;

	const Poco::Optional<std::string>& getPassphrase() const;

	void setExtension(const Poco::SharedPtr<Dot11PSKSetExtension>& val);

	void setExtension(Poco::SharedPtr<Dot11PSKSetExtension>&& val);

	void setKey(const Poco::Optional<std::string>& val);

	void setKey(Poco::Optional<std::string>&& val);

	void setPassphrase(const Poco::Optional<std::string>& val);

	void setPassphrase(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Key
	//@ order=0
	Poco::Optional<std::string> _key;

	//@ mandatory=false
	//@ name=Passphrase
	//@ order=1
	Poco::Optional<std::string> _passphrase;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<Dot11PSKSetExtension> _extension;

};


inline const Poco::SharedPtr<Dot11PSKSetExtension>& Dot11PSKSet::getExtension() const
{
	return _extension;
}


inline const Poco::Optional<std::string>& Dot11PSKSet::getKey() const
{
	return _key;
}


inline const Poco::Optional<std::string>& Dot11PSKSet::getPassphrase() const
{
	return _passphrase;
}


inline void Dot11PSKSet::setExtension(const Poco::SharedPtr<Dot11PSKSetExtension>& val)
{
	_extension = val;
}


inline void Dot11PSKSet::setExtension(Poco::SharedPtr<Dot11PSKSetExtension>&& val)
{
	_extension = std::move(val);
}


inline void Dot11PSKSet::setKey(const Poco::Optional<std::string>& val)
{
	_key = val;
}


inline void Dot11PSKSet::setKey(Poco::Optional<std::string>&& val)
{
	_key = std::move(val);
}


inline void Dot11PSKSet::setPassphrase(const Poco::Optional<std::string>& val)
{
	_passphrase = val;
}


inline void Dot11PSKSet::setPassphrase(Poco::Optional<std::string>&& val)
{
	_passphrase = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/Dot11PSKSetExtension.h"


#endif // ONVIF_Dot11PSKSet_INCLUDED
