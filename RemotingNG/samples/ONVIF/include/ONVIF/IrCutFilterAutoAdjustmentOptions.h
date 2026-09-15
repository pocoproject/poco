// 
// IrCutFilterAutoAdjustmentOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IrCutFilterAutoAdjustmentOptions_INCLUDED
#define ONVIF_IrCutFilterAutoAdjustmentOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class DurationRange;
class IrCutFilterAutoAdjustmentOptionsExtension;
} 


namespace ONVIF {


//@ name=IrCutFilterAutoAdjustmentOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IrCutFilterAutoAdjustmentOptions
{
public:
	IrCutFilterAutoAdjustmentOptions();

	IrCutFilterAutoAdjustmentOptions(
		const std::vector<std::string>& boundaryType, 
		Poco::Optional<bool> boundaryOffset, 
		const Poco::SharedPtr<DurationRange>& responseTimeRange, 
		const Poco::SharedPtr<IrCutFilterAutoAdjustmentOptionsExtension>& extension);

	virtual ~IrCutFilterAutoAdjustmentOptions();

	Poco::Optional<bool> getBoundaryOffset() const;

	const std::vector<std::string>& getBoundaryType() const;

	std::vector<std::string>& getBoundaryType();

	const Poco::SharedPtr<IrCutFilterAutoAdjustmentOptionsExtension>& getExtension() const;

	const Poco::SharedPtr<DurationRange>& getResponseTimeRange() const;

	void setBoundaryOffset(Poco::Optional<bool> val);

	void setBoundaryType(const std::vector<std::string>& val);

	void setBoundaryType(std::vector<std::string>&& val);

	void setExtension(const Poco::SharedPtr<IrCutFilterAutoAdjustmentOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<IrCutFilterAutoAdjustmentOptionsExtension>&& val);

	void setResponseTimeRange(const Poco::SharedPtr<DurationRange>& val);

	void setResponseTimeRange(Poco::SharedPtr<DurationRange>&& val);

private:
	//@ name=BoundaryType
	//@ order=0
	std::vector<std::string> _boundaryType;

	//@ mandatory=false
	//@ name=BoundaryOffset
	//@ order=1
	Poco::Optional<bool> _boundaryOffset;

	//@ mandatory=false
	//@ name=ResponseTimeRange
	//@ order=2
	Poco::SharedPtr<DurationRange> _responseTimeRange;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<IrCutFilterAutoAdjustmentOptionsExtension> _extension;

};


inline Poco::Optional<bool> IrCutFilterAutoAdjustmentOptions::getBoundaryOffset() const
{
	return _boundaryOffset;
}


inline const std::vector<std::string>& IrCutFilterAutoAdjustmentOptions::getBoundaryType() const
{
	return _boundaryType;
}


inline std::vector<std::string>& IrCutFilterAutoAdjustmentOptions::getBoundaryType()
{
	return _boundaryType;
}


inline const Poco::SharedPtr<IrCutFilterAutoAdjustmentOptionsExtension>& IrCutFilterAutoAdjustmentOptions::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<DurationRange>& IrCutFilterAutoAdjustmentOptions::getResponseTimeRange() const
{
	return _responseTimeRange;
}


inline void IrCutFilterAutoAdjustmentOptions::setBoundaryOffset(Poco::Optional<bool> val)
{
	_boundaryOffset = val;
}


inline void IrCutFilterAutoAdjustmentOptions::setBoundaryType(const std::vector<std::string>& val)
{
	_boundaryType = val;
}


inline void IrCutFilterAutoAdjustmentOptions::setBoundaryType(std::vector<std::string>&& val)
{
	_boundaryType = std::move(val);
}


inline void IrCutFilterAutoAdjustmentOptions::setExtension(const Poco::SharedPtr<IrCutFilterAutoAdjustmentOptionsExtension>& val)
{
	_extension = val;
}


inline void IrCutFilterAutoAdjustmentOptions::setExtension(Poco::SharedPtr<IrCutFilterAutoAdjustmentOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void IrCutFilterAutoAdjustmentOptions::setResponseTimeRange(const Poco::SharedPtr<DurationRange>& val)
{
	_responseTimeRange = val;
}


inline void IrCutFilterAutoAdjustmentOptions::setResponseTimeRange(Poco::SharedPtr<DurationRange>&& val)
{
	_responseTimeRange = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/DurationRange.h"
#include "ONVIF/IrCutFilterAutoAdjustmentOptionsExtension.h"


#endif // ONVIF_IrCutFilterAutoAdjustmentOptions_INCLUDED
