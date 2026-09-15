// 
// ToneCompensationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ToneCompensationOptions_INCLUDED
#define ONVIF_ToneCompensationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ToneCompensationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ToneCompensationOptions
{
public:
	ToneCompensationOptions();

	ToneCompensationOptions(
		const std::vector<std::string>& mode, 
		bool level);

	virtual ~ToneCompensationOptions();

	bool getLevel() const;

	const std::vector<std::string>& getMode() const;

	std::vector<std::string>& getMode();

	void setLevel(bool val);

	void setMode(const std::vector<std::string>& val);

	void setMode(std::vector<std::string>&& val);

private:
	//@ name=Mode
	//@ order=0
	std::vector<std::string> _mode;

	//@ name=Level
	//@ order=1
	bool _level;

};


inline bool ToneCompensationOptions::getLevel() const
{
	return _level;
}


inline const std::vector<std::string>& ToneCompensationOptions::getMode() const
{
	return _mode;
}


inline std::vector<std::string>& ToneCompensationOptions::getMode()
{
	return _mode;
}


inline void ToneCompensationOptions::setLevel(bool val)
{
	_level = val;
}


inline void ToneCompensationOptions::setMode(const std::vector<std::string>& val)
{
	_mode = val;
}


inline void ToneCompensationOptions::setMode(std::vector<std::string>&& val)
{
	_mode = std::move(val);
}


} // ONVIF


#endif // ONVIF_ToneCompensationOptions_INCLUDED
