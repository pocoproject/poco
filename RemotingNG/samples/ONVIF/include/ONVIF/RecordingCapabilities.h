// 
// RecordingCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingCapabilities_INCLUDED
#define ONVIF_RecordingCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RecordingCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingCapabilities
{
public:
	RecordingCapabilities();

	RecordingCapabilities(
		const Poco::URI& xAddr, 
		bool receiverSource, 
		bool mediaProfileSource, 
		bool dynamicRecordings, 
		bool dynamicTracks, 
		int maxStringLength);

	virtual ~RecordingCapabilities();

	bool getDynamicRecordings() const;

	bool getDynamicTracks() const;

	int getMaxStringLength() const;

	bool getMediaProfileSource() const;

	bool getReceiverSource() const;

	const Poco::URI& getXAddr() const;

	void setDynamicRecordings(bool val);

	void setDynamicTracks(bool val);

	void setMaxStringLength(int val);

	void setMediaProfileSource(bool val);

	void setReceiverSource(bool val);

	void setXAddr(const Poco::URI& val);

	void setXAddr(Poco::URI&& val);

private:
	//@ name=XAddr
	//@ order=0
	Poco::URI _xAddr;

	//@ name=ReceiverSource
	//@ order=1
	bool _receiverSource;

	//@ name=MediaProfileSource
	//@ order=2
	bool _mediaProfileSource;

	//@ name=DynamicRecordings
	//@ order=3
	bool _dynamicRecordings;

	//@ name=DynamicTracks
	//@ order=4
	bool _dynamicTracks;

	//@ name=MaxStringLength
	//@ order=5
	int _maxStringLength;

};


inline bool RecordingCapabilities::getDynamicRecordings() const
{
	return _dynamicRecordings;
}


inline bool RecordingCapabilities::getDynamicTracks() const
{
	return _dynamicTracks;
}


inline int RecordingCapabilities::getMaxStringLength() const
{
	return _maxStringLength;
}


inline bool RecordingCapabilities::getMediaProfileSource() const
{
	return _mediaProfileSource;
}


inline bool RecordingCapabilities::getReceiverSource() const
{
	return _receiverSource;
}


inline const Poco::URI& RecordingCapabilities::getXAddr() const
{
	return _xAddr;
}


inline void RecordingCapabilities::setDynamicRecordings(bool val)
{
	_dynamicRecordings = val;
}


inline void RecordingCapabilities::setDynamicTracks(bool val)
{
	_dynamicTracks = val;
}


inline void RecordingCapabilities::setMaxStringLength(int val)
{
	_maxStringLength = val;
}


inline void RecordingCapabilities::setMediaProfileSource(bool val)
{
	_mediaProfileSource = val;
}


inline void RecordingCapabilities::setReceiverSource(bool val)
{
	_receiverSource = val;
}


inline void RecordingCapabilities::setXAddr(const Poco::URI& val)
{
	_xAddr = val;
}


inline void RecordingCapabilities::setXAddr(Poco::URI&& val)
{
	_xAddr = std::move(val);
}


} // ONVIF


#endif // ONVIF_RecordingCapabilities_INCLUDED
