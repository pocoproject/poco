// 
// RecordingEncryption.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingEncryption_INCLUDED
#define ONVIF_RecordingEncryption_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RecordingEncryption
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingEncryption
{
public:
	RecordingEncryption();

	RecordingEncryption(
		const std::string& mode, 
		const std::string& kID, 
		const Poco::Optional<std::string>& key, 
		const std::vector<std::string>& track);

	virtual ~RecordingEncryption();

	const std::string& getKID() const;

	const Poco::Optional<std::string>& getKey() const;

	const std::string& getMode() const;

	const std::vector<std::string>& getTrack() const;

	std::vector<std::string>& getTrack();

	void setKID(const std::string& val);

	void setKID(std::string&& val);

	void setKey(const Poco::Optional<std::string>& val);

	void setKey(Poco::Optional<std::string>&& val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

	void setTrack(const std::vector<std::string>& val);

	void setTrack(std::vector<std::string>&& val);

private:
	//@ name=Mode
	//@ order=0
	//@ type=attr
	std::string _mode;

	//@ name=KID
	//@ order=1
	std::string _kID;

	//@ mandatory=false
	//@ name=Key
	//@ order=2
	Poco::Optional<std::string> _key;

	//@ mandatory=false
	//@ name=Track
	//@ order=3
	std::vector<std::string> _track;

};


inline const std::string& RecordingEncryption::getKID() const
{
	return _kID;
}


inline const Poco::Optional<std::string>& RecordingEncryption::getKey() const
{
	return _key;
}


inline const std::string& RecordingEncryption::getMode() const
{
	return _mode;
}


inline const std::vector<std::string>& RecordingEncryption::getTrack() const
{
	return _track;
}


inline std::vector<std::string>& RecordingEncryption::getTrack()
{
	return _track;
}


inline void RecordingEncryption::setKID(const std::string& val)
{
	_kID = val;
}


inline void RecordingEncryption::setKID(std::string&& val)
{
	_kID = std::move(val);
}


inline void RecordingEncryption::setKey(const Poco::Optional<std::string>& val)
{
	_key = val;
}


inline void RecordingEncryption::setKey(Poco::Optional<std::string>&& val)
{
	_key = std::move(val);
}


inline void RecordingEncryption::setMode(const std::string& val)
{
	_mode = val;
}


inline void RecordingEncryption::setMode(std::string&& val)
{
	_mode = std::move(val);
}


inline void RecordingEncryption::setTrack(const std::vector<std::string>& val)
{
	_track = val;
}


inline void RecordingEncryption::setTrack(std::vector<std::string>&& val)
{
	_track = std::move(val);
}


} // ONVIF


#endif // ONVIF_RecordingEncryption_INCLUDED
