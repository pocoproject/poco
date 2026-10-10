// 
// MediaCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MediaCapabilities_INCLUDED
#define ONVIF_MediaCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RealTimeStreamingCapabilities.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
class MediaCapabilitiesExtension;
} 


namespace ONVIF {


//@ name=MediaCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MediaCapabilities
{
public:
	MediaCapabilities();

	MediaCapabilities(
		const Poco::URI& xAddr, 
		const RealTimeStreamingCapabilities& streamingCapabilities, 
		const Poco::SharedPtr<MediaCapabilitiesExtension>& extension);

	virtual ~MediaCapabilities();

	const Poco::SharedPtr<MediaCapabilitiesExtension>& getExtension() const;

	const RealTimeStreamingCapabilities& getStreamingCapabilities() const;

	const Poco::URI& getXAddr() const;

	void setExtension(const Poco::SharedPtr<MediaCapabilitiesExtension>& val);

	void setExtension(Poco::SharedPtr<MediaCapabilitiesExtension>&& val);

	void setStreamingCapabilities(const RealTimeStreamingCapabilities& val);

	void setStreamingCapabilities(RealTimeStreamingCapabilities&& val);

	void setXAddr(const Poco::URI& val);

	void setXAddr(Poco::URI&& val);

private:
	//@ name=XAddr
	//@ order=0
	Poco::URI _xAddr;

	//@ name=StreamingCapabilities
	//@ order=1
	RealTimeStreamingCapabilities _streamingCapabilities;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<MediaCapabilitiesExtension> _extension;

};


inline const Poco::SharedPtr<MediaCapabilitiesExtension>& MediaCapabilities::getExtension() const
{
	return _extension;
}


inline const RealTimeStreamingCapabilities& MediaCapabilities::getStreamingCapabilities() const
{
	return _streamingCapabilities;
}


inline const Poco::URI& MediaCapabilities::getXAddr() const
{
	return _xAddr;
}


inline void MediaCapabilities::setExtension(const Poco::SharedPtr<MediaCapabilitiesExtension>& val)
{
	_extension = val;
}


inline void MediaCapabilities::setExtension(Poco::SharedPtr<MediaCapabilitiesExtension>&& val)
{
	_extension = std::move(val);
}


inline void MediaCapabilities::setStreamingCapabilities(const RealTimeStreamingCapabilities& val)
{
	_streamingCapabilities = val;
}


inline void MediaCapabilities::setStreamingCapabilities(RealTimeStreamingCapabilities&& val)
{
	_streamingCapabilities = std::move(val);
}


inline void MediaCapabilities::setXAddr(const Poco::URI& val)
{
	_xAddr = val;
}


inline void MediaCapabilities::setXAddr(Poco::URI&& val)
{
	_xAddr = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/MediaCapabilitiesExtension.h"


#endif // ONVIF_MediaCapabilities_INCLUDED
