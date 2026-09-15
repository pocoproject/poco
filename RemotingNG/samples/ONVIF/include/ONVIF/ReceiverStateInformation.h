// 
// ReceiverStateInformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ReceiverStateInformation_INCLUDED
#define ONVIF_ReceiverStateInformation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ReceiverStateInformation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ReceiverStateInformation
{
public:
	ReceiverStateInformation();

	ReceiverStateInformation(
		const std::string& state, 
		bool autoCreated);

	virtual ~ReceiverStateInformation();

	bool getAutoCreated() const;

	const std::string& getState() const;

	void setAutoCreated(bool val);

	void setState(const std::string& val);

	void setState(std::string&& val);

private:
	//@ name=State
	//@ order=0
	std::string _state;

	//@ name=AutoCreated
	//@ order=1
	bool _autoCreated;

};


inline bool ReceiverStateInformation::getAutoCreated() const
{
	return _autoCreated;
}


inline const std::string& ReceiverStateInformation::getState() const
{
	return _state;
}


inline void ReceiverStateInformation::setAutoCreated(bool val)
{
	_autoCreated = val;
}


inline void ReceiverStateInformation::setState(const std::string& val)
{
	_state = val;
}


inline void ReceiverStateInformation::setState(std::string&& val)
{
	_state = std::move(val);
}


} // ONVIF


#endif // ONVIF_ReceiverStateInformation_INCLUDED
