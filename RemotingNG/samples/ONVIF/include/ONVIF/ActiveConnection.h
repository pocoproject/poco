// 
// ActiveConnection.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ActiveConnection_INCLUDED
#define ONVIF_ActiveConnection_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ActiveConnection
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ActiveConnection
{
public:
	ActiveConnection();

	ActiveConnection(
		float currentBitrate, 
		float currentFps);

	virtual ~ActiveConnection();

	float getCurrentBitrate() const;

	float getCurrentFps() const;

	void setCurrentBitrate(float val);

	void setCurrentFps(float val);

private:
	//@ name=CurrentBitrate
	//@ order=0
	float _currentBitrate;

	//@ name=CurrentFps
	//@ order=1
	float _currentFps;

};


inline float ActiveConnection::getCurrentBitrate() const
{
	return _currentBitrate;
}


inline float ActiveConnection::getCurrentFps() const
{
	return _currentFps;
}


inline void ActiveConnection::setCurrentBitrate(float val)
{
	_currentBitrate = val;
}


inline void ActiveConnection::setCurrentFps(float val)
{
	_currentFps = val;
}


} // ONVIF


#endif // ONVIF_ActiveConnection_INCLUDED
