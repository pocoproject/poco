// 
// StopMulticastStreaming.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_StopMulticastStreaming_INCLUDED
#define ONVIF_Media_StopMulticastStreaming_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#StopMulticastStreaming"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API StopMulticastStreaming
{
public:
	StopMulticastStreaming();

	StopMulticastStreaming(const std::string& profileToken);

	virtual ~StopMulticastStreaming();

	const std::string& getProfileToken() const;

	void setProfileToken(const std::string& val);

	void setProfileToken(std::string&& val);

private:
	//@ name=ProfileToken
	//@ order=0
	std::string _profileToken;

};


inline const std::string& StopMulticastStreaming::getProfileToken() const
{
	return _profileToken;
}


inline void StopMulticastStreaming::setProfileToken(const std::string& val)
{
	_profileToken = val;
}


inline void StopMulticastStreaming::setProfileToken(std::string&& val)
{
	_profileToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_StopMulticastStreaming_INCLUDED
