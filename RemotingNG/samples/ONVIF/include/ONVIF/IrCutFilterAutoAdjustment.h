// 
// IrCutFilterAutoAdjustment.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IrCutFilterAutoAdjustment_INCLUDED
#define ONVIF_IrCutFilterAutoAdjustment_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class IrCutFilterAutoAdjustmentExtension;
} 


namespace ONVIF {


//@ name=IrCutFilterAutoAdjustment
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IrCutFilterAutoAdjustment
{
public:
	IrCutFilterAutoAdjustment();

	IrCutFilterAutoAdjustment(
		const std::string& boundaryType, 
		Poco::Optional<float> boundaryOffset, 
		const Poco::Optional<std::string>& responseTime, 
		const Poco::SharedPtr<IrCutFilterAutoAdjustmentExtension>& extension);

	virtual ~IrCutFilterAutoAdjustment();

	Poco::Optional<float> getBoundaryOffset() const;

	const std::string& getBoundaryType() const;

	const Poco::SharedPtr<IrCutFilterAutoAdjustmentExtension>& getExtension() const;

	const Poco::Optional<std::string>& getResponseTime() const;

	void setBoundaryOffset(Poco::Optional<float> val);

	void setBoundaryType(const std::string& val);

	void setBoundaryType(std::string&& val);

	void setExtension(const Poco::SharedPtr<IrCutFilterAutoAdjustmentExtension>& val);

	void setExtension(Poco::SharedPtr<IrCutFilterAutoAdjustmentExtension>&& val);

	void setResponseTime(const Poco::Optional<std::string>& val);

	void setResponseTime(Poco::Optional<std::string>&& val);

private:
	//@ name=BoundaryType
	//@ order=0
	std::string _boundaryType;

	//@ mandatory=false
	//@ name=BoundaryOffset
	//@ order=1
	Poco::Optional<float> _boundaryOffset;

	//@ mandatory=false
	//@ name=ResponseTime
	//@ order=2
	Poco::Optional<std::string> _responseTime;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<IrCutFilterAutoAdjustmentExtension> _extension;

};


inline Poco::Optional<float> IrCutFilterAutoAdjustment::getBoundaryOffset() const
{
	return _boundaryOffset;
}


inline const std::string& IrCutFilterAutoAdjustment::getBoundaryType() const
{
	return _boundaryType;
}


inline const Poco::SharedPtr<IrCutFilterAutoAdjustmentExtension>& IrCutFilterAutoAdjustment::getExtension() const
{
	return _extension;
}


inline const Poco::Optional<std::string>& IrCutFilterAutoAdjustment::getResponseTime() const
{
	return _responseTime;
}


inline void IrCutFilterAutoAdjustment::setBoundaryOffset(Poco::Optional<float> val)
{
	_boundaryOffset = val;
}


inline void IrCutFilterAutoAdjustment::setBoundaryType(const std::string& val)
{
	_boundaryType = val;
}


inline void IrCutFilterAutoAdjustment::setBoundaryType(std::string&& val)
{
	_boundaryType = std::move(val);
}


inline void IrCutFilterAutoAdjustment::setExtension(const Poco::SharedPtr<IrCutFilterAutoAdjustmentExtension>& val)
{
	_extension = val;
}


inline void IrCutFilterAutoAdjustment::setExtension(Poco::SharedPtr<IrCutFilterAutoAdjustmentExtension>&& val)
{
	_extension = std::move(val);
}


inline void IrCutFilterAutoAdjustment::setResponseTime(const Poco::Optional<std::string>& val)
{
	_responseTime = val;
}


inline void IrCutFilterAutoAdjustment::setResponseTime(Poco::Optional<std::string>&& val)
{
	_responseTime = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/IrCutFilterAutoAdjustmentExtension.h"


#endif // ONVIF_IrCutFilterAutoAdjustment_INCLUDED
