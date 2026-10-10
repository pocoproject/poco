// 
// GetGuaranteedNumberOfVideoEncoderInstancesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetGuaranteedNumberOfVideoEncoderInstancesResponse_INCLUDED
#define ONVIF_Media_GetGuaranteedNumberOfVideoEncoderInstancesResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetGuaranteedNumberOfVideoEncoderInstancesResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetGuaranteedNumberOfVideoEncoderInstancesResponse
{
public:
	GetGuaranteedNumberOfVideoEncoderInstancesResponse();

	GetGuaranteedNumberOfVideoEncoderInstancesResponse(
		int totalNumber, 
		Poco::Optional<int> jPEG, 
		Poco::Optional<int> h264, 
		Poco::Optional<int> mPEG4);

	virtual ~GetGuaranteedNumberOfVideoEncoderInstancesResponse();

	Poco::Optional<int> getH264() const;

	Poco::Optional<int> getJPEG() const;

	Poco::Optional<int> getMPEG4() const;

	int getTotalNumber() const;

	void setH264(Poco::Optional<int> val);

	void setJPEG(Poco::Optional<int> val);

	void setMPEG4(Poco::Optional<int> val);

	void setTotalNumber(int val);

private:
	//@ name=TotalNumber
	//@ order=0
	int _totalNumber;

	//@ mandatory=false
	//@ name=JPEG
	//@ order=1
	Poco::Optional<int> _jPEG;

	//@ mandatory=false
	//@ name=H264
	//@ order=2
	Poco::Optional<int> _h264;

	//@ mandatory=false
	//@ name=MPEG4
	//@ order=3
	Poco::Optional<int> _mPEG4;

};


inline Poco::Optional<int> GetGuaranteedNumberOfVideoEncoderInstancesResponse::getH264() const
{
	return _h264;
}


inline Poco::Optional<int> GetGuaranteedNumberOfVideoEncoderInstancesResponse::getJPEG() const
{
	return _jPEG;
}


inline Poco::Optional<int> GetGuaranteedNumberOfVideoEncoderInstancesResponse::getMPEG4() const
{
	return _mPEG4;
}


inline int GetGuaranteedNumberOfVideoEncoderInstancesResponse::getTotalNumber() const
{
	return _totalNumber;
}


inline void GetGuaranteedNumberOfVideoEncoderInstancesResponse::setH264(Poco::Optional<int> val)
{
	_h264 = val;
}


inline void GetGuaranteedNumberOfVideoEncoderInstancesResponse::setJPEG(Poco::Optional<int> val)
{
	_jPEG = val;
}


inline void GetGuaranteedNumberOfVideoEncoderInstancesResponse::setMPEG4(Poco::Optional<int> val)
{
	_mPEG4 = val;
}


inline void GetGuaranteedNumberOfVideoEncoderInstancesResponse::setTotalNumber(int val)
{
	_totalNumber = val;
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetGuaranteedNumberOfVideoEncoderInstancesResponse_INCLUDED
