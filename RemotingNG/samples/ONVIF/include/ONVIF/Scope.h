// 
// Scope.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Scope_INCLUDED
#define ONVIF_Scope_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Scope
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Scope
{
public:
	Scope();

	Scope(
		const std::string& scopeDef, 
		const Poco::URI& scopeItem);

	virtual ~Scope();

	const std::string& getScopeDef() const;

	const Poco::URI& getScopeItem() const;

	void setScopeDef(const std::string& val);

	void setScopeDef(std::string&& val);

	void setScopeItem(const Poco::URI& val);

	void setScopeItem(Poco::URI&& val);

private:
	//@ name=ScopeDef
	//@ order=0
	std::string _scopeDef;

	//@ name=ScopeItem
	//@ order=1
	Poco::URI _scopeItem;

};


inline const std::string& Scope::getScopeDef() const
{
	return _scopeDef;
}


inline const Poco::URI& Scope::getScopeItem() const
{
	return _scopeItem;
}


inline void Scope::setScopeDef(const std::string& val)
{
	_scopeDef = val;
}


inline void Scope::setScopeDef(std::string&& val)
{
	_scopeDef = std::move(val);
}


inline void Scope::setScopeItem(const Poco::URI& val)
{
	_scopeItem = val;
}


inline void Scope::setScopeItem(Poco::URI&& val)
{
	_scopeItem = std::move(val);
}


} // ONVIF


#endif // ONVIF_Scope_INCLUDED
