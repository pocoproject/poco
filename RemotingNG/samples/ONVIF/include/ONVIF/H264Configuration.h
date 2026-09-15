// 
// H264Configuration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_H264Configuration_INCLUDED
#define ONVIF_H264Configuration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=H264Configuration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API H264Configuration
{
public:
	H264Configuration();

	H264Configuration(
		int govLength, 
		const std::string& h264Profile);

	virtual ~H264Configuration();

	int getGovLength() const;

	const std::string& getH264Profile() const;

	void setGovLength(int val);

	void setH264Profile(const std::string& val);

	void setH264Profile(std::string&& val);

private:
	//@ name=GovLength
	//@ order=0
	int _govLength;

	//@ name=H264Profile
	//@ order=1
	std::string _h264Profile;

};


inline int H264Configuration::getGovLength() const
{
	return _govLength;
}


inline const std::string& H264Configuration::getH264Profile() const
{
	return _h264Profile;
}


inline void H264Configuration::setGovLength(int val)
{
	_govLength = val;
}


inline void H264Configuration::setH264Profile(const std::string& val)
{
	_h264Profile = val;
}


inline void H264Configuration::setH264Profile(std::string&& val)
{
	_h264Profile = std::move(val);
}


} // ONVIF


#endif // ONVIF_H264Configuration_INCLUDED
