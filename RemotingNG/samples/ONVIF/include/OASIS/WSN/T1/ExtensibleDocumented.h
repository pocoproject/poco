// 
// ExtensibleDocumented.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_T1_ExtensibleDocumented_INCLUDED
#define OASIS_WSN_T1_ExtensibleDocumented_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSN {
namespace T1 {
class Documentation;
} } } 


namespace OASIS {
namespace WSN {
namespace T1 {


//@ name=ExtensibleDocumented
//@ namespace="http://docs.oasis-open.org/wsn/t-1"
//@ serialize
class ONVIF_API ExtensibleDocumented
{
public:
	ExtensibleDocumented();

	ExtensibleDocumented(const Poco::SharedPtr<Documentation>& documentation);

	virtual ~ExtensibleDocumented();

	const Poco::SharedPtr<Documentation>& getDocumentation() const;

	void setDocumentation(const Poco::SharedPtr<Documentation>& val);

	void setDocumentation(Poco::SharedPtr<Documentation>&& val);

private:
	//@ mandatory=false
	//@ name=documentation
	//@ order=0
	Poco::SharedPtr<Documentation> _documentation;

};


inline const Poco::SharedPtr<Documentation>& ExtensibleDocumented::getDocumentation() const
{
	return _documentation;
}


inline void ExtensibleDocumented::setDocumentation(const Poco::SharedPtr<Documentation>& val)
{
	_documentation = val;
}


inline void ExtensibleDocumented::setDocumentation(Poco::SharedPtr<Documentation>&& val)
{
	_documentation = std::move(val);
}


} } } // OASIS::WSN::T1


// The following headers are required for template instantiation.
#include "OASIS/WSN/T1/Documentation.h"


#endif // OASIS_WSN_T1_ExtensibleDocumented_INCLUDED
