// 
// StreamSetup.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_StreamSetup_INCLUDED
#define ONVIF_StreamSetup_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Transport.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=StreamSetup
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API StreamSetup
{
public:
	StreamSetup();

	StreamSetup(
		const std::string& stream, 
		const Transport& transport);

	virtual ~StreamSetup();

	const std::string& getStream() const;

	const Transport& getTransport() const;

	void setStream(const std::string& val);

	void setStream(std::string&& val);

	void setTransport(const Transport& val);

	void setTransport(Transport&& val);

private:
	//@ name=Stream
	//@ order=0
	std::string _stream;

	//@ name=Transport
	//@ order=1
	Transport _transport;

};


inline const std::string& StreamSetup::getStream() const
{
	return _stream;
}


inline const Transport& StreamSetup::getTransport() const
{
	return _transport;
}


inline void StreamSetup::setStream(const std::string& val)
{
	_stream = val;
}


inline void StreamSetup::setStream(std::string&& val)
{
	_stream = std::move(val);
}


inline void StreamSetup::setTransport(const Transport& val)
{
	_transport = val;
}


inline void StreamSetup::setTransport(Transport&& val)
{
	_transport = std::move(val);
}


} // ONVIF


#endif // ONVIF_StreamSetup_INCLUDED
