// 
// ProblemAction.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_Addressing_ProblemAction_INCLUDED
#define W3_Addressing_ProblemAction_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace W3 {
namespace Addressing {
class AttributedURI;
} } 


namespace W3 {
namespace Addressing {


//@ name=ProblemActionType
//@ namespace="http://www.w3.org/2005/08/addressing"
//@ serialize
class ONVIF_API ProblemAction
{
public:
	ProblemAction();

	ProblemAction(
		const Poco::SharedPtr<AttributedURI>& action, 
		const Poco::Optional<Poco::URI>& soapAction);

	virtual ~ProblemAction();

	const Poco::SharedPtr<AttributedURI>& getAction() const;

	const Poco::Optional<Poco::URI>& getSoapAction() const;

	void setAction(const Poco::SharedPtr<AttributedURI>& val);

	void setAction(Poco::SharedPtr<AttributedURI>&& val);

	void setSoapAction(const Poco::Optional<Poco::URI>& val);

	void setSoapAction(Poco::Optional<Poco::URI>&& val);

private:
	//@ mandatory=false
	//@ name=Action
	//@ order=0
	Poco::SharedPtr<AttributedURI> _action;

	//@ mandatory=false
	//@ name=SoapAction
	//@ order=1
	Poco::Optional<Poco::URI> _soapAction;

};


inline const Poco::SharedPtr<AttributedURI>& ProblemAction::getAction() const
{
	return _action;
}


inline const Poco::Optional<Poco::URI>& ProblemAction::getSoapAction() const
{
	return _soapAction;
}


inline void ProblemAction::setAction(const Poco::SharedPtr<AttributedURI>& val)
{
	_action = val;
}


inline void ProblemAction::setAction(Poco::SharedPtr<AttributedURI>&& val)
{
	_action = std::move(val);
}


inline void ProblemAction::setSoapAction(const Poco::Optional<Poco::URI>& val)
{
	_soapAction = val;
}


inline void ProblemAction::setSoapAction(Poco::Optional<Poco::URI>&& val)
{
	_soapAction = std::move(val);
}


} } // W3::Addressing


// The following headers are required for template instantiation.
#include "W3/Addressing/AttributedURI.h"


#endif // W3_Addressing_ProblemAction_INCLUDED
